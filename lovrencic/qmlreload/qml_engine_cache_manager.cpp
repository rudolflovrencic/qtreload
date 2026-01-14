#include "qml_engine_cache_manager.hpp"

#include <QtQml>
#include <QQmlComponent>
#include <QQmlApplicationEngine>

#include <cassert>

namespace lovrencic::qmlreload {

QmlEngineCacheManager::QmlEngineCacheManager(QObject* parent) noexcept : QObject{parent} {}

void QmlEngineCacheManager::clearCache()
{
    auto* const engine = qobject_cast<QQmlApplicationEngine*>(qmlEngine(this));
    if (!engine) { throw QmlEngineRetrievalError{}; }

    QMetaObject::invokeMethod(
        this,
        [this, engine]() noexcept -> void {
            engine->clearSingletons();
            engine->clearComponentCache();
            emit cacheCleared();
        },
        Qt::QueuedConnection);
}

QmlEngineRetrievalError::QmlEngineRetrievalError() noexcept : std::runtime_error{"failed to retrieve QML engine"} {}

}
