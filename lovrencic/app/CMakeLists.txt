add_executable(lovrencic-app)
target_link_libraries(lovrencic-app
    PRIVATE Qt6::Core
            Qt6::Gui
            Qt6::Qml
            Qt6::Quick
            Qt6::QuickControls2)
set(HOT_RELOADABLE_QML_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/First.qml"
    "${CMAKE_CURRENT_SOURCE_DIR}/qml/Second.qml")
if (TARGET lovrencic::qmlreload)
    target_link_libraries(lovrencic-app PRIVATE lovrencic::qmlreload)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        PROPERTIES
            COMPILE_DEFINITIONS HOT_RELOAD)
    set(HOT_RELOAD_ENTRY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/qml/First.qml")
    list(TRANSFORM HOT_RELOADABLE_QML_FILES PREPEND "\""
         OUTPUT_VARIABLE HOT_RELOAD_WATCHED_FILES)
    list(TRANSFORM HOT_RELOAD_WATCHED_FILES APPEND "\"")
    list(JOIN HOT_RELOAD_WATCHED_FILES ", " HOT_RELOAD_WATCHED_FILES)
    message("${HOT_RELOAD_WATCHED_FILES}")
    configure_file("qml/HotReloadableMain.qml.in"
        "${PROJECT_BINARY_DIR}/generated/HotReloadableMain.qml"
        @ONLY
        NEWLINE_STYLE LF)
    set_source_files_properties("${PROJECT_BINARY_DIR}/generated/HotReloadableMain.qml"
        PROPERTIES
            QT_RESOURCE_ALIAS "HotReloadableMain.qml")
    set(QML_FILES "${PROJECT_BINARY_DIR}/generated/HotReloadableMain.qml")
else()
    set(QML_FILES "${HOT_RELOADABLE_QML_FILES}")
    list(APPEND QML_FILES "qml/Main.qml")
endif()
qt_add_qml_module(lovrencic-app
    URI       "lovrencic.app"
    SOURCES   "main.cpp"
    QML_FILES "${QML_FILES}")
