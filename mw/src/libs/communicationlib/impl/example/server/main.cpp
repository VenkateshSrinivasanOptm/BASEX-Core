#include "server.h"
#include<cbusconfig.h>
#include<QTest>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList arguments = a.arguments();
    CBusConfig::initialize("server.cfg");
    QTest::qSleep(2000);
    Server server(arguments.at(1).toInt()/2);
    a.exec();
}
