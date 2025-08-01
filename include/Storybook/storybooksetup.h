#pragma once

#include <QString>
#include <QStringList>
#include <QUrl>

class QQmlEngine;

class StorybookSetup
{
public:
    static void registerTypesLocal(const QStringList& watchedPaths,
                                   const QString& pagesPath,
                                   const QString& testExecutable,
                                   const QString& testsPath);

    static void registerTypesRemote(const QUrl& versionUrl,
                                    const QUrl& queryUrl,
                                    const QUrl& pageAccessBaseUrl);

    static void configureEngine(QQmlEngine* engine, bool local);
};
