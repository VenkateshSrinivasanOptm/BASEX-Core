#ifndef UPSERVICE_H
#define UPSERVICE_H
#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include "tcpclient.h"
#include "initializationmessage.h"

#define MWNAMESTR "uploadservice"
#define MSVERSTR "1.0"

class UploadCBusComm;

class UploadService : public QObject {
    Q_OBJECT
public:
    static UploadService &getInstance();

    /* initialisation method for startup sequence */
    void initialise(void);
    TcpClient* client();

signals:/* signals to process to received can data */
    void Registraion_Fixed_Part_CAN_Message();
    void Registraion_Dynamic_Part_CAN_Message();
    void Heart_Beat_Fixed_Part_CAN_Message();
    void Heart_Beat_Dynamic_Part_CAN_Message();
    void System_Health_Fixed_Part_CAN_Message();
    void System_Health_Dynamic_Part_CAN_Message();


public slots:
    void uploadCanData(quint16 requestId, quint16 messageType, const QString &data);
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    UploadService();
    virtual ~UploadService();
    Q_DISABLE_COPY(UploadService)
    QSharedPointer<UploadCBusComm> m_CBusComm;
    TcpClient mClient;

};

#endif // UPSERVICE_H
