#include "downloadservice.h"
#include "downloadcbuscomm.h"
#include <QDebug>
#include <QCoreApplication>

DownloadService::DownloadService() : m_CBusComm(new DownloadCBusComm()) {
    qDebug() << "DownloadService " << __FUNCTION__;
}

DownloadService::~DownloadService() {}

DownloadService &DownloadService::getInstance() {
    static DownloadService instance;
    return instance;
}

void DownloadService::initialise() {
    qDebug() << "DownloadService " << __FUNCTION__;
    // Publish
    m_CBusComm->initialise();
}

TcpClient* DownloadService::client()
{
    return &mClient;
}

void DownloadService::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;
}

