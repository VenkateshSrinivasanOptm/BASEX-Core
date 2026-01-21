#include "CanserviceDelegate.h"
#include "CanServiceClient.h"
#include <QDebug>
#include <QVariant>

bool enableDebugLogs = false;
#define CONDITIONAL_DEBUG if (enableDebugLogs) qDebug()

CanserviceDelegate::CanserviceDelegate() : canServiceClient(new CanServiceClient)
{
    CONDITIONAL_DEBUG << "CanserviceDelegate Constructor";
    connect(canServiceClient.get(), &CanServiceClient::WifiSignalStrength_Signal, this, &CanserviceDelegate::WifiSignalStrength_Signal);
    connect(canServiceClient.get(), &CanServiceClient::gsmSignalStrength_Signal, this, &CanserviceDelegate::gsmSignalStrength_Signal);
    connect(canServiceClient.get(), &CanServiceClient::connectWifiMessage_Signal, this, &CanserviceDelegate::connectWifiMessage_Signal);
    connect(canServiceClient.get(), &CanServiceClient::disconnectWifiMessage_signal, this, &CanserviceDelegate::disconnectWifiMessage_signal);
}

CanserviceDelegate::~CanserviceDelegate()
{

}

CanserviceDelegate &CanserviceDelegate::getInstance()
{
    CONDITIONAL_DEBUG << __FUNCTION__;
    static CanserviceDelegate instance;
    return instance;
}

void CanserviceDelegate::WriteBatteryBin(const QVariant msgId,const QVariant batteryId)
{
    // canServiceClient->WriteBatteryBin(msgId,batteryId);
}
