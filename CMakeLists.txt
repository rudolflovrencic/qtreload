cmake_minimum_required(VERSION 3.22)

project(qtreload
    VERSION   "0.1.0"
    LANGUAGES "CXX")

option(HOT_RELOAD "Enable hot reload of QML files." false)

find_package(Qt6 "6.10.0"
    REQUIRED
    COMPONENTS
        Core
        Gui
        Graphs
        Qml
        Quick
        QuickControls2)
qt_standard_project_setup(REQUIRES "6.10.0")

if (HOT_RELOAD)
    add_subdirectory("lovrencic/qmlreload")
endif()
add_subdirectory("lovrencic/app")
