#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "MessageHandler.h"
#include "EngineManager.h"
#include "Utility.h"

#include <QDir>
#include <QTranslator>
#include <QTemporaryFile>
#include <QFont>

# if not defined (SIMULATION)
#include <cbusconfig.h>
#endif
 bool PRINT_COMMANDLINE = false;

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    if(argc>1){
        PRINT_COMMANDLINE = (*argv[1] == '1');
    }

  //  if(!PRINT_COMMANDLINE) {
   //     qputenv("QT_LOGGING_RULES", "*.debug=false;*.warning=false");
   // }

  //  qInstallMessageHandler(customMessageHandler);

    QGuiApplication app(argc, argv);
# if not defined (SIMULATION)

    QString configFilePath = createConfigFileFromResource(":/communication/ui.cfg", "ui.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        qDebug() << "Config file registered successfully";
    }
    else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
#endif
    // Initialize Translator
    // QTranslator translator;
    // const QStringList uiLanguages = QLocale::system().uiLanguages();
    // for (auto &locale : uiLanguages) {
    //     QString s = "hi_IN";
    //     const QString baseName = "BaseX_Hmi_" + QLocale(s).name();
    //     if (translator.load(":/i18n/" + baseName)) {
    //         qApp->installTranslator(&translator);
    //         break;
    //     }
    // }

    QQmlApplicationEngine engine;
    // Initialize EngineManager

    EngineManager engineManager;
    engineManager.setEngine(&engine, &app);


    QFont font("Inter");
    app.setFont(font);

    return app.exec();
}
