#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <downloadservice.h>
#include <cbusconfig.h>
#include <Utility.h>

int main(int argc, char *argv[]) {
    QCoreApplication downloadservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/ds.cfg", "downloadservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        DownloadService::getInstance().initialise();
        return downloadservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
