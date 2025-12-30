#ifndef QMLENGINECACHEMANAGER_HPP
#define QMLENGINECACHEMANAGER_HPP

#include <QObject>

#include <QtQmlIntegration/qqmlintegration.h>

class QQmlEngine;
class QJSEngine;

class QmlEngineCacheManager : public QObject {
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
  private:
    QQmlEngine* qml_engine;

  public:
    explicit QmlEngineCacheManager(QQmlEngine* qml_engine, QObject* parent = nullptr) noexcept;

    [[nodiscard]] static QmlEngineCacheManager* create(QQmlEngine* qml_engine, QJSEngine* /*js_engine*/) noexcept;

  public slots:
    void clearCache();
};

#endif
