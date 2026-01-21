#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <uploadservice.h>
#include <cbusconfig.h>
#include <Utility.h>

int main(int argc, char *argv[]) {
    QCoreApplication uploadservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/us.cfg", "uploadservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        UploadService::getInstance().initialise();
        return uploadservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
