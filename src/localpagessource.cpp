#include "Storybook/localpagessource.h"

#include <QFileInfo>

#include "Storybook/directoryfileswatcher.h"

LocalPagesSource::LocalPagesSource(const QString &path, QObject *parent)
    : AbstractPagesSource{parent}, m_path(path),
    m_pagesWatcher(new DirectoryFilesWatcher(
          path, QStringLiteral("*Page.qml"), this))
{
    connect(m_pagesWatcher, &DirectoryFilesWatcher::filesChanged, this,
            &LocalPagesSource::pagesChanged);
}

QStringList LocalPagesSource::pages() const
{
    return m_pagesWatcher->files();
}

QString LocalPagesSource::page(const QString &page) const
{
    QFile file(page);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.fileName();
        return {};
    }

    QByteArray content = file.readAll();
    return QString::fromUtf8(content);
}
