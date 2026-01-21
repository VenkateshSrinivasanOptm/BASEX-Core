#include <QtCore/QCoreApplication>
#include <cbusconfig.h>
#include <cbusinterface.h>
#include <QDebug>
#include <QList>
#include <QTest>
#include "subscriber.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

      qDebug() << CBusConfig::initialize("sub.cfg");
      CBusInterface &bus(CBusInterface::instance());
      QTest::qSleep(5000);
      Subscriber sub;
      QList<quint16> types;
      types << 1;
      bus.subscribe(&sub,types);


    return a.exec();
}

