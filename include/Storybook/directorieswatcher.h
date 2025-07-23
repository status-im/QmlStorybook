#pragma once

#include <QObject>

#include <QtQmlIntegration/qqmlintegration.h>

class QFileSystemWatcher;

class DirectoriesWatcher : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DirectoriesWatcher(QObject *parent = nullptr);
    void addPaths(const QStringList &paths);

signals:
    void changed();

private:
    QFileSystemWatcher* fsWatcher;
};
