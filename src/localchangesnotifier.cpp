#include "Storybook/localchangesnotifier.h"

#include <QFileSystemWatcher>
#include <QDirIterator>

LocalChangesNotifier::LocalChangesNotifier(QObject *parent)
    : AbstractChangesNotifier{parent}, fsWatcher(new QFileSystemWatcher(this))
{
    connect(fsWatcher, &QFileSystemWatcher::directoryChanged,
            this, &LocalChangesNotifier::changed);
}

void LocalChangesNotifier::addPaths(const QStringList &paths)
{
    for (auto& path : paths) {
        QDirIterator it(path, QDir::AllDirs | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories);

        while (it.hasNext()) {
            const auto& subpath = it.filePath();

            if (!subpath.isEmpty())
                fsWatcher->addPath(subpath);

            it.next();
        }

        fsWatcher->addPath(path);
    }
}
