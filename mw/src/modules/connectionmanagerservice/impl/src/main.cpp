#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <connectionmanagerservice.h>
#include <Utility.h>

int main(int argc, char *argv[])
{
    QCoreApplication fotaservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/connectionmanagerservice.cfg", "connectionmanagerservice.cfg");
    if (CBusConfig::initialize(configFilePath))
    {
        ConnectionManagerservice::getInstance().initialise();
        return fotaservice.exec();
    }
    else
    {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
