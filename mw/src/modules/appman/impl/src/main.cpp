#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <cbusconfig.h>
#include <mwstartup.h>
#include <Utility.h>
#define CONFIG_FILE ":/appman.cfg"
#include <iostream>

void customLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString typeStr;
    QString formattedMessage;

    // Determine the type of message
    switch (type) {
    case QtDebugMsg:
        typeStr = "[DEBUG]";
        break;
    case QtInfoMsg:
        typeStr = "[INFO]";
        break;
    case QtWarningMsg:
        typeStr = "[WARNING]";
        break;
    case QtCriticalMsg:
        typeStr = "[CRITICAL]";
        break;
    case QtFatalMsg:
        typeStr = "[FATAL]";
        break;
    }

    // Process and format the message
    QString processedMsg = msg;
    processedMsg.replace("\\n", "\n");  // Replace escaped newlines with actual newlines
    processedMsg.replace("\\\"", "\""); // Replace escaped quotes with actual quotes
    processedMsg = processedMsg.trimmed(); // Remove leading and trailing whitespace

    // Add indentation for readability
    QStringList lines = processedMsg.split('\n');
    for (int i = 1; i < lines.size(); ++i) {
        lines[i] = "    " + lines[i]; // Indent all lines except the first
    }
    processedMsg = lines.join('\n');

    // Format the log message with timestamp and context
    formattedMessage = QString("%1 | %2 (%3:%4)\n%5")
                           .arg(typeStr,
                                context.file ? QString(context.file) : "Unknown file",
                                QString::number(context.line),
                                QString(context.function ? context.function : "Unknown function"),
                                processedMsg);

    // Print the formatted message
    std::cout << formattedMessage.toStdString() << std::endl;

    // If fatal, abort
    if (type == QtFatalMsg) {
        abort();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customLogHandler);
    Q_INIT_RESOURCE(appmanlib);
    QString configFilePath = createConfigFileFromResource(":/appman.cfg", "appman.cfg");

#ifndef TEST_BUILD
    if (!CBusConfig::initialize(configFilePath))
    {
        qDebug() << "ERROR : FAILED TO OPEN CONFIG FILE " << CONFIG_FILE;
        return 1;
    }
#endif

    // Start up the middleware components
    MwStartup startup;
    startup.startAll();

    return a.exec();
}








