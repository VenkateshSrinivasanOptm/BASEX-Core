#ifndef UTILITY_H
#define UTILITY_H

#include <QDir>
#include <QString>
#include <QCoreApplication>

QString createConfigFileFromResource(const QString &resourcePath, const QString &fileName) {
    // Get the executable directory
    QString exeDirPath = QCoreApplication::applicationDirPath();
    QString filePath = QDir(exeDirPath).filePath(fileName); // Construct the full path

    QFile resourceFile(resourcePath);
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open resource file:" << resourcePath;
        return QString();
    }

    // Create an actual file in the executable directory with the specified name
    QFile actualFile(filePath);
    if (actualFile.open(QIODevice::WriteOnly)) {
        actualFile.write(resourceFile.readAll()); // Copy contents from the resource file
        actualFile.close();
        return actualFile.fileName(); // Return the full path of the created file
    } else {
        qWarning() << "Could not create file at:" << filePath;
        return QString();
    }
}

#endif // UTILITY_H
