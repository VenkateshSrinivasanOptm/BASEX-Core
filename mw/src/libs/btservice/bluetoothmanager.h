#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QCoreApplication>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceInfo>
#include <QTextStream>
#include <QDebug>

class BluetoothManager : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothManager(QObject *parent = nullptr);
private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo& device);
    void scanFinished();
    void pairDevice(const QBluetoothDeviceInfo& device);
    void pairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing);

private:
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothLocalDevice* localDevice;
    QList<QBluetoothDeviceInfo> devices;
};

#endif // BLUETOOTHMANAGER_H
