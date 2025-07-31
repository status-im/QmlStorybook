#pragma once

#include "Storybook/abstractchangesnotifier.h"

#include <QObject>

class QFileSystemWatcher;

class LocalChangesNotifier : public AbstractChangesNotifier
{
    Q_OBJECT
public:
    explicit LocalChangesNotifier(QObject *parent = nullptr);
    void addPaths(const QStringList &paths);

private:
    QFileSystemWatcher* fsWatcher;
};
