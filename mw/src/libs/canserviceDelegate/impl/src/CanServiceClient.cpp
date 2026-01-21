#include "CanServiceClient.h"
#include <QDebug>
#include <callbackgen.h>
#include <comlibservices.h>

bool enableDebugLogs1 = false;
#define CONDITIONAL_DEBUG if (enableDebugLogs1) qDebug()

CanServiceClient::CanServiceClient() : cbus(CBusInterface::instance())
{
    CONDITIONAL_DEBUG << "CanServiceClient constructor";
    QList<quint16> messages;
    messages << BATTEY_BIN_ID_MessageMacro
        << BSS_Batt_Chg_BIN_Lock_State_MessageMacro
        << BSS_BattChgRemainTimeMessageMacro
        << BSS_BattSocEstPercMessageMacro
        << BSS_BattStateMessageMacro
        << BSS_BattEnergyMeterAverageCurrentMessageMacro
        << BSS_BattEnergyMeterCurrentMessageMacro
        << BSS_EnergyMeterTotalKiloWattMessageMacro
        << BSS_EnergyMeterVoltageMessageMacro
        << BSS_HVAC_Electrical_Param_MessageMacro
        << BSS_HVAC_FAN_Param_MessageMacro
        << BSS_HVAC_SW_Ver_Unit_MessageMacro
        << BSS_HVAC_Temp_Param_MessageMacro
        << BSS_Sen_Resp_MessageMacro
        << BSS_SensorInterface_Stat_MessageMacro
        << BSS_TempHumiditySensor_MessageMacro
        << BSS_EnergyMeterKiloVoltAmpereMessageMacro
        << BSS_EnergyMeterKiloWattMessageMacro
        << BSS_EnergyMeterParamMessageMacro
        << BSS_ChgConfig_MessageMacro
        << BSS_Control_Param_MessageMacro
        << BSS_HVAC_Control_Param_MessageMacro
        << BSS_OTA_Req_MessageMacro
        << BSS_Sen_ControlComm_MessageMacro
        << WifiSignalStrengthMessageMacro
        << GSM_SIGNAL_UPDATE
        << ConnectWifiMessageMacro
        << DisconnectWifiMessageMacro
        << COMM_CHG_Param_MessageMacro
        << BLE_CONNECTION_STATUS_UPDATE;

    foreach (quint16 msg, messages) {
        CONDITIONAL_DEBUG << "Msg : " << msg;
    }

    cbus.subscribe(this, messages);
}


MessagePtr CanServiceClient::subscriberDeserialize(quint16 messageType,QDataStream &stream)
{
    CONDITIONAL_DEBUG << __FUNCTION__ << " incoming message " << messageType;
    Message *message(0);
    switch (messageType)
    {
        case WifiSignalStrengthMessageMacro:
            message = new WifiSignalStrengthMessage();
            CONDITIONAL_DEBUG << "created WifiSignalStrengthMessage in CanserviceDelegate";
            break;
        case GSM_SIGNAL_UPDATE:
            message = new GsmSignalStrengthUpdate();
            CONDITIONAL_DEBUG << "created GsmSignalStrengthUpdate in CanserviceDelegate";
            break;
        case ConnectWifiMessageMacro:
            message = new ConnectWifiMessage();
            CONDITIONAL_DEBUG << "created ConnectWifiMessage in CanserviceDelegate";
            break;
        case DisconnectWifiMessageMacro:
            message = new DisconnectWifiMessage();
            CONDITIONAL_DEBUG << "created DisconnectWifiMessage in CanserviceDelegate";
            break;
        default:
            message = nullptr; // In case message type doesn't match any known type
            break;
    }
    if (message == NULL)
    {
        CONDITIONAL_DEBUG << __FUNCTION__
                 << " DID YOU NOT FORGET TO IMPLEMENT THE DESERIALIZE FOR THIS "
                    "TYPE OF MSG ? ";
        return MessagePtr();
    }
    else
    {
        message->deserialize(stream);
        return MessagePtr(message);
    }
}

bool CanServiceClient::callback(quint16 requestId, MessagePtr reply)
{
    CONDITIONAL_DEBUG << __FUNCTION__ << " callback received for requestId " << requestId
             << ", message type " << reply->messageType;

    switch (reply->messageType)
    {
    // TODO return true if messageType is captured
    default:
        break;
    }
    return false;
}

MessagePtr CanServiceClient::deserialize(quint16 messageType,QDataStream &data)
{
    CONDITIONAL_DEBUG << __FUNCTION__ << " starting deserialising for message type "
             << messageType;

    Message *message(0);
    switch (messageType)
    {
    // TODO create response andassign to message
    default:
        break;
    }
    if (message == NULL)
    {
        CONDITIONAL_DEBUG << __FUNCTION__
                 << " DID YOU NOT FORGET TO IMPLEMENT THE DESERIALIZE FOR THIS "
                    "TYPE OF MSG ? ";
        return MessagePtr();
    }
    else
    {
        message->deserialize(data);
        return MessagePtr(message);
    }
}

bool CanServiceClient::subscriberMessageHandler(MessagePtr message)
{
    CONDITIONAL_DEBUG << __FILE__ << __FUNCTION__ << " incoming message " << message->messageType; switch (message->messageType)
    {
        case WifiSignalStrengthMessageMacro:
        {
            QSharedPointer<WifiSignalStrengthMessage const> request = message.staticCast<WifiSignalStrengthMessage const>();
            CONDITIONAL_DEBUG << "Received WifiSignalStrengthMessage in CanserviceDelegate.";
            emit WifiSignalStrength_Signal(request);
            return true;
        }
        case GSM_SIGNAL_UPDATE:
        {
            QSharedPointer<GsmSignalStrengthUpdate const> request = message.staticCast<GsmSignalStrengthUpdate const>();
            CONDITIONAL_DEBUG << "Received GsmSignalStrengthUpdate in CanserviceDelegate.";
            emit gsmSignalStrength_Signal(request);
            return true;
        }
        case ConnectWifiMessageMacro:
        {
            QSharedPointer<ConnectWifiMessage const> request = message.staticCast<ConnectWifiMessage const>();
            CONDITIONAL_DEBUG << "Received ConnectWifiMessage in CanserviceDelegate.";
            emit connectWifiMessage_Signal(request);
            return true;
        }
        case DisconnectWifiMessageMacro:
        {
            QSharedPointer<DisconnectWifiMessage const> request = message.staticCast<DisconnectWifiMessage const>();
            CONDITIONAL_DEBUG << "Received DisconnectWifiMessage in CanserviceDelegate.";
            emit disconnectWifiMessage_signal(request);
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

// void CanServiceClient::WriteBatteryBin(const QVariant msgId,const QVariant batteryId)
// {
//     // send message to db service.
//     CONDITIONAL_DEBUG << __FUNCTION__ << " sending tune message over cbus";
//     Message *message = new BatteryBinIDMessage(msgId,batteryId);
//     QSharedPointer<CallBack> callBack =
//         QSharedPointer<CallBack>(new CallBackGen<CanServiceClient>(
//             this, &CanServiceClient::callback, &CanServiceClient::deserialize));
//     quint16 resultr =
//         cbus.callServiceWithCallback(CAN_SERV_DELEGATE, MessagePtr(message), callBack);
//     CONDITIONAL_DEBUG << __FUNCTION__ << " cbus returns: " << resultr;
// }
