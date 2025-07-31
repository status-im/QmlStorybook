#include "Storybook/qmlfilesserver.h"

#include "Storybook/directorieswatcher.h"

#include <QDir>
#include <QFileInfo>
#include <QHttpServer>
#include <QJsonArray>
#include <QJsonObject>
#include <QMimeDatabase>
#include <QTcpServer>

#include <memory>

using namespace Qt::Literals::StringLiterals;

QmlFilesServer::QmlFilesServer(QStringList basePaths, QString pagesPath, QObject *parent)
    : QObject{parent}, m_server(new QHttpServer(this)),
    m_basePaths(std::move(basePaths)), m_pagesPath(std::move(pagesPath))
{
    if (!m_basePaths.contains(m_pagesPath))
        m_basePaths << m_pagesPath;

    auto watcher = new DirectoriesWatcher(this);
    watcher->addPaths(m_basePaths);

    connect(watcher, &DirectoriesWatcher::changed, this,
            [this]() { m_version++; });
}

void QmlFilesServer::start(quint16 port)
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

        QString filePath = findFirstExistingFile(relativePath);

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
        return;
    }

    qDebug() << "Listening on port" << tcpserver->serverPort();
    tcpserver.release();
}

bool QmlFilesServer::isRelativePathSafe(const QString &relativePath)
{
    const auto parts = relativePath.split('/', Qt::SkipEmptyParts);
    return !parts.contains(u".."_s);
}

QString QmlFilesServer::findFirstExistingFile(const QString &relativePath) const
{
    for (auto& base : m_basePaths)
    {
        QString fullPath = QDir(base).absoluteFilePath(
            QUrl::fromPercentEncoding(relativePath.toUtf8()));

        if (QFileInfo::exists(fullPath) && QFileInfo(fullPath).isFile())
            return fullPath;
    }
    return QString();
}
