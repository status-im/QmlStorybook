#pragma once

#include <QObject>
#include <QStringList>

class QQmlEngine;

class QmlEngineUtils : public QObject
{
    Q_OBJECT

public:
    explicit QmlEngineUtils(QQmlEngine* engine);

    Q_INVOKABLE void clearComponentCache() const;
    Q_INVOKABLE QStringList importPaths() const;
    Q_INVOKABLE bool removeImportPath(const QString& path) const;
    Q_INVOKABLE void addImportPath(const QString& path) const;
    Q_INVOKABLE void setImportPaths(const QStringList& paths) const;

private:
    QQmlEngine* m_engine = nullptr;
};
