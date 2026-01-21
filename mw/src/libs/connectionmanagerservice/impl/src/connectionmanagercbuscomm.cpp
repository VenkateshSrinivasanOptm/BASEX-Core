#include "connectionmanagercbuscomm.h"
#include "cbusinterface.h" // communication library
#include <comlibservices.h>
#include <connectionmanagerservicemessages.h>
#include <QDebug>
#include <QTimer>
#include <appmanif.h>
#include <connectionmanagerservice.h>
#include <initializationmessage.h>
#include <comlibservices.h>
#include <WifiSignalStrengthMessage.h>
#include <gsmservicemessages.h>

ConnectionManagerCBusComm::ConnectionManagerCBusComm() : cbus(CBusInterface::instance()) , m_gsmSignalStrength(-1),
    m_isWifiConnected(false),m_currentConnectionMgrState(GSM_CONNECTED)
{
    QList<quint16> messages;
    messages  << GSM_SIGNAL_UPDATE << WifiSignalStrengthMessageMacro << GSM_SIGNAL_UPDATE;

    cbus.subscribe(this, messages);

}

ConnectionManagerCBusComm::~ConnectionManagerCBusComm()
{
    cbus.unregisterService(CONNECTIONMANAGER_SERV);
}

void ConnectionManagerCBusComm::initialise()
{
    connect(AppManIf::getInstance(), SIGNAL(establishCommunication()), this, SLOT(establishCommunication()));
    connect(AppManIf::getInstance(), SIGNAL(notifyStateChange(StateChange)), this, SLOT(handlePowerRestartNotification(StateChange)));
    connect(AppManIf::getInstance(), SIGNAL(heartbeatNotification()), this, SLOT(heartbeat()));

    AppManIf::getInstance()->notifyPublished(MWNAMESTR, MSVERSTR);
    qDebug() << "AppManIf::getInstance()->notifyPublished ConnectionManagerservice";

    QTimer::singleShot(REGISTER_SERV_DELAY,[this]()
    {
        if(!cbus.registerService(CONNECTIONMANAGER_SERV, this, this))
        {
            qDebug() << "CONNECTIONMANAGER_SERV Already regitered";
        }
        else
            qDebug() << "CONNECTIONMANAGER_SERV is regitered successfully";

        QList<quint16> messages;
        messages << BLSTARTUP_INTIALIZATION << WifiSignalStrengthMessageMacro << GSM_SIGNAL_UPDATE;
        cbus.subscribe(this, messages);

        QTimer::singleShot(300000, this,[this]()
        {
           qDebug() << "Timer got timed out to check signal strenth for 5 min , if its still didnt turn of , we will connect wifi";
           qDebug() << "Signal strength of GSM : " << m_gsmSignalStrength;
           if(m_gsmSignalStrength == -1)
               connectWifi();
        });
    });
}

void ConnectionManagerCBusComm::establishCommunication(void)
{
    qDebug() <<__FUNCTION__ ;
    // Subscribe

    // subscribe for events from the application business logic startup component (stefan)
    // currently already done in initialise
    AppManIf::getInstance()->communicationEstablished();   //Subscribed

    // STARTUP COMPLETE.
}

void ConnectionManagerCBusComm::heartbeat()
{
    qDebug() <<__FUNCTION__ ;
    AppManIf::getInstance()->sendHeartbeatResponse();
}

void ConnectionManagerCBusComm::handlePowerRestartNotification(StateChange state)
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

MessagePtr ConnectionManagerCBusComm::deserialize(quint16 messageType, QDataStream &stream)
{
    Message *message(0);
    qDebug() << __FUNCTION__ << messageType;
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

bool ConnectionManagerCBusComm::messageHandler(quint16 requestId, MessagePtr message)
{
    qDebug() << __FUNCTION__ << " request id" << requestId;
    switch (message->messageType)
    {

    }
    return false;
}

bool ConnectionManagerCBusComm::messageHandler(MessagePtr message)
{
    qDebug() << __FUNCTION__ << message->messageType;

    switch (message->messageType)
    {

    }
    return false;
}

//-------------------------------------------------------
// Deserialize Startup service messages
//-------------------------------------------------------
MessagePtr ConnectionManagerCBusComm::subscriberDeserialize(quint16 messageType,
                                             QDataStream &stream)
{
    switch (messageType)
    {
        case BLSTARTUP_INTIALIZATION:
        {
            Message *message = new InitializationMessage();
            message->deserialize(stream);
            return MessagePtr(message);
            qDebug() << "DB Service subscriberDeserialize - BLSTARTUP_INTIALIZATION";
        }
        case WifiSignalStrengthMessageMacro:
        {
            Message *message = new WifiSignalStrengthMessage();
            message->deserialize(stream);
            return MessagePtr(message);
            qDebug() << "subscriberDeserialize - WifiSignalStrengthMessageMacro";
        }
        case GSM_SIGNAL_UPDATE:
        {
            Message *message = new GsmSignalStrengthUpdate();
            message->deserialize(stream);
            return MessagePtr(message);
            qDebug() << "subscriberDeserialize - GsmSignalStrengthUpdate";
        }
    default:
        break;
    }
    return MessagePtr();
}
void ConnectionManagerCBusComm::connectWifi()
{
    m_isWifiConnected = true;
    qDebug() << "Connecting to Wi-Fi...";

    MessagePtr ConnectWifiMessageValue = QSharedPointer<ConnectWifiMessage>(new ConnectWifiMessage());
    cbus.push(ConnectWifiMessageValue);
    qDebug() << "ConnectWifiMessage pushed to CBus successfully.";

    qDebug() << "Connecting to Wi-Fi called successfully.";
}

void ConnectionManagerCBusComm::disconnectWifi()
{
    m_isWifiConnected = false;
    qDebug() << "Disconnecting from Wi-Fi...";

    QSharedPointer<DisconnectWifiMessage> DisconnectWifiMessageValue = QSharedPointer<DisconnectWifiMessage>(new DisconnectWifiMessage());
    cbus.push(DisconnectWifiMessageValue);
    qDebug() << "Disconnecting from Wi-Fi called successfully.";
}

void ConnectionManagerCBusComm::handleGsmSignalStrength(int gsmSignalStrength)
{
    m_gsmSignalStrength = gsmSignalStrength;
    qDebug() << "Current GSM Signal Strength : " << gsmSignalStrength;
    qDebug() << "m_currentConnectionMgrState : " << m_currentConnectionMgrState;
    qDebug() << "m_isWifiConnected : " << m_isWifiConnected;

    if (gsmSignalStrength >= 31 && gsmSignalStrength <= 98) // Excellent signal
    {
        qDebug() << "GSM Signal is in the 'Excellent' range (31 - 98).";
        if (m_currentConnectionMgrState == WIFI_CONNECTED)
        {
            if(m_isWifiConnected)
            {
                disconnectWifi();
                qDebug() << "Switched to GSM due to excellent signal strength.";
                m_currentConnectionMgrState = GSM_CONNECTED;
            }
        }
    }
    else if (gsmSignalStrength >= 20 && gsmSignalStrength <= 30)    // Very good signal
    {
        qDebug() << "GSM Signal is in the 'Very Good' range (20 - 30).";
        if (m_currentConnectionMgrState == WIFI_CONNECTED)
        {
            if(m_isWifiConnected)
            {
                disconnectWifi();
                qDebug() << "Switched to GSM due to very good signal strength.";
                m_currentConnectionMgrState = GSM_CONNECTED;
            }
        }
    }
    else if (gsmSignalStrength >= 15 && gsmSignalStrength <= 19)    // good signal
    {
        qDebug() << "GSM Signal is in the 'Fair' range (15 - 19).";
        if (m_currentConnectionMgrState == WIFI_CONNECTED)
        {
            if(m_isWifiConnected)
            {
                disconnectWifi();
                qDebug() << "Switched to GSM due to very good signal strength.";
                m_currentConnectionMgrState = GSM_CONNECTED;
            }
        }
    }
    else if (gsmSignalStrength >= 10 && gsmSignalStrength <= 14)    // good signal
    {
        qDebug() << "GSM Signal is in the 'Very Fair' range (10 - 14).";
        if (m_currentConnectionMgrState == WIFI_CONNECTED)
        {
            if(m_isWifiConnected)
            {
                disconnectWifi();
                qDebug() << "Switched to GSM due to very good signal strength.";
                m_currentConnectionMgrState = GSM_CONNECTED;
            }
        }
    }
    else if (gsmSignalStrength == 99)   // Not detectable or unknown
    {
        qDebug() << "GSM Signal is 'Not Detectable' or 'Unknown' (99).";
        if (m_currentConnectionMgrState == GSM_CONNECTED)
        {
            if(!m_isWifiConnected)
            {
                connectWifi();
                qDebug() << "Switched to Wi-Fi due to GSM signal being not detectable or unknown.";
                m_currentConnectionMgrState = WIFI_CONNECTED;
            }
        }
    }
    else    // Poor or weak signal (0 to 9)
    {
        qDebug() << "GSM Signal is in the 'Poor' or 'Weak' range (0 - 19).";
        if (m_currentConnectionMgrState == GSM_CONNECTED)
        {
            if(!m_isWifiConnected)
            {
                connectWifi();
                qDebug() << "Switched to Wi-Fi due to poor GSM signal.";
                m_currentConnectionMgrState = WIFI_CONNECTED;
            }
        }
    }
}

void ConnectionManagerCBusComm::handleWifiSignalStrength(int wifiSignalStrength)
{
    m_wifiSignalStrength = wifiSignalStrength;
    qDebug() << "Wi-Fi Signal Strength: " << wifiSignalStrength;

    if (wifiSignalStrength == 4)  // Excellent signal
    {
        qDebug() << "Wi-Fi Signal is in the 'Excellent' range.";
    }
    else if (wifiSignalStrength == 3)  // Good signal
    {
        qDebug() << "Wi-Fi Signal is in the 'Good' range.";
    }
    else if (wifiSignalStrength == 2)  // Fair signal
    {
        qDebug() << "Wi-Fi Signal is in the 'Fair' range.";
    }
    else  // Poor signal
    {
        qDebug() << "Wi-Fi Signal is in the 'Poor' range.";
    }
}

//-------------------------------------------------------
// Handle incoming Startup service messages
//-------------------------------------------------------
bool ConnectionManagerCBusComm::subscriberMessageHandler(MessagePtr message)
{
    switch (message->messageType)
    {
        case BLSTARTUP_INTIALIZATION:
        {
            QSharedPointer<InitializationMessage const> request =
                message.staticCast<InitializationMessage const>();
            emit startupService(request);
            qDebug() << "ConnectionManager service subscriberMessageHandler - BLSTARTUP_INTIALIZATION";
            return true;
        }
        case WifiSignalStrengthMessageMacro:
        {
            QSharedPointer<WifiSignalStrengthMessage const> WifiSignalStrengthMessageValue = message.staticCast<WifiSignalStrengthMessage const>();
            qDebug() << "Received WifiSignalStrengthMessage in ConnectionManagerCBusComm";

            int wifiSignalStrength = WifiSignalStrengthMessageValue.get()->m_signalStrength;
            handleWifiSignalStrength(wifiSignalStrength);

            return true;
        }
        case GSM_SIGNAL_UPDATE:
        {
            QSharedPointer<GsmSignalStrengthUpdate const> GsmSignalStrengthUpdateValue = message.staticCast<GsmSignalStrengthUpdate const>();
            qDebug() << "Received GsmSignalStrengthUpdate.";

            int gsmSignalStrength = GsmSignalStrengthUpdateValue->data;

            qDebug() << "GSM Signal Strength (converted): " << gsmSignalStrength;
            handleGsmSignalStrength(gsmSignalStrength);

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
void ConnectionManagerCBusComm::sendReply(quint16 requestId, MessagePtr const &message)
{
    cbus.sendReply(requestId, message);
}

bool ConnectionManagerCBusComm::callService(quint8 serviceName, MessagePtr const &message)
{
    return cbus.callService(serviceName, message);
}

void ConnectionManagerCBusComm::push(MessagePtr const &message)
{
    cbus.push(message);
}
