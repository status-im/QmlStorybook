#include "Storybook/storybookdata.h"


StorybookData::StorybookData(const QAbstractItemModel *pagesModel, QObject *parent)
    : QObject(parent), m_pagesModel(pagesModel)
{

}

const QAbstractItemModel* StorybookData::pagesModel() const
{
    return m_pagesModel;
}
