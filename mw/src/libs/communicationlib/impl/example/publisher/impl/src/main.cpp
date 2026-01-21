#include <QtCore/QCoreApplication>
#include <cbusconfig.h>
#include <cbusinterface.h>
#include <QDebug>
#include <QTest>

#include "pubMessage.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

      qDebug() << CBusConfig::initialize("pub.cfg");
      CBusInterface &bus(CBusInterface::instance());

      QTest::qSleep(10000);
      PubMessage *msg=new PubMessage(PUB_MESSAGE,"test", 1);
      MessagePtr message=MessagePtr(msg);
      bus.push(message);



    return a.exec();
}

