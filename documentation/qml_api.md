# QML API Documentation

This document provides an overview of the QML APIs in QmlStorybook.

---

## `CheckBoxFlowSelector.qml`

A flow of checkboxes that allows for multiple selections.

### Properties

-   `model`: The model for the checkboxes.
-   `selection`: The currently selected items.
-   `initialSelection`: Whether the checkboxes are initially selected.
-   `exclusive`: Whether the selection is exclusive.

---

## `CompilationErrorsBox.qml`

A box that displays QML compilation errors.

### Properties

-   `errors`: The QML errors to display.

---

## `FigmaImagesProxyModel.qml`

A proxy model that resolves Figma image links.

### Properties

-   `figmaLinksCache`: The Figma links cache.
-   `sourceModel`: The source model.

---

## `FigmaLinksCache.qml`

A cache for Figma image links.

### Properties

-   `figmaToken`: The Figma API token.

### Functions

-   `getImageUrl(figmaLink, cb)`: Gets the image URL for a Figma link.

---

## `FigmaPreviewWindow.qml`

A window for previewing Figma designs.

### Properties

-   `model`: The model for the Figma images.

### Signals

-   `removeLinksRequested(var indexes)`: Emitted when links are requested to be removed.
-   `appendLinksRequested(var links)`: Emitted when links are requested to be appended.

---

## `FigmaUtils.qml`

A utility object for working with Figma.

### Functions

-   `decomposeLink(link)`: Decomposes a Figma link into its file and node ID.
-   `getLinks(token, file, nodeIds, cb)`: Gets the image links for a set of Figma nodes.

---

## `FilteredPagesList.qml`

A list of pages with filtering and section support.

### Properties

-   `model`: The source model for the pages.
-   `currentPage`: The currently selected page.
-   `localPagesPath`: The path to the local pages.

### Signals

-   `pageSelected(string page)`: Emitted when a page is selected.
-   `statusClicked`: Emitted when the status is clicked.

---

## `FlickableImage.qml`

A flickable image with a busy indicator.

### Properties

-   `source`: The source of the image.

---

## `GenericListView.qml`

A generic list view with support for movable items, custom roles, and more.

### Properties

-   `movable`: Whether the items in the list are movable.
-   `roles`: The roles to be visualized.
-   `delegateHeight`: The height of the delegate.
-   `skipEmptyRoles`: Whether to skip empty roles.
-   `label`: The label for the list view.
-   `insetComponent`: An additional component to be instantiated within every delegate.
-   `margin`: The margin of the list view.

### Signals

-   `moveRequested(int from, int to)`: Emitted when an item is moved.
-   `rowClicked(int index, string role)`: Emitted when a row is clicked.

---

## `HotReloaderControls.qml`

Controls for the hot reloader.

### Properties

-   `autoReloadEnabled`: Whether auto-reloading is enabled.

### Signals

-   `forceReloadClicked`: Emitted when the "Reload now" button is clicked.

### Functions

-   `notifyReload()`: Notifies the controls that a reload has occurred.

---

## `ImageSelectPopup.qml`

A popup for selecting an image.

### Properties

-   `model`: The model for the images.

### Signals

-   `selected(string icon)`: Emitted when an image is selected.

---

## `ImagesGridView.qml`

A grid view for displaying images.

### Signals

-   `clicked(int index)`: Emitted when an image is clicked.

---

## `ImagesNavigationLayout.qml`

A layout for navigating between images.

### Properties

-   `count`: The number of images.
-   `currentIndex`: The index of the current image.

### Signals

-   `moveUp`: Emitted when the up button is clicked.
-   `moveLeft`: Emitted when the left button is clicked.
-   `moveRight`: Emitted when the right button is clicked.

---

## `InspectionItem.qml`

An item for inspecting visual properties.

### Properties

-   `name`: The name of the item.
-   `objName`: The object name of the item.
-   `isVisual`: Whether the item is visual.
-   `showNonVisual`: Whether to show non-visual items.
-   `forceSelect`: Whether to force selection of the item.
-   `visualParent`: The visual parent of the item.
-   `visualRoot`: The visual root of the item.

### Signals

-   `clicked`: Emitted when the item is clicked.

---

## `InspectionItemsList.qml`

A list of inspection items.

---

## `InspectionPanel.qml`

A panel for inspecting items.

### Properties

-   `propagateClipping`: Whether to propagate clipping.
-   `showNonVisualItems`: Whether to show non-visual items.
-   `showScreenshot`: Whether to show a screenshot of the source item.
-   `sourceItem`: The source item to inspect.

### Signals

-   `clicked(int index)`: Emitted when an item is clicked.

---

## `InspectionUtils.qml`

A utility object for inspection.

### Functions

-   `isVisual(item)`: Returns whether an item is visual.
-   `baseName(item)`: Returns the base name of an item.
-   `baseTypeName(item)`: Returns the base type name of an item.
-   `trimQQuickPrefix(name)`: Trims the "QQuick" prefix from a name.
-   `simpleName(item)`: Returns a simple name for an item.
-   `findItemsByTypeName(root, typeName)`: Finds items by type name.
-   `findVisualsByTypeName(root, typeName, onlyItems = false)`: Finds visual items by type name.

---

## `InspectionWindow.qml`

A window for inspecting items.

### Functions

-   `inspect(sourceItem)`: Inspects a source item.

---

## `LimitProxyModel.qml`

A proxy model that limits the number of items.

### Properties

-   `limit`: The maximum number of items.

---

## `Logs.qml`

An object for logging events.

### Properties

-   `logText`: The text of the log.

### Functions

-   `logEvent(name, argumentNames, args)`: Logs an event.

---

## `LogsAndControlsPanel.qml`

A panel with tabs for logs and controls.

### Properties

-   `logsView`: The logs view.
-   `controls`: The controls for the panel.

---

## `LogsView.qml`

A view for displaying logs.

### Properties

-   `logText`: The text of the log.

---

## `NoFigmaTokenDialog.qml`

A dialog that informs the user that the Figma token is not set.

---

## `NothingToInspectDialog.qml`

A dialog that informs the user that there is nothing to inspect.

### Properties

-   `pageName`: The name of the page.

---

## `PageToolBar.qml`

A toolbar for a page.

### Properties

-   `componentName`: The name of the component.
-   `figmaPagesCount`: The number of Figma pages.
-   `mobile`: Whether the toolbar is in mobile mode.
-   `testRunnerController`: The test runner controller.

### Signals

-   `figmaPreviewClicked`: Emitted when the Figma preview button is clicked.
-   `inspectClicked`: Emitted when the inspect button is clicked.
-   `backClicked`: Emitted when the back button is clicked.

---

## `PagesList.qml`

A list of pages.

### Properties

-   `currentPage`: The currently selected page.
-   `localPagesPath`: The path to the local pages.

### Signals

-   `pageSelected(string page)`: Emitted when a page is selected.
-   `sectionClicked(int index)`: Emitted when a section is clicked.
-   `statusClicked`: Emitted when the status is clicked.

---

## `PopupBackground.qml`

A background for popups.

---

## `RadioButtonFlowSelector.qml`

A flow of radio buttons that allows for a single selection.

### Properties

-   `model`: The model for the radio buttons.
-   `selection`: The currently selected item.
-   `initialSelection`: The initially selected item.

---

## `SettingsLayout.qml`

A layout for settings.

### Properties

-   `loadAsynchronously`: Whether to load pages asynchronously.
-   `runTestsAutomatically`: Whether to run tests automatically.
-   `figmaToken`: The Figma API token.
-   `showFigmaSettings`: Whether to show the Figma settings.
-   `showTestsSettings`: Whether to show the tests settings.

---

## `SingleItemProxyModel.qml`

A proxy model that filters for a single item.

### Properties

-   `roleName`: The name of the role to filter by.
-   `value`: The value to filter for.

---

## `SourceCodeBox.qml`

A box that displays source code.

### Properties

-   `sourceCode`: The source code to display.
-   `hasErrors`: Whether the source code has errors.

---

## `StatusStatisticsDialog.qml`

A dialog that displays statistics about the status of pages.

### Properties

-   `pagesModel`: The pages model.

---

## `Storybook.qml`

The main component of the Storybook.

### Properties

-   `currentPage`: The currently selected page.
-   `darkMode`: Whether dark mode is enabled.
-   `mobile`: Whether the application is running on a mobile platform.

---

## `StorybookUtils.qml`

A utility object for the Storybook.

### Functions

-   `formatQmlCode(code)`: Formats QML code.
-   `findChild(parent, name)`: Finds a child of an item by its object name.

---

## `TestRunnerController.qml`

A controller for the test runner.

### Properties

-   `running`: Whether tests are running.
-   `aborted`: Whether the tests were aborted.

### Signals

-   `started`: Emitted when the tests are started.
-   `finished(int failedTests, bool aborted, bool crashed)`: Emitted when the tests are finished.

### Functions

-   `getTestsCount(testFileName)`: Gets the number of tests in a file.
-   `getTestsPath()`: Gets the path to the tests.
-   `runTests(testFileName)`: Runs the tests in a file.
-   `abort()`: Aborts the tests.

---

## `TestRunnerControls.qml`

Controls for the test runner.

### Properties

-   `mode`: The mode of the controls.
-   `numberOfFailedTests`: The number of failed tests.

### Signals

-   `runClicked`: Emitted when the run button is clicked.
-   `abortClicked`: Emitted when the abort button is clicked.

---

## `TestUtils.qml`

A utility object for tests.

### Functions

-   `findTextItem(root, text)`: Finds a text item by its text.
-   `findByType(root, type)`: Finds an item by its type.
