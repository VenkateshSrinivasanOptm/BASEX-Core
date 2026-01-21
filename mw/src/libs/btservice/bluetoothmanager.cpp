#include "bluetoothmanager.h"

 BluetoothManager::BluetoothManager(QObject* parent)
        : QObject(parent),localDevice(new QBluetoothLocalDevice(this)) {
        // Show local Bluetooth MAC address
        if (localDevice->isValid()) {
            qInfo() << "Local Bluetooth Device Name:" << localDevice->name();
            qInfo() << "Local Bluetooth MAC Address:" << localDevice->address().toString();
        } else {
            qWarning() << "No Bluetooth adapter found!";
            return;
        }
        // Connect signals for device discovery
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &BluetoothManager::deviceDiscovered);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                this, &BluetoothManager::scanFinished);

        // Connect signal to handle pairing status changes
        connect(localDevice, &QBluetoothLocalDevice::pairingFinished,
                this, &BluetoothManager::pairingFinished);
    }

    void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo& device) {
        qInfo() << "Discovered Device:";
        qInfo() << "  Name:" << device.name();
        qInfo() << "  MAC Address:" << device.address().toString();
        devices.append(device);
    }

    void BluetoothManager::scanFinished() {
        qInfo() << "Device discovery finished.";
        if (devices.isEmpty()) {
            qInfo() << "No devices found.";
        } else {
            //qInfo() << "Select a device to pair with by entering its index:";
            for (int i = 0; i < devices.size(); ++i) {
                qInfo() << i << ":" << devices[i].name() << "(" << devices[i].address().toString() << ")";
            }
        }
    }

    void BluetoothManager::pairDevice(const QBluetoothDeviceInfo& device) {
        if (!localDevice->isValid()) {
            qWarning() << "No Bluetooth adapter found!";
            return;
        }
        qInfo() << "Attempting to pair with device:" << device.name();
        localDevice->requestPairing(device.address(), QBluetoothLocalDevice::Paired);
        qInfo() << "Pairing request sent.";
    }

    void BluetoothManager::pairingFinished(const QBluetoothAddress& address, QBluetoothLocalDevice::Pairing pairing) {
        if (pairing == QBluetoothLocalDevice::Paired) {
            qInfo() << "Successfully paired with device:" << address.toString();
        } else if (pairing == QBluetoothLocalDevice::AuthorizedPaired) {
            qInfo() << "Device paired and authorized:" << address.toString();
        } else if (pairing == QBluetoothLocalDevice::Unpaired) {
            qWarning() << "Device unpaired:" << address.toString();
        }
    }




