#ifndef CONNECTIONMANAGERSERVICE_H
#define CONNECTIONMANAGERSERVICE_H

#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include "connectionmanagerservicemessages.h"
#include <initializationmessage.h>

#define MWNAMESTR "ConnectionManagerservice"
#define MSVERSTR "1.0"

class ConnectionManagerCBusComm;

class ConnectionManagerservice : public QObject {
    Q_OBJECT
public:
    static ConnectionManagerservice& getInstance();

    /* initialisation method for startup sequence */
    void initialise(void);

public slots:
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    ConnectionManagerservice();
    virtual ~ConnectionManagerservice();
    Q_DISABLE_COPY(ConnectionManagerservice);
    QSharedPointer<ConnectionManagerCBusComm> m_CBusComm;
};

#endif // CONNECTIONMANAGERSERVICE_H
