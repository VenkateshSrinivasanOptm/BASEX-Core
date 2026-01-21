#include "wificbuscomm.h"
#include "cbusinterface.h" // communication library
#include <comlibservices.h>
#include <WifiSignalStrengthMessage.h>
#include <QDebug>
#include <QTimer>
#include <appmanif.h>
#include <wifiservice.h>
#include <initializationmessage.h>
#include <connectionmanagerservicemessages.h>

WifiCBusComm::WifiCBusComm() : cbus(CBusInterface::instance())
{
    // QList<quint16> messages;
    // messages    << CAN_SERV_UPDATE << ConnectWifiMessageMacro << DisconnectWifiMessageMacro;
    // cbus.subscribe(this, messages);
}

void WifiCBusComm::initialise() {

    connect(AppManIf::getInstance(), SIGNAL(establishCommunication()), this, SLOT(establishCommunication()));
    connect(AppManIf::getInstance(), SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(AppManIf::getInstance(), SIGNAL(heartbeatNotification()), this, SLOT(heartbeat()));

    AppManIf::getInstance()->notifyPublished(MWNAMESTR, MSVERSTR);
    qDebug() << "Madhur AppManIf::getInstance()->notifyPublished dbservice";

    QTimer::singleShot(REGISTER_SERV_DELAY,[this](){
        if(!cbus.registerService(WIFI_SERV, this, this))
        {
            qDebug() << "WIFI_SERV Already regitered";
        }
        else
            qDebug() << "WIFI_SERV regitered successfully";

        QList<quint16> messages;
        messages << BLSTARTUP_INTIALIZATION << CAN_SERV_UPDATE << ConnectWifiMessageMacro << DisconnectWifiMessageMacro;

        foreach (quint16 msg, messages) {
            qDebug() << "msg : " << msg;
        }
        cbus.subscribe(this, messages);
    });
}

void WifiCBusComm::establishCommunication(void)
{
    qDebug() <<__FUNCTION__ ;
    // Subscribe

    // subscribe for events from the application business logic startup component (stefan)
    // currently already done in initialise
    AppManIf::getInstance()->communicationEstablished();   //Subscribed

    // STARTUP COMPLETE.
}

void WifiCBusComm::heartbeat() {
    qDebug() <<__FUNCTION__ ;
    AppManIf::getInstance()->sendHeartbeatResponse();
}

void WifiCBusComm::handlePowerRestartNotification(StateChange state)
{
    qDebug() <<  __FUNCTION__;
    switch (state)
    {
    case POWER_ON:
    {    break;
    }
    case RESTART_OTHER:      // notification that other MW modules are having to restart, no restart
        break;
    case RESTARTED_MW_ACTIVE: // notification that the MW has completed a restart and all components are online.
        break;
    case POWER_OFF:          // Power notification that the box has been switched off. termination required
    case POWER_STANDBY:      // power notification that we are going to standby, no restart
    case POWER_DEEP_SLEEP:   // Power notification that the box has been set to deep sleep. termination required
    {
        //TODO
    }
    case RESTART:
    { //TODO
    }
    default:
        exit(STB_LEVEL_ERR);
    }
}

WifiCBusComm::~WifiCBusComm() { cbus.unregisterService(WIFI_SERV); }

MessagePtr WifiCBusComm::deserialize(quint16 messageType, QDataStream &stream) {

    Message *message(0);
    qDebug() << __FUNCTION__ << messageType;
    switch (messageType)
    {

    }
    if (message == NULL) {
        return MessagePtr();
    } else {
        message->deserialize(stream);
        return MessagePtr(message);
    }
}

bool WifiCBusComm::messageHandler(quint16 requestId, MessagePtr message) {
    qDebug() << __FUNCTION__ << " request id" << requestId;
    switch (message->messageType) {

    }
    return false;
}

bool WifiCBusComm::messageHandler(MessagePtr message) {
    qDebug() << __FUNCTION__ << message->messageType;

    switch (message->messageType) {
    }
    return false;
}

//-------------------------------------------------------
// Deserialize Startup service messages
//-------------------------------------------------------
MessagePtr WifiCBusComm::subscriberDeserialize(quint16 messageType,
                                             QDataStream &stream) {

    switch (messageType) {
    case BLSTARTUP_INTIALIZATION: {
        Message *message = new InitializationMessage();
        message->deserialize(stream);
        return MessagePtr(message);
        qDebug() << "WifiCBusComm subscriberDeserialize - BLSTARTUP_INTIALIZATION";
    }
    case ConnectWifiMessageMacro:
    {
        Message *message = new ConnectWifiMessage();
        message->deserialize(stream);
        return MessagePtr(message);
        qDebug() << "WifiCBusComm subscriberDeserialize - ConnectWifiMessage";
    }
    case DisconnectWifiMessageMacro:
    {
        Message *message = new DisconnectWifiMessage();
        message->deserialize(stream);
        return MessagePtr(message);
        qDebug() << "WifiCBusComm subscriberDeserialize - DisconnectWifiMessage";
    }
    default:
        break;
    }
    return MessagePtr();
}

//-------------------------------------------------------
// Handle incoming Startup service messages
//-------------------------------------------------------
bool WifiCBusComm::subscriberMessageHandler(MessagePtr message) {

    switch (message->messageType) {
    case BLSTARTUP_INTIALIZATION: {
           QSharedPointer<InitializationMessage const> request =
             message.staticCast<InitializationMessage const>();
        emit startupService(request);
        return true;
        qDebug() << "WifiCBusComm subscriberMessageHandler - BLSTARTUP_INTIALIZATION";

    }
    case ConnectWifiMessageMacro:
    {
        QSharedPointer<ConnectWifiMessage const> request = message.staticCast<ConnectWifiMessage const>();
        qDebug() << "WifiCBusComm subscriberMessageHandler - ConnectWifiMessage";
        if(!request.isNull())
            emit connectWifi();
        return true;
    }
    case DisconnectWifiMessageMacro:
    {
        QSharedPointer<DisconnectWifiMessage const> request = message.staticCast<DisconnectWifiMessage const>();
        qDebug() << "WifiCBusComm subscriberMessageHandler - DisconnectWifiMessage";
        if(!request.isNull())
            emit disconnectWifi();
        return true;
    }
    break;

    default:
        break;
    }
    return false;
}

//-----------------------------------------------------------
// Forward calls to CBus
//-----------------------------------------------------------
void WifiCBusComm::sendReply(quint16 requestId, MessagePtr const &message) {
    cbus.sendReply(requestId, message);
}

bool WifiCBusComm::callService(quint8 serviceName, MessagePtr const &message) {
    return cbus.callService(serviceName, message);
}

void WifiCBusComm::push(MessagePtr const &message) { cbus.push(message); }
