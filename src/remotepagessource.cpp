#include "Storybook/remotepagessource.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRestAccessManager>
#include <QRestReply>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;

RemotePagesSource::RemotePagesSource(const QUrl& queryUrl,
                                     const QUrl& pageAccessBaseUrl,
                                     QObject *parent)
    : AbstractPagesSource{parent}, m_queryUrl(queryUrl),
    m_pagesAccessBaseUrl(pageAccessBaseUrl), m_rest(
          new QRestAccessManager(new QNetworkAccessManager(this), this))
{
    initUpdate();
}

QStringList RemotePagesSource::pages() const
{
    QStringList pages;
    for (auto& [page, _] : m_pages)
        pages << page;

    return extractPageNames(pages);
}

QString RemotePagesSource::page(const QString &page) const
{
    QString key = page + u"Page.qml"_s;
    auto it = m_pagesContent.find(key);

    if (it == m_pagesContent.cend())
        return {};

    return it->second;
}

QUrl RemotePagesSource::pageUrl(const QString &page)
{
    QUrl url = m_pagesAccessBaseUrl;
    QString path = url.path();
    if (!path.endsWith('/'))
        path += '/';

    url.setPath(path + page);
    return url;
}

void RemotePagesSource::initUpdate()
{
    m_pagesNew.clear();
    m_pagesContentNew.clear();
    m_added.clear();
    m_changed.clear();
    m_removed.clear();
    m_replies.clear();
    m_requestsCounter = 0;
    m_aborted = false;

    QNetworkRequest request(m_queryUrl);
    m_rest->get(request, this, [this](QRestReply& reply) {
        QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> guard(
            reply.networkReply());

        if (!reply.isSuccess()) {
            scheduleUpdate();
            return;
        }

        const auto jsonDoc = reply.readJson();

        if (!jsonDoc) {
            qWarning() << "Failed to read JSON: null document returned.";
            scheduleUpdate();
            return;
        }

        const QJsonArray array = jsonDoc->array();

        for (auto &value : array) {
            if (!value.isObject())
                continue;

            QJsonObject obj = value.toObject();
            m_pagesNew[obj.value(u"page"_s).toString()] =
                QDateTime::fromMSecsSinceEpoch(
                    obj.value(u"timestamp"_s).toInteger());
        }

        for (auto& [page, timestamp] : std::as_const(m_pagesNew)) {
            auto it = m_pages.find(page);

            if (it == m_pages.end())
                m_added << page;
            else if (it->second != timestamp)
                m_changed << page;
            else
                m_pagesContentNew[page] = m_pagesContent[page];
        }

        for (auto& [page, _] : std::as_const(m_pages))
            if (m_pagesNew.find(page) == m_pagesNew.end())
                m_removed << page;

        QStringList newOrChanged = m_added + m_changed;

        if (newOrChanged.empty()) {
            if (m_removed.empty())
                scheduleUpdate();
            else
                finalizeUpdate(true);

            return;
        }

        for (auto& page : std::as_const(newOrChanged))
        {
            QNetworkRequest request(pageUrl(page));
            auto total = newOrChanged.size();

            auto reply = m_rest->get(request, this, [this, page, total](QRestReply& reply) {

                if (!reply.isSuccess() || m_aborted) {
                    if (!m_aborted) {
                        m_aborted = true;
                        finalizeUpdate(false);
                    }

                    return;
                }

                m_pagesContentNew[page] = reply.readText();
                m_requestsCounter++;

                if (m_requestsCounter == total)
                    finalizeUpdate(true);
            });

            m_replies.push_back(reply);
        }
    });
}

void RemotePagesSource::finalizeUpdate(bool success)
{
    for (auto reply : m_replies) {
        reply->abort();
        reply->deleteLater();
    }

    scheduleUpdate();

    if (!success)
        return;

    m_pages = std::move(m_pagesNew);
    m_pagesContent = std::move(m_pagesContentNew);

    emit pagesChanged(extractPageNames(m_added),
                      extractPageNames(m_removed),
                      extractPageNames(m_changed));
}

void RemotePagesSource::scheduleUpdate()
{
    QTimer::singleShot(s_updateIntervalMs, this, &RemotePagesSource::initUpdate);
}

QString RemotePagesSource::extractPageName(const QString &path)
{
    return path.chopped((u"Page.qml"_s).size());
}

QStringList RemotePagesSource::extractPageNames(const QStringList &paths)
{
    QStringList pages;
    pages.reserve(paths.size());

    std::transform(paths.begin(), paths.end(), std::back_inserter(pages),
                   [](auto path) { return extractPageName(path); });

    return pages;
}
