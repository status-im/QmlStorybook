#include "Storybook/storybookdata.h"


StorybookData::StorybookData(const QAbstractItemModel *pagesModel,
                             const QString& localPagesPath, Mode mode, QObject *parent)
    : QObject(parent), m_pagesModel(pagesModel),
    m_localPagesPath(localPagesPath), m_mode(mode)
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

StorybookData::Mode StorybookData::mode() const
{
    return m_mode;
}
