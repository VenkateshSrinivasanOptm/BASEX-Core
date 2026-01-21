#include "canservice.h"
#include "cbusinterface.h" // communication library
#include <comlibservices.h>
#include "canlib.h"
#include "canparser.h"
#include <QDebug>
#include <QThread>
#include "callbackgen.h"
#include <QTimer>
#include <appmanif.h>
#include "initializationmessage.h"

Q_DECLARE_METATYPE (QSharedPointer<InitializationMessage const>)

#define BATTERY_SLOT_COUNT_9 9
#define BATTERY_SLOT_COUNT_12 12
#define BATTERY_SLOT_COUNT_15 15

#define BATTERY_SLOT_NO_0    0
#define BATTERY_SLOT_NO_1    1
#define BATTERY_SLOT_NO_2    2
#define BATTERY_SLOT_NO_3    3
#define BATTERY_SLOT_NO_8    8
#define BATTERY_SLOT_NO_9    9
#define BATTERY_SLOT_NO_19   19

#include <QByteArray>
#include <cstdint>

bool enableDebugLogs = false;

Canservice::Canservice():cBus(CBusInterface::instance()), m_canParser(Canlib::getInstance())
{
    QList<quint16> messages;

    cBus.subscribe(this, messages);

    connect(this, SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));

    // read the config.json file and keep it in a struct
}

Canservice::~Canservice()
{
    cBus.unregisterService(CAN_SERV);
}

void Canservice::writeCANMessage(QByteArray canData, uint canMsg)
{
    if(enableDebugLogs)
        qDebug()<< "Print final payload after preparing the can data to write the max output current values for battery slot  00 to 06";
    for (int i = 0; i < canData.size(); ++i)
    {
        char byte = canData[i];
        QString binaryString;
        for (int bit = 0; bit < 8; ++bit)
        {
            // Changed to iterate from 0 to 7
            binaryString.append((byte & (1 << (7 - bit))) ? '1' : '0');  // Adjusted the bit position
        }
        if(enableDebugLogs)
            qDebug()<< "Byte" << i << ":" << binaryString;
    }
    if(enableDebugLogs)
        qDebug()<< "end";

    bool writeSuccess = m_canParser.getCanParser().writeFrameCanDeviceOne(canMsg, canData);

    if (writeSuccess)
    {
        if(enableDebugLogs)
            qDebug()<< "CAN frame written successfully for configuration:" << QString::number(canMsg, 16).toUpper();
        if(enableDebugLogs)
            qDebug()<< "Data written:" << canData.toHex();
    }
    else
    {
        if(enableDebugLogs)
            qDebug()<< "Failed to write CAN frame for configuration:" << QString::number(canMsg, 16).toUpper();
        if(enableDebugLogs)
            qDebug()<< "Data attempted to write:" << canData.toHex();
    }
}


Canservice &Canservice::getInstance()
{
    static Canservice canservice;
    return canservice;
}

void Canservice::processStart(QSharedPointer<InitializationMessage const> msg)
{
    //todo do starup logic here
}


void Canservice::initialise()
{
    connect(AppManIf::getInstance(), SIGNAL(establishCommunication()), this, SLOT(establishCommunication()));
    connect(AppManIf::getInstance(), SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(AppManIf::getInstance(), SIGNAL(heartbeatNotification()), this, SLOT(heartbeat()));

    if(enableDebugLogs)
        qDebug()<< "Madhur AppManIf::getInstance()->notifyPublished canservice";
    AppManIf::getInstance()->notifyPublished(MWNAMESTR, MSVERSTR);

    /** Register service */
    QTimer::singleShot(REGISTER_SERV_DELAY,[this]()
   {
       if(!cBus.registerService(CAN_SERV, this, this))
       {
           if(enableDebugLogs)
               qDebug()<< "CAN_SERV Already regitered";
       }
       QList<quint16> messages;
       messages << BLSTARTUP_INTIALIZATION;
       cBus.subscribe(this, messages);
   });
}

void Canservice::establishCommunication(void)
{
    if(enableDebugLogs)
        qDebug()<<__FUNCTION__ ;
    // Subscribe

    // subscribe for events from the application business logic startup component (stefan)
    // currently already done in initialise
    AppManIf::getInstance()->communicationEstablished();   //Subscribed

    // STARTUP COMPLETE.
}

void Canservice::heartbeat() {
    if(enableDebugLogs)
        qDebug()<<__FUNCTION__ ;
    AppManIf::getInstance()->sendHeartbeatResponse();
}

void Canservice::handlePowerRestartNotification(StateChange state)
{
    if(enableDebugLogs)
        qDebug()<<  __FUNCTION__;
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

bool Canservice::callback(quint16 requestId, MessagePtr reply)
{
    return true;
}

MessagePtr Canservice::deserialize(quint16 messageType, QDataStream &stream)
{
    Message *message(0);

    switch (messageType)
    {
    }

    if (message == NULL)
    {
        return MessagePtr();
    }
    else
    {
        message->deserialize(stream);
        return MessagePtr(message);
    }
}

bool Canservice::messageHandler(quint16 requestId, MessagePtr message)
{
    switch (message->messageType)
    {
    /*case DB_SERV_WRITE_CONFIG_REQ:
         * {
        QSharedPointer<WriteConfigRequest const> request =
            message.staticCast<WriteConfigRequest const>();
        emit writeConfig(requestId, DB_SERV_WRITE_CONFIG_REQ, request->data);
        return true;
        }*/
    default:
        break;
    }
    return false;
}


bool Canservice::messageHandler(MessagePtr message)
{
    if(enableDebugLogs)
        qDebug()<< " request reached ";
    switch(message->messageType)
    {
        default:
            break;
    }
    return false;
}

//-------------------------------------------------------
// Deserialize Startup service messages
//-------------------------------------------------------
MessagePtr Canservice::subscriberDeserialize(quint16 messageType,
                                             QDataStream &stream)
{
    switch (messageType)
    {
    case BLSTARTUP_INTIALIZATION:
    {
        Message *message = new InitializationMessage();
        message->deserialize(stream);
        if(enableDebugLogs)
            qDebug()<< "Madhur CAN Service subscriberDeserialize - BLSTARTUP_INTIALIZATION";
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
bool Canservice::subscriberMessageHandler(MessagePtr message)
{
    switch (message->messageType)
    {
    case BLSTARTUP_INTIALIZATION:
    {
        if(enableDebugLogs)
            qDebug()<< "Madhur CAN Service subscriberMessageHandler - BLSTARTUP_INTIALIZATION";
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
void Canservice::sendReply(quint16 requestId, MessagePtr const &message)
{
    cBus.sendReply(requestId, message);
}

bool Canservice::callService(quint8 serviceName, MessagePtr const &message)
{
    if(enableDebugLogs)
        qDebug()<< __FUNCTION__ << serviceName;
    return cBus.callService(serviceName, message);
}
