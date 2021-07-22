#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QString>

#include "QFWIPreProcess.h"
#include "QFWIProcess.h"
#include "QFWIPostProcess.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<QPreProcess>("com.fwi", 1, 0, "QPreProcess");
    qmlRegisterType<QProcess>("com.fwi", 1, 0, "QProcess");
    qmlRegisterType<QPostProcess>("com.fwi", 1, 0, "QPostProcess");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
