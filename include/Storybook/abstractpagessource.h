#pragma once

#include <QObject>

class AbstractPagesSource : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPagesSource(QObject *parent = nullptr)
        : QObject(parent) {}

    virtual QStringList pages() const = 0;
    virtual QString page(const QString& page) const = 0;

signals:
    void pagesChanged(const QStringList& added, const QStringList& removed,
                      const QStringList& changed);
};
