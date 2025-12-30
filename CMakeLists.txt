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
add_executable(qtreload)
target_sources(qtreload
    PRIVATE
        "src/main.cpp"
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS "src")
target_link_libraries(qtreload
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Graphs)

# TODO: Consider making the hot reloder a separate QML module.

set(SOURCES
    "src/watcher.hpp"
    "src/watcher.cpp")
set(QML_FILES "Main.qml")
set(HOT_RELOADABLE_QML_FILES
    "content/First.qml"
    "content/Second.qml")
if (HOT_RELOAD)
    list(APPEND SOURCES
        "src/qml_engine_cache_manager.hpp"
        "src/qml_engine_cache_manager.cpp")
    set(MAIN_HOT_RELOADABLE_QML_FILE_PATH "${PROJECT_SOURCE_DIR}/content/First.qml")
    list(TRANSFORM HOT_RELOADABLE_QML_FILES PREPEND "\"${PROJECT_SOURCE_DIR}/"
         OUTPUT_VARIABLE HOT_RELOADABLE_QML_FILE_PATHS)
    list(TRANSFORM HOT_RELOADABLE_QML_FILE_PATHS APPEND "\"")
    list(JOIN HOT_RELOADABLE_QML_FILE_PATHS ", " HOT_RELOADABLE_QML_FILE_PATHS)
    configure_file("${PROJECT_SOURCE_DIR}/HotReloader.qml.in"
        "${PROJECT_BINARY_DIR}/generated/HotReloader.qml"
        @ONLY
        NEWLINE_STYLE LF)
    list(APPEND QML_FILES "${PROJECT_BINARY_DIR}/generated/HotReloader.qml")
    set_source_files_properties("${PROJECT_BINARY_DIR}/generated/HotReloader.qml"
        PROPERTIES
            QT_RESOURCE_ALIAS "HotReloader.qml")
else()
    list(APPEND QML_FILES "${HOT_RELOADABLE_QML_FILES}")
endif()
qt_add_qml_module(qtreload
    URI             "lovrencic.qtreload"
    VERSION         "1.0"
    RESOURCE_PREFIX "/"
    SOURCES         "${SOURCES}"
    QML_FILES       "${QML_FILES}")
