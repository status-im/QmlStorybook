#include "Storybook/qmlengineutils.h"

#include <QQmlEngine>

QmlEngineUtils::QmlEngineUtils(QQmlEngine* engine)
    : m_engine(engine)
{
}

void QmlEngineUtils::clearComponentCache() const {
    m_engine->clearComponentCache();
}


QStringList QmlEngineUtils::importPaths() const
{
    return m_engine->importPathList();
}

bool QmlEngineUtils::removeImportPath(const QString& path) const
{
    auto paths = m_engine->importPathList();
    int idx = paths.indexOf(path);

    if (idx == -1)
        return false;

    paths.remove(idx);
    m_engine->setImportPathList(paths);

    return true;
}

void QmlEngineUtils::addImportPath(const QString& path) const
{
    m_engine->addImportPath(path);
}

void QmlEngineUtils::setImportPaths(const QStringList &paths) const
{
    m_engine->setImportPathList(paths);
}
