#ifndef ENGINEMANAGER_H
#define ENGINEMANAGER_H

#include "TranslationManager.h"
#include <QQmlApplicationEngine>
class QGuiApplication;
class EngineManager {
public:
    EngineManager();

    QQmlApplicationEngine* engine() const;
    void setEngine(QQmlApplicationEngine *engine, QGuiApplication* app);

private:
    QQmlApplicationEngine *m_engine;
    TranslationManager translationManager;

};

#endif // ENGINEMANAGER_H
