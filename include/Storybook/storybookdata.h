#pragma once

#include <QObject>
#include <QAbstractItemModel>

class StorybookData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QAbstractItemModel* pagesModel READ pagesModel CONSTANT)
    Q_PROPERTY(QString localPagesPath READ localPagesPath CONSTANT)

public:
    explicit StorybookData(const QAbstractItemModel* pagesModel,
                           const QString& localPagesPath,
                           QObject *parent = nullptr);

    const QAbstractItemModel* pagesModel() const;
    const QString& localPagesPath() const;

private:
    const QAbstractItemModel* m_pagesModel;
    QString m_localPagesPath;
};
