#include "connectionmanagerservice.h"
#include "connectionmanagercbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>

ConnectionManagerservice::ConnectionManagerservice() : m_CBusComm(new ConnectionManagerCBusComm())
{

}

ConnectionManagerservice::~ConnectionManagerservice()
{

}

ConnectionManagerservice& ConnectionManagerservice::getInstance()
{
    static ConnectionManagerservice instance;
    return instance;
}

void ConnectionManagerservice::initialise()
{
    // Publish
    m_CBusComm->initialise();
    qDebug() << "ConnectionManagerservice " << __FUNCTION__;

    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));
}

void ConnectionManagerservice::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;
}
