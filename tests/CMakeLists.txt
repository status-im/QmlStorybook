find_package(QT NAMES Qt6 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

enable_testing()

add_executable(DirectoryFilesWatcherTest tst_DirectoryFilesWatcher.cpp)
target_link_libraries(DirectoryFilesWatcherTest PRIVATE Storybook Qt6::Test)
add_test(NAME DirectoryFilesWatcherTest COMMAND DirectoryFilesWatcherTest)

add_executable(SectionsDecoratorModelTest tst_SectionsDecoratorModel.cpp)
target_link_libraries(SectionsDecoratorModelTest PRIVATE Storybook Qt6::Test)
add_test(NAME SectionsDecoratorModelTest COMMAND SectionsDecoratorModelTest)
