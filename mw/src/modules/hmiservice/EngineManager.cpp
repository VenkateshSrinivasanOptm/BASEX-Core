#include "EngineManager.h"
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

QQmlApplicationEngine *EngineManager::engine() const { return m_engine; }

void EngineManager::setEngine(QQmlApplicationEngine *engine, QGuiApplication *app) {
    m_engine = engine;
    engine->rootContext()->setContextProperty("translationManager", &translationManager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreated, app,
        [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine->load(url);

}

EngineManager::EngineManager() : m_engine(nullptr) {}
