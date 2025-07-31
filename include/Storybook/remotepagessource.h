#pragma once

#include "Storybook/abstractpagessource.h"

#include <QDateTime>
#include <QStringList>
#include <QUrl>

#include <map>
#include <vector>

class QRestAccessManager;
class QNetworkReply;

class RemotePagesSource : public AbstractPagesSource
{
    Q_OBJECT

public:
    explicit RemotePagesSource(const QUrl& queryUrl,
                               const QUrl& pageAccessBaseUrl,
                               QObject *parent = nullptr);

    // AbstractPagesSource interface
    QStringList pages() const override;
    QString page(const QString &page) const override;

private:
    QUrl pageUrl(const QString& page);
    void initUpdate();
    void finalizeUpdate(bool success);
    void scheduleUpdate();

    QUrl m_queryUrl;
    QUrl m_pagesAccessBaseUrl;
    QRestAccessManager* m_rest = nullptr;

    std::map<QString, QDateTime> m_pages;
    std::map<QString, QString> m_pagesContent;
    std::map<QString, QDateTime> m_pagesNew;
    std::map<QString, QString> m_pagesContentNew;

    QStringList m_added;
    QStringList m_changed;
    QStringList m_removed;

    std::vector<QNetworkReply*> m_replies;
    bool m_aborted = false;
    qsizetype m_requestsCounter = 0;

    static constexpr int s_updateIntervalMs = 2000;

    static QString extractPageName(const QString& path);
    static QStringList extractPageNames(const QStringList& paths);
};
