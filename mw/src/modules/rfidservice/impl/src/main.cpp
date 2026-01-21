#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <rfidservice.h>
#include <Utility.h>

int main(int argc, char *argv[]) {
    QCoreApplication rfidservice(argc, argv);

    QString configFilePath = createConfigFileFromResource(":/rfidservice.cfg", "rfidservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        Rfidservice::getInstance().initialise();
        return rfidservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
