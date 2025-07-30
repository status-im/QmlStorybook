#include "Storybook/localpagessource.h"

#include "Storybook/directoryfileswatcher.h"

#include <QFileInfo>
#include <QDir>

using namespace Qt::Literals::StringLiterals;

LocalPagesSource::LocalPagesSource(const QString &path, QObject *parent)
    : AbstractPagesSource{parent}, m_path(path),
    m_pagesWatcher(new DirectoryFilesWatcher(
          path, u"*Page.qml"_s, this))
{
    connect(m_pagesWatcher, &DirectoryFilesWatcher::filesChanged, this,
            [this](const QStringList& added, const QStringList& removed, const QStringList& changed) {
        emit pagesChanged(extractPageNames(added),
                          extractPageNames(removed),
                          extractPageNames(changed));
    });
}

QStringList LocalPagesSource::pages() const
{
    return extractPageNames(m_pagesWatcher->files());
}

QString LocalPagesSource::page(const QString &page) const
{
    QFile file(QDir(m_path).absoluteFilePath(page + u"Page.qml"_s));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.fileName();
        return {};
    }

    QByteArray content = file.readAll();
    return QString::fromUtf8(content);
}

QString LocalPagesSource::extractPageName(const QString &path)
{
    return QFileInfo(path).fileName().chopped((u"Page.qml"_s).size());
}

QStringList LocalPagesSource::extractPageNames(const QStringList &paths)
{
    QStringList pages;
    pages.reserve(paths.size());

    std::transform(paths.begin(), paths.end(), std::back_inserter(pages),
                   [](auto path) { return extractPageName(path); });

    return pages;
}
