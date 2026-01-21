#include "dbservice.h"
#include "dbcbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>

Dbservice::Dbservice() : m_CBusComm(new DbCBusComm()) {

}

Dbservice::~Dbservice() {}

Dbservice &Dbservice::getInstance() {
    static Dbservice instance;
    return instance;
}
void Dbservice::initialise() {

    // Publish
    m_CBusComm->initialise();

    qDebug() << "Dbservice " << __FUNCTION__;
}


