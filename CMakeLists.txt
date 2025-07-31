cmake_minimum_required(VERSION 3.19)

project(QmlStorybook VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

option(QMLSTORYBOOK_BUILD_TESTS "Build tests" OFF)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick HttpServer)
find_package(Qt6 REQUIRED COMPONENTS Core Quick HttpServer)

qt_add_library(QmlStorybookLib
  # sources
  src/cachecleaner.cpp
  src/directorieswatcher.cpp
  src/directoryfileswatcher.cpp
  src/figmaio.cpp
  src/figmalinks.cpp
  src/figmalinksmodel.cpp
  src/localpagessource.cpp
  src/modelutils.cpp
  src/pagesmodel.cpp
  src/qmlfilesserver.cpp
  src/remotepagessource.cpp
  src/sectionsdecoratormodel.cpp
  src/storybookdata.cpp
  src/storybooksetup.cpp
  src/systemutils.cpp
  src/testsrunner.cpp
  # headers
  include/Storybook/abstractpagessource.h
  include/Storybook/cachecleaner.h
  include/Storybook/directorieswatcher.h
  include/Storybook/directoryfileswatcher.h
  include/Storybook/figmaio.h
  include/Storybook/figmalinks.h
  include/Storybook/figmalinksmodel.h
  include/Storybook/localpagessource.h
  include/Storybook/modelutils.h
  include/Storybook/pagesmodel.h
  include/Storybook/pagesmodelenums.h
  include/Storybook/qmlfilesserver.h
  include/Storybook/remotepagessource.h
  include/Storybook/sectionsdecoratormodel.h
  include/Storybook/storybookdata.h
  include/Storybook/storybooksetup.h
  include/Storybook/systemutils.h
  include/Storybook/testsrunner.h
  # qrc
  qml/storybook.qrc
)

set(QML_STORYBOOK_QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/qml"
  CACHE STRING "Path to QmlStorybook qml files")

target_include_directories(QmlStorybookLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(FetchContent)

if(NOT TARGET SortFilterProxyModel)
  FetchContent_Declare(
    SortFilterProxyModel
    GIT_REPOSITORY https://github.com/status-im/SortFilterProxyModel.git
    GIT_TAG 6477b1e465b02f18007900125864cdfa812ba8ad
  )

  FetchContent_MakeAvailable(SortFilterProxyModel)
  target_compile_options(SortFilterProxyModel PRIVATE -w)
endif()

if(NOT TARGET QtModelsToolkit)
  FetchContent_Declare(
    QtModelsToolkit
    GIT_REPOSITORY https://github.com/status-im/QtModelsToolkit.git
    GIT_TAG 99588ada4371dc639c46836b0fca1a880428ef7c
  )

  FetchContent_MakeAvailable(QtModelsToolkit)
endif()

target_link_libraries(QmlStorybookLib
  PRIVATE Qt6::Core Qt6::Quick Qt6::HttpServer SortFilterProxyModel QtModelsToolkit)

if(QMLSTORYBOOK_BUILD_TESTS)
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS QmlStorybookLib
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
