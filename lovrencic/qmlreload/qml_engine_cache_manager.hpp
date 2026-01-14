#ifndef QMLENGINECACHEMANAGER_HPP
#define QMLENGINECACHEMANAGER_HPP

#include <QObject>

#include <QtQmlIntegration/qqmlintegration.h>

#include <stdexcept>
#include <type_traits>

namespace lovrencic::qmlreload {

class QmlEngineCacheManager : public QObject {
    Q_OBJECT
    QML_ELEMENT
  public:
    explicit QmlEngineCacheManager(QObject* parent = nullptr) noexcept;

    [[nodiscard]] QString get_hot_reloader_object_name() const noexcept;
    void set_hot_reloader_object_name(QString newHotReloaderObjectName) noexcept;

  public slots:
    void clearCache();

  signals:
    void cacheCleared();
    void hotReloaderObjectNameChanged(QString newHotReloaderObjectName);
};

class QmlEngineRetrievalError final : public std::runtime_error {
  public:
    QmlEngineRetrievalError() noexcept;
};
static_assert(std::is_nothrow_copy_constructible_v<QmlEngineRetrievalError>);

}

#endif
