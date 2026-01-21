#include "uploadservice.h"
#include "uploadcbuscomm.h"
#include <QDebug>
#include <QCoreApplication>
#include "tcpclient.h"

UploadService::UploadService() : m_CBusComm(new UploadCBusComm()) {

    qDebug() << "UploadService " << __FUNCTION__;
    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));
}

UploadService::~UploadService() {}

TcpClient* UploadService::client()
{
    return &mClient;
}
UploadService &UploadService::getInstance() {
    static UploadService instance;
    return instance;
}

void UploadService::initialise() {

    // Publish
    m_CBusComm->initialise();
    connect(m_CBusComm.data(), SIGNAL(uploadCanData(quint16,quint16,QString&)),
            this, SLOT(uploadCanData(quint16, quint16, QString&)));

    qDebug() << "UploadService " << __FUNCTION__;
}

void UploadService::uploadCanData(quint16 requestId, quint16 messageType,
                                   const QString &data) {
    Q_UNUSED(requestId);
    Q_UNUSED(messageType);
    Q_UNUSED(data);
    // TODO - perform upload can data.
    qDebug() << __FUNCTION__;
}

void UploadService::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;
}

