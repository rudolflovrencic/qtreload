#include <QtQml>
#include <QQmlContext>
#include <QQuickStyle>
#include <QGuiApplication>
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(QStringLiteral("lovrencic"));
    QCoreApplication::setApplicationName(QStringLiteral("app"));

    const QGuiApplication app{argc, argv};

    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() noexcept -> void { QCoreApplication::exit(EXIT_FAILURE); },
        Qt::QueuedConnection);

    static constexpr const char main_source_file[]{
#ifdef HOT_RELOAD
        "HotReloadableMain"
#else
        "Main"
#endif
    };
    engine.loadFromModule("lovrencic.app", main_source_file);

    return app.exec();
}
