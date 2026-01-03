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
        Qt6::QuickControls2)

function (enable_qml_hotreload target)
    get_target_property(source_dir ${target} SOURCE_DIR)
    get_target_property(qmldir_content ${target} _qt_internal_qmldir_content)
    string(REGEX REPLACE "prefer [^\n]*" "prefer ${source_dir}/" qmldir_content "${qmldir_content}")
    set_property(TARGET ${target} PROPERTY _qt_internal_qmldir_content "${qmldir_content}")
endfunction()

qt_add_qml_module(qtreload
    URI       "lovrencic.qtreload"
    SOURCES   "src/qml_engine_cache_manager.hpp"
              "src/qml_engine_cache_manager.cpp"
    QML_FILES "Main.qml"
              "qml/First.qml"
              "qml/Second.qml")
if (HOT_RELOAD)
    enable_qml_hotreload(qtreload)
endif()
