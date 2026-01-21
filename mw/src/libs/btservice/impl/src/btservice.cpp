#include "btservice.h"
#include "btcbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "btservice.h"

Btservice::Btservice() : m_CBusComm(new BtCBusComm()){

    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));
}

Btservice::~Btservice() {

}

Btservice &Btservice::getInstance() {
    static Btservice instance;
    return instance;
}

void Btservice::initialise() {
    // Publish
    m_CBusComm->initialise();
    qDebug() << "Btservice " << __FUNCTION__;
    InitBLE();
}

uint64_t Btservice::get64BitValue(quint8* IMEI)
{
    uint64_t value = 0;
    // Combine bytes into a uint64_t
    for (int i = 0; i < 7; ++i) {
        value |= static_cast<uint64_t>(IMEI[i]) << (8 * (6 - i));
    }
    return value;
}

uint64_t Btservice::get64BitValueFrom48bits(uint8_t *BLE_MACID)
{
    uint64_t value = 0;
    for (int i = 0; i < 6; ++i) {
        value |= static_cast<uint64_t>(BLE_MACID[i]) << (8 * (5 - i));
    }
    return value;
}

void Btservice::get64BitValueFrom70bits(uint8_t *SIM_ICCID, uint8_t& upperBits, uint64_t& lowerBits)
{
    // Assign the first 64 bits to `lowerBits`
    for (int i = 0; i < 8; ++i) {
        lowerBits |= static_cast<uint64_t>(SIM_ICCID[i]) << (8 * (7 - i));
    }

    // Assign the remaining 6 bits to `upperBits`
    upperBits = SIM_ICCID[8] >> 2; // Only the most significant 6 bits of the 9th byte
}


void Btservice::InitBLE()
{
    // Set up advertising data
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setIncludePowerLevel(true);
    advertisingData.setLocalName("BASEXCORE_BT");
    advertisingData.setServices(QList<QBluetoothUuid>() << QBluetoothUuid("{6E400001-B5A3-F393-E0A9-E50E24DCCA9E}"));

    // Create BLE Controller (Peripheral mode)
    leController= QLowEnergyController::createPeripheral();
    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(QBluetoothUuid("{6E400001-B5A3-F393-E0A9-E50E24DCCA9E}"));

    QLowEnergyCharacteristicData charDataWrite;
    charDataWrite.setUuid(QBluetoothUuid("{6E400002-B5A3-F393-E0A9-E50E24DCCA9E}"));
    charDataWrite.setValue(QByteArray(2, 0));
    charDataWrite.setProperties(QLowEnergyCharacteristic::Write);
    serviceData.addCharacteristic(charDataWrite);

    QLowEnergyCharacteristicData charDataRead;
    charDataRead.setUuid(QBluetoothUuid("{6E400003-B5A3-F393-E0A9-E50E24DCCA9E}"));
    charDataRead.setValue(QByteArray(2, 0));
    charDataRead.setProperties(QLowEnergyCharacteristic::Read);
    serviceData.addCharacteristic(charDataRead);

    const QLowEnergyDescriptorData clientConfigRW(QBluetoothUuid("{00002902-0000-1000-8000-00805f9b34fb}"),
                                                  QByteArray(2, 0));
    charDataWrite.addDescriptor(clientConfigRW);
    charDataRead.addDescriptor(clientConfigRW);

    QLowEnergyCharacteristicData charDataNotify;
    charDataNotify.setUuid(QBluetoothUuid("{6E400004-B5A3-F393-E0A9-E50E24DCCA9E}"));
    charDataNotify.setValue(QByteArray(2, 0));
    charDataNotify.setProperties(QLowEnergyCharacteristic::Notify);
    const QLowEnergyDescriptorData clientConfig(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration,
                                                QByteArray(2, 0));
    charDataNotify.addDescriptor(clientConfig);
    service = QSharedPointer<QLowEnergyService>(leController->addService(serviceData));

    // Connect signals to detect BLE connections and disconnections
    QObject::connect(leController, &QLowEnergyController::connected, [&]()
    {
        qDebug() << "Device connected!";
        // MessagePtr message = QSharedPointer<BTConnectionStatusUpdate>(new BTConnectionStatusUpdate(true));
        // m_CBusComm->push(message);
        const QLowEnergyCharacteristic characteristic;
        service->readCharacteristic(characteristic);
        qDebug() << "charcterstic received on connect";
        qDebug() << "value " << characteristic.value();
        qDebug() << "uuid" << characteristic.uuid();
        qDebug() << "name = " << characteristic.name();
        QList<QLowEnergyDescriptor> des = characteristic.descriptors();
        for (const QLowEnergyDescriptor &desc : des) {
            qDebug() << "Descriptor UUID:" << desc.uuid().toString();
            //qDebug() << "Descriptor handle:" << desc.handle().;
            qDebug() << "Descriptor name:" << desc.name();
        }
        QByteArray charValue = service->characteristic(QBluetoothUuid("{6E400002-B5A3-F393-E0A9-E50E24DCCA9E}")).value();
        qDebug() << "Current Value:" << charValue;

    });

    QObject::connect(leController, &QLowEnergyController::disconnected, [&]() {
        qDebug() << "Device disconnected!";
        // MessagePtr message = QSharedPointer<BTConnectionStatusUpdate>(new BTConnectionStatusUpdate(false));
        // m_CBusComm->push(message);
        // Restart advertising when the device disconnects
        qDebug() << "Restarting advertising...";
        leController->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, advertisingData);
    });

    // Connect signal to handle write requests
    QObject::connect(service.data(), &QLowEnergyService::characteristicWritten, [&](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {
        //if (characteristic.uuid() == customCharUuid) {
        qDebug() << "Received Data: " << newValue.toHex();
        //}
    });

    // Connect signal to handle write requests
    QObject::connect(service.data(), &QLowEnergyService::characteristicRead, [&](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {
        //if (characteristic.uuid() == customCharUuid) {
        qDebug() << "Received Data: " << newValue.toHex();
        //}
    });

    // Connect signal to handle write requests
    QObject::connect(service.data(), &QLowEnergyService::characteristicChanged, [&](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue) {
        //if (characteristic.uuid() == customCharUuid) {
        qDebug() << "Received Data: " << newValue.toHex();
        //}
    });

    // Start advertising
    leController->startAdvertising(QLowEnergyAdvertisingParameters(), advertisingData, advertisingData);
    qDebug() << "Advertising started...";
}

void Btservice::readData()
{
    qDebug() << "Btservice " << __FUNCTION__;
    QByteArray data;// = m_serial->readAll();
    switch(data[3])
    {
    }
}

void Btservice::writeData()
{
    qDebug() << "Btservice " << __FUNCTION__;
}

void Btservice::handleError()
{
    qDebug() << "Btservice " << __FUNCTION__ ;
}

void Btservice::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;
}



