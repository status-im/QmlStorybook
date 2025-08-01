#pragma once

#include <QObject>
#include <QAbstractItemModel>

class StorybookData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QAbstractItemModel* pagesModel READ pagesModel CONSTANT)
    Q_PROPERTY(QString localPagesPath READ localPagesPath CONSTANT)
    Q_PROPERTY(StorybookData::Mode mode READ mode CONSTANT)

public:
    enum Mode {
        Local,
        Remote
    };
    Q_ENUM(Mode)

    explicit StorybookData(const QAbstractItemModel* pagesModel,
                           const QString& localPagesPath,
                           Mode mode, QObject *parent = nullptr);

    const QAbstractItemModel* pagesModel() const;
    const QString& localPagesPath() const;
    Mode mode() const;

private:
    const QAbstractItemModel* m_pagesModel;
    QString m_localPagesPath;
    Mode m_mode = Local;
};
