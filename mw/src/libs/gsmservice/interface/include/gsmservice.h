#ifndef GSMSERVICE_H
#define GSMSERVICE_H

#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include "gsmservicemessages.h"
#include <initializationmessage.h>
#include <QSerialPort>
#include <QTimer>

#define TESTING
#define MWNAMESTR "gsmservice"
#define MSVERSTR "1.0"

class GsmCBusComm;

class Gsmservice : public QObject {
    Q_OBJECT
public:
    static Gsmservice& getInstance();

    /* initialisation method for startup sequence */
    void initialise(void);

public slots:
    void processStart(QSharedPointer<InitializationMessage const>);
public slots:
    void readData();
    void writeData(QByteArray data);
    void handleError();
    void CheckSignalStrength();

private:
    Gsmservice();
    virtual ~Gsmservice();
    Q_DISABLE_COPY(Gsmservice);
    QSharedPointer<GsmCBusComm> m_CBusComm;
    QSharedPointer<QSerialPort> m_serial;
    QString str;
    QTimer m_signalStrength;
};

#endif // GSMSERVICE_H
