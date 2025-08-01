#include "Storybook/storybooksetup.h"

#include "Storybook/figmalinks.h"
#include "Storybook/localchangesnotifier.h"
#include "Storybook/localpagessource.h"
#include "Storybook/pagesmodel.h"
#include "Storybook/pagesmodelenums.h"
#include "Storybook/qmlengineutils.h"
#include "Storybook/sectionsdecoratormodel.h"
#include "Storybook/storybookdata.h"
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
    auto storybookDataFactory = [pagesPath](QQmlEngine*, QJSEngine*) {
        auto source = new LocalPagesSource(pagesPath);
        auto pagesModel = new PagesModel(source);
        auto storybookData = new StorybookData(pagesModel, pagesPath, StorybookData::Local);
        source->setParent(storybookData);
        pagesModel->setParent(storybookData);

        return storybookData;
    };

    qmlRegisterSingletonType<StorybookData>(
        "Storybook", 1, 0, "StorybookData", storybookDataFactory);

    qmlRegisterUncreatableMetaObject(PagesModelEnums::staticMetaObject,
                                     "Storybook", 1, 0, "PagesModelEnums",
                                     "PagesModel enums");

    qmlRegisterType<SectionsDecoratorModel>("Storybook", 1, 0, "SectionsDecoratorModel");
    qmlRegisterUncreatableType<FigmaLinks>("Storybook", 1, 0, "FigmaLinks", {});

    auto notifierFactory = [watchedPaths](QQmlEngine*, QJSEngine*) {
        auto notifier = new LocalChangesNotifier();
        notifier->addPaths(watchedPaths);

        return notifier;
    };

    qmlRegisterSingletonType<AbstractChangesNotifier>(
        "Storybook", 1, 0, "ChangesNotifier", notifierFactory);


    auto runnerFactory = [testExecutable, testsPath](QQmlEngine*, QJSEngine*) {
        return new TestsRunner(testExecutable, testsPath);
    };

    qmlRegisterSingletonType<TestsRunner>(
        "Storybook", 1, 0, "TestsRunner", runnerFactory);

    auto qmlEngineUtilsFactory = [](QQmlEngine* engine, QJSEngine*) {
        return new QmlEngineUtils(engine);
    };

    qmlRegisterSingletonType<QmlEngineUtils>(
        "Storybook", 1, 0, "QmlEngineUtils", qmlEngineUtilsFactory);
}

void StorybookSetup::configureEngine(QQmlEngine* engine)
{
    static HotReloadUrlInterceptor interceptor;

    engine->addUrlInterceptor(&interceptor);
}
