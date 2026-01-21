#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <wifiservice.h>
#include <Utility.h>

int main(int argc, char *argv[]) {
    QCoreApplication rfidservice(argc, argv);

    QString configFilePath = createConfigFileFromResource(":/wifiservice.cfg", "wifiservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        Wifiservice::getInstance().initialise();
        return rfidservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
