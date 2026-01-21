#ifndef CANSERVICEDELEGATE_H
#define CANSERVICEDELEGATE_H

#include <QObject>
#include <WifiSignalStrengthMessage.h>
#include <gsmservicemessages.h>
#include <connectionmanagerservicemessages.h>

class CanServiceClient;

class CanserviceDelegate : public QObject
{
    Q_OBJECT

public:
    /**
    * This method will return a reference to a Canservicedelegate Object.
    */
    static CanserviceDelegate &getInstance();

public slots:

    void WriteBatteryBin(const QVariant msgId, const QVariant batteryId);

signals:

    /** Signal which send back the reponse if config data is written */
    void writeConfigDataResponse(bool written);


    void WifiSignalStrength_Signal(QSharedPointer<const WifiSignalStrengthMessage> WifiSignalStrengthMessageValue);

    void gsmSignalStrength_Signal(QSharedPointer<const GsmSignalStrengthUpdate> GsmSignalStrengthUpdateValue);

    void connectWifiMessage_Signal(QSharedPointer<const ConnectWifiMessage> ConnectWifiMessageValue);

    void disconnectWifiMessage_signal(QSharedPointer<const DisconnectWifiMessage> DisconnectWifiMessageValue);

private:

    const QScopedPointer<CanServiceClient> canServiceClient;

public:
    CanserviceDelegate();
    virtual ~CanserviceDelegate();
    Q_DISABLE_COPY(CanserviceDelegate);


};

#endif // CANSERVICEDELEGATE_H
