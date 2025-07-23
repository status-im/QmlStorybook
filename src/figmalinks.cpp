#include "Storybook/figmalinks.h"

FigmaLinks::FigmaLinks(const QMap<QString, QStringList>& linksMap, QObject *parent)
    : QObject{parent}, m_linksMap{linksMap}
{
}

const QMap<QString, QStringList>& FigmaLinks::getLinksMap() const
{
    return m_linksMap;
}
