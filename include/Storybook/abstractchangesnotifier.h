#pragma once

#include <QObject>

class AbstractChangesNotifier : public QObject
{
    Q_OBJECT
public:
    explicit AbstractChangesNotifier(QObject *parent = nullptr)
        : QObject(parent) {}

signals:
    void changed();
};
