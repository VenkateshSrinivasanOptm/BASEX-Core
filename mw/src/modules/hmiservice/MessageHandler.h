#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <QDir>
#include <QGuiApplication>
void cleanupOldLogs(const QString &logFolder, int daysToKeep = 5) {
    QDir dir(logFolder);
    if (!dir.exists()) {
        return;
    }

    QFileInfoList fileList = dir.entryInfoList(QDir::Files, QDir::Time);
    QDateTime currentDateTime = QDateTime::currentDateTime();

    for (const QFileInfo &fileInfo : fileList) {
        QDateTime fileDate = fileInfo.lastModified();
        if (fileDate.daysTo(currentDateTime) > daysToKeep) {
            QString p = fileInfo.filePath();
            if (QFile::remove(fileInfo.filePath())) {
                qDebug() << "Deleted old log file:" << fileInfo.fileName();
            } else {
                qDebug() << "Failed to delete log file:" << fileInfo.fileName();
            }
        }
    }
}


void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg) {
    QString logFolder = "Logs";
    QDir().mkpath(logFolder);
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString logFilePath = logFolder + "/log_" + date + ".txt";

    cleanupOldLogs(qApp->applicationFilePath()+"/"+logFolder, 10);

    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);

        QString message;
        switch (type) {
        case QtDebugMsg:    message = qAppName() + QString(" Debug: %1").arg(msg); break;
        case QtInfoMsg:     message = qAppName() +QString(" Info: %1").arg(msg); break;
        case QtWarningMsg:  message = qAppName() +QString(" Warning: %1").arg(msg); break;
        case QtCriticalMsg: message = qAppName() +QString(" Critical: %1").arg(msg); break;
        case QtFatalMsg:    message = qAppName() +QString(" Fatal: %1").arg(msg); break;
        }
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
        logFile.flush();
    } else {
        qDebug() << "Failed to open log file!";
    }
    logFile.close();
}


#endif // MESSAGEHANDLER_H
