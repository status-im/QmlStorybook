#pragma once

#include <QString>
#include <QStringList>

class QQmlEngine;

class StorybookSetup
{
public:
    static void registerTypes(const QStringList& watchedPaths,
                              const QString& pagesPath,
                              const QString& testExecutable,
                              const QString& testsPath);

    static void configureEngine(QQmlEngine* engine, const QString& pagesLocalPath);
};
