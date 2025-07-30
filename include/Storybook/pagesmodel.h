#pragma once

#include <QAbstractListModel>
#include <QDateTime>

#include "figmalinksmodel.h"
#include "pagesmodelenums.h"

class AbstractPagesSource;

struct PagesModelItem {
    QDateTime lastModified;
    QString title;
    QString category;
    int status = 0;
    QStringList figmaLinks;
};

class PagesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PagesModel(const AbstractPagesSource* source, QObject *parent = nullptr);

    enum Roles {
        TitleRole = Qt::UserRole + 1,
        CategoryRole,
        StatusRole,
        FigmaRole
    };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    void onPagesChanged(const QStringList& added, const QStringList& removed,
                        const QStringList& changed);

    int getIndex(const QString& page) const;

    PagesModelItem readMetadata(const QString& page);
    QList<PagesModelItem> readMetadata(const QStringList& pages);

    static QString extractCategory(const QByteArray& content);
    static PagesModelEnums::Status extractStatus(const QByteArray& content);
    static QStringList extractFigmaLinks(const QByteArray& content);

    void setFigmaLinks(const QString& title, const QStringList& links);

    QList<PagesModelItem> m_items;
    QMap<QString, FigmaLinksModel*> m_figmaSubmodels;
    const AbstractPagesSource* m_source = nullptr;
};
