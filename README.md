# QmlStorybook

QmlStorybook is a powerful tool for developers working with QML, designed to streamline the process of prototyping, developing, and testing QML applications. It provides a live development environment with features like hot reloading, Figma integration, and a built-in test runner.

## Features

- **Hot Reloading**: See your changes in real-time without rebuilding your application. QmlStorybook monitors your QML files for changes and automatically reloads the view.
- **HTTP Server**: A built-in HTTP server allows you to serve your QML files to other platforms, such as Android and iOS, for rapid testing and prototyping on different devices.
- **Figma Integration**: Seamlessly integrate your Figma designs into your QML applications. Link Figma components to your QML code and preview them side-by-side.
- **Integrated Test Runner**: Run your QML tests directly from the Storybook UI. Get immediate feedback on your tests and easily debug any issues.

## Getting Started

### Prerequisites

- Qt 6.2 or higher
- CMake 3.19 or higher
- A C++17 compliant compiler

### Building and Integration

#### Standalone

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/status-im/QmlStorybook.git
    cd QmlStorybook
    ```

2.  **Configure and build the project:**

    ```bash
    cmake -B build
    cmake --build build
    ```

3.  **Run the Storybook:**

    The Storybook executable will be located in the `build` directory.

#### CMake Integration with `FetchContent`

You can easily integrate QmlStorybook into your CMake project using `FetchContent`.

```cmake
include(FetchContent)

FetchContent_Declare(
  QmlStorybook
  GIT_REPOSITORY https://github.com/status-im/QmlStorybook.git
  GIT_TAG <commit_hash> # Use a specific commit hash for stability
)

FetchContent_MakeAvailable(QmlStorybook)

# Link against the QmlStorybook library
target_link_libraries(YourApp PRIVATE QmlStorybookLib)
```

### Usage

For detailed information on how to use QmlStorybook, please refer to the following documentation:

- **[C++ API Documentation](documentation/cpp_api.md)**
- **[QML API Documentation](documentation/qml_api.md)**
- **[Testing Documentation](documentation/testing.md)**
- **[Advanced Topics](documentation/advanced.md)**

## In Action

QmlStorybook is used in the development of the Status desktop application. You can see a live and complex example of its usage in the [status-desktop repository](https://github.com/status-im/status-desktop/tree/master/storybook). In this project, QmlStorybook is used for:

-   **Prototyping and developing** complex QML views with hot reloading.
-   **Inspecting and refactoring** QML components.
-   **Running QML tests** directly from the Storybook UI.
-   **Integrating with Figma** to ensure design consistency.

This real-world example showcases how QmlStorybook can be used to improve the development workflow of a large-scale QML application.

## Contributing

We welcome contributions from the community! If you'd like to contribute to QmlStorybook, please follow these steps:

1.  Fork the repository.
2.  Create a new branch for your feature or bug fix.
3.  Make your changes and commit them with descriptive messages.
4.  Push your changes to your fork.
5.  Create a pull request to the `master` branch of the main repository.

Before submitting a pull request, please make sure that your code adheres to the project's coding standards and that all tests pass.

## License

QmlStorybook is licensed under the [Mozilla Public License Version 2.0](LICENSE.md).
