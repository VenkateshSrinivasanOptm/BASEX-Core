#ifndef CONNECTIONMANAGERCBUSCOMM_H
#define CONNECTIONMANAGERCBUSCOMM_H

#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include "connectionmanagerservicemessages.h"
#include <appmanif.h>
#include <initializationmessage.h>

class CBusInterface;

enum ConnectionState
{
    GSM_CONNECTED,
    WIFI_CONNECTED
};

class ConnectionManagerCBusComm : public QObject,
                   public MessageHandler,
                   public Deserializer,
                   public SubscriberIntf {
    Q_OBJECT
public:
    ConnectionManagerCBusComm();
    virtual ~ConnectionManagerCBusComm();

    /*! To initialize the CBus and subscribe it to the Startup services */
    void initialise();

    /*! Method to deserialize messages transmitted by the communication lirary */
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &stream);

    /*! Methods handles messages received from Resource Controller */
    virtual bool messageHandler(MessagePtr message);

    /*! Method handles received messages transmitted by the communication lirary
   */
    virtual bool messageHandler(quint16 requestId, MessagePtr message);

    /*! Method to deserialize the startup message: BLSTARTUP_INTIALIZATION */
    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);

    /*! Method to handle incoming startup message: BLSTARTUP_INTIALIZATION */
    bool subscriberMessageHandler(MessagePtr message);

    /*! Forwards sendReply to CBus */
    void sendReply(quint16 requestId, MessagePtr const &message);

    /*! Forwards callService (= noreply message to a service) to CBus */
    bool callService(quint8 serviceName, MessagePtr const &message);

    /*! Forwards push (= publish message to all the subscribed services) to CBus
   */
    void push(MessagePtr const &message);

public slots:
    /* heartbeat method for application manager */
    void heartbeat();
    /* subscribe method for startup sequence */
    void establishCommunication(void);
    /* handle poweer rstart notication method for starequestIdrtup sequence */
    void handlePowerRestartNotification(StateChange state);

signals:
    void startupService(QSharedPointer<InitializationMessage const>);

private:
    CBusInterface &cbus;
    bool m_isWifiConnected;
    ConnectionState m_currentConnectionMgrState;

    int m_gsmSignalStrength;
    int m_wifiSignalStrength;

    void connectWifi();
    void disconnectWifi();
    void handleGsmSignalStrength(int gsmSignalStrength);
    void handleWifiSignalStrength(int wifiSignalStrength);
};

#endif // CONNECTIONMANAGERCBUSCOMM_H
