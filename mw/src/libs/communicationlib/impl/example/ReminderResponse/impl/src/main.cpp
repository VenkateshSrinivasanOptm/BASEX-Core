#include <QtCore/QCoreApplication>
#include <cbusconfig.h>
#include <QDebug>
#include <QTest>
#include "datasync.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << CBusConfig::initialize("ex2.cfg");
    QTest::qSleep(10000);  // this to make sure that al the thread are started
    DataSync ds;


    return a.exec();
}
