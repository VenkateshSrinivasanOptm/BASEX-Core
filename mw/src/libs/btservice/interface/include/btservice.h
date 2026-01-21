#ifndef BTSERVICE_H
#define BTSERVICE_H
#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include <QSerialPort>
#include <initializationmessage.h>
#include <bluetoothmanager.h>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyAdvertisingData>
#include <QLowEnergyAdvertisingParameters>
#include <QBluetoothUuid>
#include <QTimer>
#include <QLowEnergyServiceData>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyDescriptorData>

#define TESTING
#define MWNAMESTR "btservice"
#define MSVERSTR "1.0"

class BtCBusComm;
class Btservice : public QObject {
    Q_OBJECT
public:
    static Btservice &getInstance();
    /* initialisation method for startup sequence */
    void initialise(void);
    void initSerialPort();
    uint64_t get64BitValue(uint8_t* IMEI);
    uint64_t get64BitValueFrom48bits(uint8_t* BLE_MACID);
    void get64BitValueFrom70bits(uint8_t* BLE_MACID, uint8_t& upperBits, uint64_t& lowerBits);
    void InitBLE();

public slots:
    void readData();
    void writeData();
    void handleError();
    void processStart(QSharedPointer<InitializationMessage const>);

private:
    Btservice();
    virtual ~Btservice();
    Q_DISABLE_COPY(Btservice);
    QSharedPointer<BtCBusComm> m_CBusComm;
    BluetoothManager manager;
    QLowEnergyController* leController;
    QSharedPointer<QLowEnergyService> service;
};

#endif // BTSERVICE_H
