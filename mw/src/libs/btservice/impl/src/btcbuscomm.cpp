#include "btcbuscomm.h"
#include "cbusinterface.h" // communication library
#include <comlibservices.h>
#include <QDebug>
#include <QTimer>
#include <appmanif.h>
#include <btservice.h>

BtCBusComm::BtCBusComm() : cbus(CBusInterface::instance()) {}

void BtCBusComm::initialise() {

    connect(AppManIf::getInstance(), SIGNAL(establishCommunication()), this, SLOT(establishCommunication()));
    connect(AppManIf::getInstance(), SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(AppManIf::getInstance(), SIGNAL(heartbeatNotification()), this, SLOT(heartbeat()));

    AppManIf::getInstance()->notifyPublished(MWNAMESTR, MSVERSTR);
    qDebug() << "Madhur AppManIf::getInstance()->notifyPublished dbservice";

    QTimer::singleShot(REGISTER_SERV_DELAY,[this](){
        if(!cbus.registerService(BT_SERV, this, this))
        {
            qDebug() << "BT_SERV Already regitered";
        }
        QList<quint16> messages;
        messages << BLSTARTUP_INTIALIZATION;
        cbus.subscribe(this, messages);
    });
}

void BtCBusComm::establishCommunication(void)
{
    qDebug() <<__FUNCTION__ ;
    // Subscribe

    // subscribe for events from the application business logic startup component (stefan)
    // currently already done in initialise
    AppManIf::getInstance()->communicationEstablished();   //Subscribed

    // STARTUP COMPLETE.
}

void BtCBusComm::heartbeat() {
    qDebug() <<__FUNCTION__ ;
    AppManIf::getInstance()->sendHeartbeatResponse();
}

void BtCBusComm::handlePowerRestartNotification(StateChange state)
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

void BtCBusComm::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__ ;
}

BtCBusComm::~BtCBusComm() { cbus.unregisterService(BT_SERV); }

MessagePtr BtCBusComm::deserialize(quint16 messageType, QDataStream &stream) {

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

bool BtCBusComm::messageHandler(quint16 requestId, MessagePtr message) {
    qDebug() << __FUNCTION__ << " request id" << requestId;
    switch (message->messageType) {

    default:
        break;
    }
    return false;
}
bool BtCBusComm::messageHandler(MessagePtr message) {
    qDebug() << __FUNCTION__ << message->messageType;
    switch (message->messageType) {
    break;
    default:
        break;
    }
    return false;
}
//-------------------------------------------------------
// Deserialize Startup service messages
//-------------------------------------------------------
MessagePtr BtCBusComm::subscriberDeserialize(quint16 messageType,
                                             QDataStream &stream) {

    switch (messageType) {
    case BLSTARTUP_INTIALIZATION: {
        Message *message = new InitializationMessage();
        message->deserialize(stream);
        return MessagePtr(message);

    }
    default:
        break;
    }
    return MessagePtr();
}

//-------------------------------------------------------
// Handle incoming Startup service messages
//-------------------------------------------------------
bool BtCBusComm::subscriberMessageHandler(MessagePtr message) {

    switch (message->messageType) {
    case BLSTARTUP_INTIALIZATION: {
        QSharedPointer<InitializationMessage const> request =
            message.staticCast<InitializationMessage const>();
        emit startupService(request);
        return true;
    }

    default:
        break;
    }
    return false;
}
//-----------------------------------------------------------
// Forward calls to CBus
//-----------------------------------------------------------
void BtCBusComm::sendReply(quint16 requestId, MessagePtr const &message) {
    cbus.sendReply(requestId, message);
}

bool BtCBusComm::callService(quint8 serviceName, MessagePtr const &message) {
    return cbus.callService(serviceName, message);
}

void BtCBusComm::push(MessagePtr const &message) { cbus.push(message); }
