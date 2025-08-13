# Advanced Topics

This document covers advanced topics related to using QmlStorybook.

## Remote Development

QmlStorybook supports remote development, which allows you to run the Storybook on your local machine and view the QML components on a remote device, such as an Android or iOS device. This is particularly useful for testing and debugging on different platforms.

To use remote development, you need to:

1.  **Start the Storybook in remote mode**: Run the Storybook with the `-m remote` or `--mode remote` command-line argument. This will start an HTTP server that serves your QML files.
2.  **Set up a reverse proxy**: On your remote device, you need to set up a reverse proxy to forward requests to the Storybook server running on your local machine. For Android, you can use `adb reverse`:

    ```bash
    adb reverse tcp:8080 tcp:8080
    ```

3.  **Run the Storybook on the device**: Run the Storybook application on your remote device. It will connect to the server on your local machine and display the QML components.

## Recommended Project Structure

When using QmlStorybook in your project, we recommend the following directory structure:

```
.
├── CMakeLists.txt
├── main.cpp
├── pages/
│   ├── MyComponentPage.qml
│   └── ...
├── qmlTests/
│   ├── tst_MyComponent.qml
│   └── ...
└── stubs/
    ├── MyDataStub.qml
    └── ...
```

-   `pages/`: Contains the Storybook pages for your components.
-   `qmlTests/`: Contains the QML tests for your components.
-   `stubs/`: Contains stub implementations of your data models or other dependencies.

## Mocking Data

When testing your components in isolation, you may need to provide mock data. You can do this by creating stub implementations of your data models and loading them into the QML context.

Here is an example of how to load mock data in your `main.cpp`:

```cpp
#include <QQmlApplicationEngine>
#include <QQmlContext>

void loadContextPropertiesMocks(const char* storybookRoot, QQmlApplicationEngine& engine)
{
    // Load your mock data here
    engine.rootContext()->setContextProperty("myData", "qrc:/stubs/MyDataStub.qml");
}
```

## Extending QmlStorybook

QmlStorybook can be extended with custom tools to fit your specific needs. For example, you can create a custom tool to validate your Storybook pages.

The `status-desktop` project includes a `PagesValidator` executable that demonstrates how to do this. The validator checks that all pages have a corresponding test file and that the test file is correctly named.
