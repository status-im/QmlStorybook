#include "Storybook/remotechangesnotifier.h"

#include <QRestAccessManager>
#include <QRestReply>
#include <QTimer>

RemoteChangesNotifier::RemoteChangesNotifier(const QUrl& queryUrl, QObject *parent)
    : AbstractChangesNotifier{parent}, m_queryUrl(queryUrl),
    m_rest{new QRestAccessManager{new QNetworkAccessManager{this}, this}}
{
    query();
}

void RemoteChangesNotifier::query()
{
    QNetworkRequest request(m_queryUrl);

    m_rest->get(request, this, [this](QRestReply& reply) {
        if (reply.isSuccess()) {
            bool ok = false;
            quint64 version = reply.readText().toULongLong(&ok);

            if (ok && version != m_version) {
                m_version = version;
                emit changed();
            }
        }

        reply.networkReply()->deleteLater();

        QTimer::singleShot(s_updateIntervalMs, this,
                           &RemoteChangesNotifier::query);
    });
}
