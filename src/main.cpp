#include <QtQml>
#include <QQmlContext>
#include <QQuickStyle>
#include <QGuiApplication>
#include <QFileSystemWatcher>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(QStringLiteral("lovrencic"));
    QCoreApplication::setApplicationName(QStringLiteral("qtreload"));

    QGuiApplication app{argc, argv};

    QQuickStyle::setStyle("Basic");

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() noexcept -> void { QCoreApplication::exit(EXIT_FAILURE); },
        Qt::QueuedConnection);

    engine.loadFromModule("lovrencic.qtreload", "Main");

    return app.exec();
}
