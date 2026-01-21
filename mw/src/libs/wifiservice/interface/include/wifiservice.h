#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include "WifiSignalStrengthMessage.h"
#include <initializationmessage.h>

#include <QObject>
#include <QProcess>
#include <QTimer>

#define TESTING
#define MWNAMESTR "wifiservice"
#define MSVERSTR "1.0"

class WifiCBusComm;

class Wifiservice : public QObject
{
    Q_OBJECT
public:
    static Wifiservice& getInstance();

    /* initialisation method for startup sequence */
    void initialise(void);

    void connectWifi();
    void disconnectWifi();

public slots:
    void processStart(QSharedPointer<InitializationMessage const>);
    void updateWifiSSIDAndPassword_Slot();
    void analyseNetworkStatus();

    void onConnectWifi();
    void onDisConnectWifi();

private slots:
    void processOutput();
    void processError();
private:
    Wifiservice();
    virtual ~Wifiservice();
    Q_DISABLE_COPY(Wifiservice);
    QSharedPointer<WifiCBusComm> m_CBusComm;

    QString m_WIFI_MACID;
    QString m_WIFI_PASSWORD;
    QString m_WIFI_SSID;
    QTimer m_fetchNetworkStatusTimer;
    int m_signalStrength;
    int m_retryCount;
    bool m_initialConnect;
    QProcess m_wpaSupplicantProcess;

    bool enableDebugLogs = true;

    void killWpaSupplicant();
    void startWpaSupplicant();
    void startNetworkChecks();
    bool unblockWifiDevices();
    void bringInterfaceDown();

    void printConnectedSSID(QStringList lines);
    void fetchSignalStrength(QStringList lines);
    void checkNetworkInterfaces(const QString &command);
    void updateWpaSupplicantConfig();
    bool executeCommandUsingProcess(QString command);
    void executeCommandWithDebug(QString command);
    void changePermissionsWithChmod(const QString &filePath);
    void restartWifi();
    void setIPAndGateway();
};

#endif // WIFISERVICE_H
