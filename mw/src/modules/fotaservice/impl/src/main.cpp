#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <cbusconfig.h>
#include <fotaservice.h>
#include <Utility.h>

int main(int argc, char *argv[])
{
    QCoreApplication fotaservice(argc, argv);
    QString configFilePath = createConfigFileFromResource(":/fotaservice.cfg", "fotaservice.cfg");
    if (CBusConfig::initialize(configFilePath))
    {
        FotaService::getInstance().initialise();
        return fotaservice.exec();
    }
    else
    {
        qDebug() << "Could not initiate from cfg file. We are in current dir : "
                 << QDir::currentPath();
    }
}
