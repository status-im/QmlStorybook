#pragma once

#include <QObject>
#include <QAbstractItemModel>

class StorybookData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QAbstractItemModel* pagesModel READ pagesModel CONSTANT)

public:
    explicit StorybookData(const QAbstractItemModel* pagesModel, QObject *parent = nullptr);

    const QAbstractItemModel* pagesModel() const;

private:
    const QAbstractItemModel* m_pagesModel;
};
