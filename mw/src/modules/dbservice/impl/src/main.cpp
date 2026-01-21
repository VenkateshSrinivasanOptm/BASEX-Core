#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <dbservice.h>
#include <Utility.h>

int main(int argc, char *argv[]) {
    QCoreApplication dbservice(argc, argv);
    QDir dir;
    dir.mkdir("./logs/");
    createConfigFileFromResource(":/config.json", "config.json");
    QString configFilePath = createConfigFileFromResource(":/dbservice.cfg", "dbservice.cfg");
    if (CBusConfig::initialize(configFilePath)) {
        Dbservice::getInstance().initialise();
        return dbservice.exec();
    } else {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
