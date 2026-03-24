# Testing Documentation

This document provides an overview of how to write and run tests in QmlStorybook.

## Writing Tests

Tests in QmlStorybook are written using the Qt Test framework. You can create a new test by creating a new QML file in the `tests` directory. The test file should have the same name as the component it is testing, with the `tst_` prefix. For example, a test for `MyComponent.qml` should be named `tst_MyComponent.qml`.

Here is an example of a simple test:

```qml
import QtQuick
import QtTest

TestCase {
    name: "MyComponentTests"

    function test_myFunction() {
        // Your test code here
    }
}
```

## Running Tests

You can run tests in QmlStorybook in two ways:

1.  **From the Storybook UI**: Select a page in the Storybook and click the "Run tests" button in the toolbar. This will run the tests for the selected page.
2.  **From the command line**: You can run all tests by running the test executable from the build directory.

## Test Runner

The test runner is responsible for running the tests and reporting the results. The test runner is implemented in the `TestsRunner` class. The `TestRunnerController` QML component is used to control the test runner from the Storybook UI.

## Test Utilities

The `TestUtils` QML singleton provides utility functions for writing tests. These functions include:

-   `findTextItem(root, text)`: Finds a text item by its text.
-   `findByType(root, type)`: Finds an item by its type.
