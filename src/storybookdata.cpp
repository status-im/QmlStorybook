#include "Storybook/storybookdata.h"


StorybookData::StorybookData(const QAbstractItemModel *pagesModel,
                             const QString& localPagesPath, QObject *parent)
    : QObject(parent), m_pagesModel(pagesModel), m_localPagesPath(localPagesPath)
{

}

const QAbstractItemModel* StorybookData::pagesModel() const
{
    return m_pagesModel;
}

const QString &StorybookData::localPagesPath() const
{
    return m_localPagesPath;
}
