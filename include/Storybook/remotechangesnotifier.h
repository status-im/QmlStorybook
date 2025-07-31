#pragma once

#include "Storybook/abstractchangesnotifier.h"

#include <QUrl>

class QRestAccessManager;

class RemoteChangesNotifier : public AbstractChangesNotifier
{
    Q_OBJECT
public:
    explicit RemoteChangesNotifier(const QUrl& queryUrl,
                                   QObject *parent = nullptr);

private:
    void query();

    QUrl m_queryUrl;
    QRestAccessManager* m_rest = nullptr;
    quint64 m_version = 0;

    static constexpr int s_updateIntervalMs = 500;
};
