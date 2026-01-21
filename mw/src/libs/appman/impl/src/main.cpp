#include <QtCore/QCoreApplication>
#include <QProcess>
#include <QDebug>
#include "interface/include/mwstartup.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Start up the middleware components
    MwStartup startup;
    startup.startAll();

    return a.exec();
}
