# set(MAIN_HOT_RELOADABLE_QML_FILE_PATH "${PROJECT_SOURCE_DIR}/qml/First.qml")
# list(TRANSFORM HOT_RELOADABLE_QML_FILES PREPEND "\"${PROJECT_SOURCE_DIR}/"
#      OUTPUT_VARIABLE HOT_RELOADABLE_QML_FILE_PATHS)
# list(TRANSFORM HOT_RELOADABLE_QML_FILE_PATHS APPEND "\"")
# list(JOIN HOT_RELOADABLE_QML_FILE_PATHS ", " HOT_RELOADABLE_QML_FILE_PATHS)
# configure_file("${PROJECT_SOURCE_DIR}/HotReloader.qml.in"
#     "${PROJECT_BINARY_DIR}/generated/HotReloader.qml"
#     @ONLY
#     NEWLINE_STYLE LF)
# list(APPEND QML_FILES "${PROJECT_BINARY_DIR}/generated/HotReloader.qml")
# set_source_files_properties("${PROJECT_BINARY_DIR}/generated/HotReloader.qml"
#     PROPERTIES
#         QT_RESOURCE_ALIAS "HotReloader.qml")

add_library(lovrencic-qmlreload SHARED)
qt_add_qml_module(lovrencic-qmlreload
    URI       "lovrencic.qmlreload"
    SOURCES   "qml_engine_cache_manager.hpp"
              "qml_engine_cache_manager.cpp"
              "watcher.hpp"
              "watcher.cpp"
    QML_FILES "qml/HotReloader.qml")
target_link_libraries(lovrencic-qmlreload
    PUBLIC Qt6::Core
           Qt6::Qml
           Qt6::Quick)
add_library(lovrencic::qmlreload ALIAS lovrencic-qmlreload)
