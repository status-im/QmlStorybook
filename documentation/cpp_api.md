# C++ API Documentation

This document provides an overview of the C++ APIs in QmlStorybook.

---

## `AbstractChangesNotifier`

**Header:** `Storybook/abstractchangesnotifier.h`

An abstract base class for notifiers that signal changes.

### Signals

-   `changed()`: Emitted when a change is detected.

---

## `AbstractPagesSource`

**Header:** `Storybook/abstractpagessource.h`

An abstract base class for sources of pages.

### Public Functions

-   `virtual QStringList pages() const = 0`: Returns a list of all pages.
-   `virtual QString page(const QString& page) const = 0`: Returns the content of a specific page.

### Signals

-   `pagesChanged(const QStringList& added, const QStringList& removed, const QStringList& changed)`: Emitted when the pages change.

---

## `DirectoryFilesWatcher`

**Header:** `Storybook/directoryfileswatcher.h`

Watches a directory for file changes.

### Public Functions

-   `explicit DirectoryFilesWatcher(const QString &path, const QString &pattern, QObject *parent = nullptr)`: Constructor.
-   `QStringList files() const`: Returns a list of files being watched.

### Signals

-   `filesChanged(const QStringList& added, const QStringList& removed, const QStringList& changed)`: Emitted when the files in the directory change.

---

## `FigmaIO`

**Header:** `Storybook/figmaio.h`

Provides functionality for reading and writing Figma links to a file.

### Static Public Members

-   `static QMap<QString, QStringList> read(const QString &file)`: Reads Figma links from a file.
-   `static void write(const QString &file, const QMap<QString, QStringList> &map)`: Writes Figma links to a file.

---

## `FigmaLinks`

**Header:** `Storybook/figmalinks.h`

Manages Figma links.

### Public Functions

-   `explicit FigmaLinks(const QMap<QString, QStringList>& mapping, QObject *parent = nullptr)`: Constructor.
-   `const QMap<QString, QStringList>& getLinksMap() const`: Returns the map of Figma links.

---

## `FigmaLinksModel`

**Header:** `Storybook/figmalinksmodel.h`

A list model for Figma links.

### Properties

-   `count`: The number of links in the model.

### Public Functions

-   `explicit FigmaLinksModel(const QStringList &links, QObject *parent = nullptr)`: Constructor.
-   `void setContent(const QStringList &links)`: Sets the content of the model.

### Signals

-   `countChanged()`: Emitted when the number of links changes.

---

## `LocalChangesNotifier`

**Header:** `Storybook/localchangesnotifier.h`

Notifies about local file changes.

### Public Functions

-   `explicit LocalChangesNotifier(QObject *parent = nullptr)`: Constructor.
-   `void addPaths(const QStringList &paths)`: Adds paths to be watched for changes.

---

## `LocalPagesSource`

**Header:** `Storybook/localpagessource.h`

A page source that reads pages from the local file system.

### Public Functions

-   `explicit LocalPagesSource(const QString &path, QObject *parent = nullptr)`: Constructor.
-   `QStringList pages() const override`: Returns a list of all pages.
-   `QString page(const QString &page) const override`: Returns the content of a specific page.

---

## `ModelUtils`

**Header:** `Storybook/modelutils.h`

A utility class for working with `QAbstractItemModel`.

### Static Public Members

-   `static std::optional<int> findRole(const QByteArray &role, const QAbstractItemModel *model)`: Finds the role value for a given role name.

---

## `PagesModel`

**Header:** `Storybook/pagesmodel.h`

A list model for pages.

### Public Functions

-   `explicit PagesModel(const AbstractPagesSource* source, QObject *parent = nullptr)`: Constructor.

### Enums

-   `Roles`: Defines the roles for the model data.
    -   `TitleRole`
    -   `CategoryRole`
    -   `StatusRole`
    -   `FigmaRole`

---

## `PagesModelEnums`

**Header:** `Storybook/pagesmodelenums.h`

Defines enums used by `PagesModel`.

### Enums

-   `Status`: Represents the status of a page.
    -   `Uncategorized`
    -   `Bad`
    -   `Decent`
    -   `Good`

---

## `QmlEngineUtils`

**Header:** `Storybook/qmlengineutils.h`

A utility class for interacting with the QML engine.

### Public Functions

-   `explicit QmlEngineUtils(QQmlEngine* engine)`: Constructor.
-   `Q_INVOKABLE void clearComponentCache() const`: Clears the QML component cache.
-   `Q_INVOKABLE QStringList importPaths() const`: Returns the QML import paths.
-   `Q_INVOKABLE bool removeImportPath(const QString& path) const`: Removes a QML import path.
-   `Q_INVOKABLE void addImportPath(const QString& path) const`: Adds a QML import path.
-   `Q_INVOKABLE void setImportPaths(const QStringList& paths) const`: Sets the QML import paths.

---

## `QmlFilesServer`

**Header:** `Storybook/qmlfilesserver.h`

An HTTP server for serving QML files.

### Public Functions

-   `explicit QmlFilesServer(QStringList basePaths, QString pagesPath, bool amendQmldirs = true, QObject *parent = nullptr)`: Constructor.
-   `void setFileSelectors(const QStringList& selectors)`: Sets the file selectors.
-   `const QStringList& fileSelectors() const`: Returns the file selectors.
-   `bool start(quint16 port)`: Starts the server on the specified port.

---

## `RemoteChangesNotifier`

**Header:** `Storybook/remotechangesnotifier.h`

Notifies about remote changes.

### Public Functions

-   `explicit RemoteChangesNotifier(const QUrl& queryUrl, QObject *parent = nullptr)`: Constructor.

---

## `RemotePagesSource`

**Header:** `Storybook/remotepagessource.h`

A page source that reads pages from a remote server.

### Public Functions

-   `explicit RemotePagesSource(const QUrl& queryUrl, const QUrl& pageAccessBaseUrl, QObject *parent = nullptr)`: Constructor.
-   `QStringList pages() const override`: Returns a list of all pages.
-   `QString page(const QString &page) const override`: Returns the content of a specific page.

---

## `SectionsDecoratorModel`

**Header:** `Storybook/sectionsdecoratormodel.h`

A decorator model that adds sections to a source model.

### Properties

-   `sourceModel`: The source model.

### Public Functions

-   `explicit SectionsDecoratorModel(QObject *parent = nullptr)`: Constructor.
-   `void setSourceModel(QAbstractItemModel *sourceModel)`: Sets the source model.
-   `QAbstractItemModel *sourceModel() const`: Returns the source model.
-   `Q_INVOKABLE void flipFolding(int index)`: Flips the folded state of a section.

### Signals

-   `sourceModelChanged()`: Emitted when the source model changes.

---

## `StorybookData`

**Header:** `Storybook/storybookdata.h`

A data object for the Storybook.

### Properties

-   `pagesModel`: The pages model.
-   `localPagesPath`: The path to the local pages.
-   `mode`: The mode of the Storybook (Local or Remote).

### Public Functions

-   `explicit StorybookData(const QAbstractItemModel* pagesModel, const QString& localPagesPath, Mode mode, QObject *parent = nullptr)`: Constructor.
-   `const QAbstractItemModel* pagesModel() const`: Returns the pages model.
-   `const QString& localPagesPath() const`: Returns the path to the local pages.
-   `Mode mode() const`: Returns the mode of the Storybook.

---

## `StorybookSetup`

**Header:** `Storybook/storybooksetup.h`

A utility class for setting up the Storybook.

### Static Public Members

-   `static void registerTypesLocal(const QStringList& watchedPaths, const QString& pagesPath, const QString& testExecutable, const QString& testsPath)`: Registers types for local mode.
-   `static void registerTypesRemote(const QUrl& versionUrl, const QUrl& queryUrl, const QUrl& pageAccessBaseUrl)`: Registers types for remote mode.
-   `static void configureEngine(QQmlEngine* engine, bool local)`: Configures the QML engine.

---

## `SystemUtils`

**Header:** `Storybook/systemutils.h`

A utility class for system-related functions.

### Public Functions

-   `explicit SystemUtils(QObject *parent = nullptr)`: Constructor.
-   `Q_INVOKABLE QString getEnvVar(const QString &varName)`: Gets the value of an environment variable.

---

## `TestsRunner`

**Header:** `Storybook/testsrunner.h`

A class for running tests.

### Public Functions

-   `explicit TestsRunner(const QString& testRunnerExecutablePath, const QString& testsPath, QObject *parent = nullptr)`: Constructor.
-   `Q_INVOKABLE int testsCount(const QString& path)`: Returns the number of tests for a given path.
-   `Q_INVOKABLE QObject* runTests(const QString& path)`: Runs the tests for a given path.
-   `Q_INVOKABLE QString testsPath() const`: Returns the path to the tests.
