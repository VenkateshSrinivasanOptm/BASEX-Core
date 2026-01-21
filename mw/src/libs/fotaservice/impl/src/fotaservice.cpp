#include "fotaservice.h"
#include "fotacbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>

FotaService::FotaService() : m_CBusComm(new FotaCBusComm())
{

}

FotaService::~FotaService()
{

}

FotaService& FotaService::getInstance()
{
    static FotaService instance;
    return instance;
}

void FotaService::initialise()
{
    // Publish
    m_CBusComm->initialise();
    qDebug() << "fotaservice " << __FUNCTION__;

    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));
}

void FotaService::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;
}
