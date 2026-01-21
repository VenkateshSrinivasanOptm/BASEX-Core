#ifndef RFIDSERVICE_H
#define RFIDSERVICE_H

#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include "rfidservicemessages.h"
#include <initializationmessage.h>
#include "rfidreader.h"

#define MWNAMESTR "rfidservice"
#define MSVERSTR "1.0"

class RfidCBusComm;

class Rfidservice : public QObject {
    Q_OBJECT
public:
    static Rfidservice &getInstance();
    /* initialisation method for startup sequence */
    void initialise(void);

private slots:
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    Rfidservice();
    virtual ~Rfidservice();
    Q_DISABLE_COPY(Rfidservice);
    QSharedPointer<RfidCBusComm> m_CBusComm;

    RFIDReader mCardReader;
};

#endif // RFIDSERVICE_H
