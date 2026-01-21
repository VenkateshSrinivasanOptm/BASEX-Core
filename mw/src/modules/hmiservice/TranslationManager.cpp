#include "TranslationManager.h"
#include <QQmlApplicationEngine>
#include <QDebug>

TranslationManager::TranslationManager(QObject *parent) : QObject(parent) {}

void TranslationManager::changeLanguage(const QString &language) {
    bool val = false;

    // Uninstall the previous translator
    QCoreApplication::removeTranslator(&translator);

    if (language == "English") {
        val = translator.load("translations/BaseX_Hmi_Eng.qm");
    } else if (language == "Hindi") {
        val = translator.load("translations/BaseX_Hmi_Hin.qm");
    }

    if (val) {
        // Install the new translator
        QCoreApplication::installTranslator(&translator);
        QGuiApplication::instance()->sendEvent(QCoreApplication::instance(), new QEvent(QEvent::LanguageChange));
    } else {
        qDebug() << "Failed to load translation for language:" << language;
    }
}

