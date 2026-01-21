#include "rfidservice.h"
#include "rfidcbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>


Rfidservice::Rfidservice() : m_CBusComm(new RfidCBusComm()) {


}

Rfidservice::~Rfidservice() {}

Rfidservice &Rfidservice::getInstance() {
    static Rfidservice instance;
    return instance;
}
void Rfidservice::initialise() {

    // Publish
    m_CBusComm->initialise();
    qDebug() << "Rfidservice " << __FUNCTION__;
    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));
    mCardReader.start();
}

void Rfidservice::processStart(QSharedPointer<const InitializationMessage> msg)
{
 Q_UNUSED(msg)
    qDebug() << __FUNCTION__;

}

