#include "Storybook/qmlfilesserver.h"

#include "Storybook/localchangesnotifier.h"

#include <QDir>
#include <QFileInfo>
#include <QHttpServer>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QTcpServer>
#include <QSet>

#include <memory>

using namespace Qt::Literals::StringLiterals;

namespace {

bool isRelativePathSafe(const QString &relativePath)
{
    const auto parts = relativePath.split('/', Qt::SkipEmptyParts);
    return !parts.contains(u".."_s);
}

QString amendedQmldirContent(const QString& qmldirPath) {
    QFileInfo qmldirInfo(qmldirPath);
    QString dirPath = qmldirInfo.absolutePath();

    QStringList qmldirLines;
    if (qmldirInfo.exists() && qmldirInfo.isFile()) {
        QFile qmldirFile(qmldirPath);
        if (qmldirFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&qmldirFile);
            while (!in.atEnd())
                qmldirLines << in.readLine();
            qmldirFile.close();
        }
    }

    // Find version of the first QML module entry and if singleton is used
    static const QRegularExpression entryRegex(
        R"(^\s*(singleton\s+)?(\w+)\s+(\d+\.\d+)\s+(\S+\.qml)(?:\s+.*)?$)");

    QString version = "1.0"; // fallback default
    QString singletonKeyword;
    QSet<QString> listedFiles;

    for (auto& line : std::as_const(qmldirLines)) {
        QRegularExpressionMatch m = entryRegex.match(line);
        if (m.hasMatch()) {
            if (version == "1.0")
                version = m.captured(3);
            if (singletonKeyword.isEmpty() && !m.captured(1).isEmpty())
                singletonKeyword = m.captured(1); // e.g. "singleton "
            listedFiles.insert(m.captured(4));
        }
    }

    // List all QML files in the same directory as qmldir
    QDir dir(dirPath);
    QStringList qmlFiles = dir.entryList({"*.qml"}, QDir::Files);

    // Find unlisted QML files
    QSet<QString> qmlFilesSet(qmlFiles.begin(), qmlFiles.end());
    QSet<QString> unlistedFiles = qmlFilesSet - listedFiles;

    // If the file doesn't exist and there are no qml files, return empty string
    if (qmldirLines.isEmpty() && unlistedFiles.isEmpty())
        return {};

    // Prepare new entries
    QStringList newEntries;
    for (auto& filename : std::as_const(unlistedFiles)) {
        QString typeName = QFileInfo(filename).baseName();
        newEntries << QString("%1 %2 %3").arg(typeName, version, filename);
    }
    newEntries.sort();

    QStringList amendedLines = qmldirLines;
    if (!amendedLines.isEmpty() && !amendedLines.last().isEmpty() && !newEntries.isEmpty())
        amendedLines << ""; // Ensure a newline before appending
    amendedLines.append(newEntries);

    return amendedLines.join('\n');
}

} // unnamed namespace

QmlFilesServer::QmlFilesServer(QStringList basePaths, QString pagesPath,
                               bool amendQmldirs, QObject *parent)
    : QObject{parent}, m_server(new QHttpServer(this)),
    m_basePaths(std::move(basePaths)), m_pagesPath(std::move(pagesPath)),
    m_amendQmldirs(amendQmldirs)
{
    if (!m_basePaths.contains(m_pagesPath))
        m_basePaths << m_pagesPath;

    auto watcher = new LocalChangesNotifier(this);
    watcher->addPaths(m_basePaths);

    connect(watcher, &LocalChangesNotifier::changed, this,
            [this]() { m_version++; });
}

void QmlFilesServer::setFileSelectors(const QStringList& selectors)
{
    m_fileSelectors = selectors;
}

const QStringList& QmlFilesServer::fileSelectors() const
{
    return m_fileSelectors;
}

bool QmlFilesServer::start(quint16 port)
{
    m_server->route(u"/version"_s, QHttpServerRequest::Method::Get,
                    [this] { return QString::number(m_version); });

    m_server->route(u"/pages"_s, QHttpServerRequest::Method::Get, [this] {

        QDir dir(m_pagesPath);
        dir.setFilter(QDir::Files);
        dir.setNameFilters({u"*Page.qml"_s});

        const QFileInfoList filesInfo = dir.entryInfoList();

        QJsonArray files;
        std::transform(filesInfo.begin(), filesInfo.end(),
                       std::back_inserter(files),
                       [] (auto &info) {

                           return QJsonObject {
                               {
                                   { u"page"_s, info.fileName() },
                                   { u"timestamp"_s, info.lastModified().toMSecsSinceEpoch() }
                               }
                           };
                       });

        return files;
    });

    m_server->route(u"/.*"_s, [this](const QHttpServerRequest &request) {
        QString relativePath = request.url().path();

        // drop trailing "/" or "/<number>/"
        static const QRegularExpression trimRegex(u"^((/\\d+/)|/)"_s);
        relativePath = relativePath.replace(trimRegex, "");

        bool isSafe = isRelativePathSafe(relativePath);

        if (!isSafe)
            return QHttpServerResponse(
                "400 Directory traversal is not allowed",
                QHttpServerResponder::StatusCode::BadRequest);

        QFileInfo info(relativePath);

        if (info.fileName() == u"qmldir"_s) {
            QString path = findFirstExistingSourceDir(
                QFileInfo(QUrl::fromPercentEncoding(relativePath.toUtf8())).path());

            QString body = amendedQmldirContent(QDir(path).filePath(u"qmldir"_s));

            if(body.isEmpty()) {
                return QHttpServerResponse(
                    "404 Not Found", QHttpServerResponder::StatusCode::NotFound);
            }

            return QHttpServerResponse("text/plain"_ba,
                                       body.toUtf8(), QHttpServerResponder::StatusCode::Ok);
        }

        QString filePath = findFirstExistingFile(
            QUrl::fromPercentEncoding(relativePath.toUtf8()));

        if (filePath.isEmpty()) {
            return QHttpServerResponse(
                "404 Not Found", QHttpServerResponder::StatusCode::NotFound);
        }

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            return QHttpServerResponse(
                "403 Forbidden", QHttpServerResponder::StatusCode::Forbidden);

        static const QMimeDatabase mimeDb;
        QFileInfo fileInfo(filePath);
        QMimeType mime = mimeDb.mimeTypeForFile(fileInfo);
        QByteArray body = file.readAll();

        QHttpServerResponse resp(
            mime.isValid() ? mime.name().toUtf8() : "application/octet-stream"_ba,
            body, QHttpServerResponder::StatusCode::Ok);

        return resp;
    });

    auto tcpserver = std::make_unique<QTcpServer>(this);
    if (!tcpserver->listen(QHostAddress::Any, port) ||
        !m_server->bind(tcpserver.get()))
    {
        qWarning() << "Failed to start server on port" << port;
        return false;
    }

    qDebug() << "Listening on port" << tcpserver->serverPort();
    tcpserver.release();

    return true;
}

QString QmlFilesServer::findFirstExistingSourceDir(const QString &relativePath) const
{
    for (auto& base : m_basePaths)
    {
        QString fullDirPath = QDir(base).absoluteFilePath(relativePath);

        if (QFileInfo::exists(fullDirPath) && QFileInfo(fullDirPath).isDir())
        {
            if (!QDir(fullDirPath).entryList({u"*.qml"_s, u"qmldir"_s},
                                             QDir::Files).isEmpty())
                return fullDirPath;
        }
    }
    return {};
}

QString QmlFilesServer::findFirstExistingFile(const QString &relativePath) const
{
    auto valid = [](auto path) {
        QFileInfo info(path);
        return info.exists() && info.isFile();
    };

    QFileInfo info(relativePath);
    QString relativeDir = info.path();
    QString file = info.fileName();

    for (auto& base : m_basePaths)
    {
        for (const auto& selector : m_fileSelectors) {
            QString withSelector = QDir::cleanPath(relativeDir + "/" +
                                                   selector + "/" + file);
            QString fullPath = QDir(base).absoluteFilePath(withSelector);

            if (valid(fullPath))
                return fullPath;
        }

        QString fullPath = QDir(base).absoluteFilePath(relativePath);

        if (valid(fullPath))
            return fullPath;
    }
    return {};
}
