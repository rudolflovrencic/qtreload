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
