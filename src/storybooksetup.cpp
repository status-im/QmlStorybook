#include "Storybook/storybooksetup.h"

#include "Storybook/cachecleaner.h"
#include "Storybook/directorieswatcher.h"
#include "Storybook/figmalinks.h"
#include "Storybook/pagesmodel.h"
#include "Storybook/pagesmodelenums.h"
#include "Storybook/sectionsdecoratormodel.h"
#include "Storybook/testsrunner.h"

#include <QQmlAbstractUrlInterceptor>
#include <QQmlContext>
#include <QQmlEngine>

#include <qqml.h>

class HotReloadUrlInterceptor : public QQmlAbstractUrlInterceptor {
    unsigned int counter = 0;

    QUrl intercept(const QUrl &url, QQmlAbstractUrlInterceptor::DataType type) override {
        if (type != QQmlAbstractUrlInterceptor::QmlFile)
            return url;

        return url.toString() + "#" + QString::number(counter++);
    }
};

void StorybookSetup::registerTypes(const QStringList &watchedPaths,
                                   const QString &pagesPath,
                                   const QString &testExecutable,
                                   const QString &testsPath)
{
    static QString _pagesPath = pagesPath;
    Q_UNUSED(_pagesPath); // silence clazy

    qmlRegisterUncreatableMetaObject(PagesModelEnums::staticMetaObject,
                                     "Storybook", 1, 0, "PagesModelEnums",
                                     "PagesModel enums");

    struct PagesModelInitialized : public PagesModel {
        explicit PagesModelInitialized(QObject *parent = nullptr)
            : PagesModel(_pagesPath, parent) {
        }
    };

    qmlRegisterType<PagesModelInitialized>("Storybook", 1, 0, "PagesModel");
    qmlRegisterType<SectionsDecoratorModel>("Storybook", 1, 0, "SectionsDecoratorModel");
    qmlRegisterUncreatableType<FigmaLinks>("Storybook", 1, 0, "FigmaLinks", {});

    auto watcherFactory = [watchedPaths](QQmlEngine*, QJSEngine*) {
        auto watcher = new DirectoriesWatcher();

        watcher->addPaths(watchedPaths);

        return watcher;
    };

    qmlRegisterSingletonType<DirectoriesWatcher>(
        "Storybook", 1, 0, "SourceWatcher", watcherFactory);


    auto runnerFactory = [testExecutable, testsPath](QQmlEngine*, QJSEngine*) {
        return new TestsRunner(testExecutable, testsPath);
    };

    qmlRegisterSingletonType<TestsRunner>(
        "Storybook", 1, 0, "TestsRunner", runnerFactory);

    auto cleanerFactory = [](QQmlEngine* engine, QJSEngine*) {
        return new CacheCleaner(engine);
    };

    qmlRegisterSingletonType<CacheCleaner>(
        "Storybook", 1, 0, "CacheCleaner", cleanerFactory);
}

void StorybookSetup::configureEngine(QQmlEngine* engine,
                                     const QString& pagesPath)
{
    static HotReloadUrlInterceptor interceptor;

    engine->addUrlInterceptor(&interceptor);
    engine->rootContext()->setContextProperty(QStringLiteral("pagesFolder"), pagesPath);
}
