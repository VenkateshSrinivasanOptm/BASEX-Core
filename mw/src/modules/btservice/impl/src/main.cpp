#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <btservice.h>
#include "Utility.h"

int main(int argc, char *argv[]) {
    QCoreApplication btservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/btservice.cfg", "btservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        Btservice::getInstance().initialise();
        return btservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
