#include "qml_engine_cache_manager.hpp"

#include <QQmlEngine>
#include <QQmlComponent>

#include <cassert>

QmlEngineCacheManager::QmlEngineCacheManager(QQmlEngine* qml_engine, QObject* parent) noexcept
        : QObject{parent}, qml_engine{qml_engine}
{
    assert(qml_engine);
}

QmlEngineCacheManager* QmlEngineCacheManager::create(QQmlEngine* qml_engine, QJSEngine* /*js_engine*/) noexcept
{
    return new QmlEngineCacheManager{qml_engine};
}

void QmlEngineCacheManager::clearCache()
{
    qDebug() << "clearing cache!";
    qml_engine->clearComponentCache();
}
