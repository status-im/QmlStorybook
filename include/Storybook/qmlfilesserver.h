#pragma once

#include <QObject>

class QHttpServer;

class QmlFilesServer : public QObject
{
    Q_OBJECT
public:
    explicit QmlFilesServer(QStringList basePaths, QString pagesPath,
                            bool amendQmldirs = true, QObject *parent = nullptr);

    void start(quint16 port);

private:
    QString findFirstExistingSourceDir(const QString &relativePath) const;
    QString findFirstExistingFile(const QString& relativePath) const;

    quint64 m_version = 0;
    QHttpServer* m_server = nullptr;
    QStringList m_basePaths;
    QString m_pagesPath;
    bool m_amendQmldirs = true;
};
