#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <gsmservice.h>
#include <Utility.h>

int main(int argc, char *argv[]) {

    QCoreApplication gsmservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/gsmservice.cfg", "gsmservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        Gsmservice::getInstance().initialise();
        return gsmservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
