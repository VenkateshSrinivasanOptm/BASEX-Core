#ifndef CANSERVICECLIENT_H
#define CANSERVICECLIENT_H

#include <QObject>
#include <cbusinterface.h>
#include <subscriberIntf.h>
#include <WifiSignalStrengthMessage.h>
#include <gsmservicemessages.h>
#include <connectionmanagerservicemessages.h>

#define MWNAMESTR "hmiservice"
#define MSVERSTR "1.0"

class CanServiceClient : public QObject, public SubscriberIntf
{
    Q_OBJECT

public:

    CanServiceClient();
    virtual ~CanServiceClient(){};

    MessagePtr deserialize(quint16 messageType, QDataStream &data);

    bool callback(quint16 requestId, MessagePtr reply);

    MessagePtr subscriberDeserialize(quint16 messageType,QDataStream &stream) override;

    bool subscriberMessageHandler(MessagePtr message) override;

    // void WriteBatteryBin(const QVariant msgId, const QVariant batteryId);

signals:
    void WifiSignalStrength_Signal(QSharedPointer<const WifiSignalStrengthMessage> WifiSignalStrengthMessageValue);

    void gsmSignalStrength_Signal(QSharedPointer<const GsmSignalStrengthUpdate> GsmSignalStrengthUpdateValue);

    void connectWifiMessage_Signal(QSharedPointer<const ConnectWifiMessage> ConnectWifiMessageValue);

    void disconnectWifiMessage_signal(QSharedPointer<const DisconnectWifiMessage> DisconnectWifiMessageValue);

private:
    CBusInterface &cbus;
};

#endif // CANSERVICECLIENT_H
