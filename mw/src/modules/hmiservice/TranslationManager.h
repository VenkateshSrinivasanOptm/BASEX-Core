#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>

class TranslationManager : public QObject {
    Q_OBJECT
public:
    explicit TranslationManager(QObject *parent = nullptr);

    Q_INVOKABLE void changeLanguage(const QString &language);

private:
    QTranslator translator;
};

#endif // TRANSLATIONMANAGER_H
