cmake_minimum_required(VERSION 3.5)

project(TimeTracker VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Charts REQRUIED)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        mainwindow.ui
        TimeTracker.h
        TimeTracker.cpp
        DateRangeSelector.h
        DateRangeSelector.cpp
        ChartsView.h
        ChartsView.cpp
        SettingsDialog.h
        SettingsDialog.cpp
        CategoriesTab.h
        CategoriesTab.cpp
        ApplicationsTab.h
        ApplicationsTab.cpp
        Settings.h
        Settings.cpp
        ApplicationSettingsEdit.h
        ApplicationSettingsEdit.cpp
        CategorySettingsEdit.h
        CategorySettingsEdit.cpp
        Utils.h
        Utils.cpp
        FileIO.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(WIN32)
        qt_add_executable(TimeTracker WIN32 MANUAL_FINALIZATION ${PROJECT_SOURCES} "resources/icon.rc")
    else()
        qt_add_executable(TimeTracker MANUAL_FINALIZATION ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(TimeTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TimeTracker PRIVATE Qt${QT_VERSION_MAJOR}::Charts)

set_target_properties(TimeTracker PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeTracker)
endif()
