#pragma once

#include <QObject>

class QHttpServer;

class QmlFilesServer : public QObject
{
    Q_OBJECT
public:
    explicit QmlFilesServer(QStringList basePaths, QString pagesPath,
                            QObject *parent = nullptr);

    void start(quint16 port);

private:
    static bool isRelativePathSafe(const QString& relativePath);
    QString findFirstExistingFile(const QString& relativePath) const;

    quint64 m_version = 0;
    QHttpServer* m_server = nullptr;
    QStringList m_basePaths;
    QString m_pagesPath;
};
