#include "client.h"
#include<cbusconfig.h>
#include<QTest>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CBusConfig::initialize("client.cfg");
    QStringList arguments = a.arguments();
    QTest::qSleep(2000);
    Client client(arguments.at(1).toInt());
    a.exec();
}

