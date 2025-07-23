#pragma once

#include <QObject>

#include <QtQmlIntegration/qqmlintegration.h>

class QQmlEngine;

class CacheCleaner : public QObject
{
    Q_OBJECT
    QML_SINGLETON
public:
    explicit CacheCleaner(QQmlEngine* engine);
    Q_INVOKABLE void clearComponentCache() const;

private:
    QQmlEngine* engine;
};
