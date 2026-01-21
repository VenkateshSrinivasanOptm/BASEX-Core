#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <canservice.h>
#include <Utility.h>

int main(int argc, char *argv[])
{
    QCoreApplication canservice(argc, argv);

    QString configFilePath = createConfigFileFromResource(":/canservice.cfg", "canservice.cfg");
    if (CBusConfig::initialize(configFilePath))
    {
        Canservice::getInstance().initialise();
        return canservice.exec();
    }
    else
    {
        qDebug() << "[ERROR] Failed to initiate configuration from the specified file. Current working directory is:"
                 << QDir::currentPath();
    }
}
