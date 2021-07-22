#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QString>
#include <QQmlContext>
#include <QDir>

#include "Launcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Launcher>("com.fwi", 1, 0, "Launcher");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    engine.rootContext()->setContextProperty("applicationPath", "file://"+qApp->applicationDirPath()+ "/");
    engine.rootContext()->setContextProperty("CurDirPath", QString(QDir::currentPath() + "/"));


    return app.exec();
}
