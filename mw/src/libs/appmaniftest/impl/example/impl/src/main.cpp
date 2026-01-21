#include <QtCore/QCoreApplication>
#include "mwapp.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MwApp mwApp;

    AppManIf::getInstance();
    mwApp.initialise();

    return a.exec();
}
