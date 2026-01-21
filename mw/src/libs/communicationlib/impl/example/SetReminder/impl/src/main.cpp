#include <QtCore/QCoreApplication>
#include <QDebug>
#include <cbusconfig.h>
#include "dtv.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

      qDebug() << CBusConfig::initialize("ex1.cfg");
      Dtv dtv;


      dtv.setReminder(1, 2334);


    return a.exec();
}
