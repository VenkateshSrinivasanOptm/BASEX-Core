#ifndef CANPARSER_H
#define CANPARSER_H

#include <QCanBusDevice>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QVariant>

class CanParser : public QObject
{
    Q_OBJECT

public:
    explicit CanParser(QObject *parent = nullptr);
    virtual ~CanParser();
    typedef QPair<QCanBusDevice::ConfigurationKey, QVariant> ConfigurationItem;

    bool writeFrameCanDeviceOne(uint frameId, QByteArray payload) const;
    bool writeFrameCanDeviceTwo(uint frameId, QByteArray payload) const;
    void emitBSS_EMVoltage_Signal(QByteArray payLoad);

signals:


public slots :

private slots:
    void sendFrame(const QCanBusFrame &frame) const;

    void busStatus1();
    void processReceivedFramesForCan1();
    void processFramesWrittenForCan1(qint64 count);
    void processErrorsCan1(QCanBusDevice::CanBusError) const;

    void busStatus2();
    void processReceivedFramesForCan2();
    void processFramesWrittenForCan2(qint64 count);
    void processErrorsCan2(QCanBusDevice::CanBusError error) const;

private:

    QMutex m_canDeviceOneMutex;
    qint64 m_canDeviceOneAvailableFrames;
    QTimer *m_canDeviceOneBusStatusTimer;
    qint64 m_canDeviceOneFramesWrittenCount;
    QScopedPointer<QCanBusDevice> m_canDeviceOne;
    QList<ConfigurationItem> m_canDeviceOneConfigurations;

    QMutex m_canDeviceTwoMutex;
    qint64 m_canDeviceTwoAvailableFrames;
    QTimer *m_canDeviceTwoBusStatusTimer;
    qint64 m_canDeviceTwoFramesWrittenCount;
    QScopedPointer<QCanBusDevice> m_canDeviceTwo;
    QList<ConfigurationItem> m_canDeviceTwoConfigurations;

    void processPayloadData(QByteArray payLoad, quint32 frameID, QString deviceName);
    void listAvailableCanBusPluginsAndDevices();
    bool executeCommandUsingProcess(QString command);

    void connectDevice1();
    void disconnectDevice1();

    void connectDevice2();
    void disconnectDevice2();
    bool loadCANModule(QString canDeviceName);
    void processResultOFCanWrite(QByteArray payload, bool writeSuccess);
};

#endif // CANPARSER_H
