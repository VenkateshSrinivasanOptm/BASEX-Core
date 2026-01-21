#include "canparser.h"
#include <QCanBus>
#include <QCanBusFrame>
#include <QDebug>
#include <QTimer>
#include <QVariant>
#include <QProcess>

bool enableDebugLogs = false;

#define DECIMAL_PRECISION 3


CanParser::CanParser(QObject *parent)
    : QObject{parent}, m_canDeviceOneBusStatusTimer(new QTimer(this)),
    m_canDeviceOneFramesWrittenCount(0),m_canDeviceTwoBusStatusTimer(new QTimer(this)),m_canDeviceTwoFramesWrittenCount(0)
{
    if(enableDebugLogs)
        qDebug() << "CanParser Constructor is called";
    if(loadCANModule("can0"))
    {
        if(enableDebugLogs)
            qDebug() << "Can0 module loaded successfully";
        listAvailableCanBusPluginsAndDevices();
        connectDevice1();
        connect(m_canDeviceOneBusStatusTimer, &QTimer::timeout, this, &CanParser::busStatus1);
        m_canDeviceOneBusStatusTimer->start(5000);
    }
    else
        if(enableDebugLogs)
            qDebug() << "Can0 module not loaded";

    if(loadCANModule("can1"))
    {
        if(enableDebugLogs)
            qDebug() << "Can1 module loaded successfully";
        listAvailableCanBusPluginsAndDevices();
        connect(m_canDeviceTwoBusStatusTimer, &QTimer::timeout, this, &CanParser::busStatus1);
        m_canDeviceTwoBusStatusTimer->start(5000);
        connectDevice2();
    }
    else
        if(enableDebugLogs)
            qDebug() << "Can1 module not loaded";
}

CanParser::~CanParser()
{
    disconnectDevice1();
    disconnectDevice2();
}

bool CanParser::executeCommandUsingProcess(QString command)
{
    if(enableDebugLogs)
        qDebug() << "Command to execute : " << command;
    QProcess process;
    process.start("sh", QStringList() << "-c" << command);
    process.waitForFinished();

    if (process.exitCode() == 0)
    {
        if(enableDebugLogs)
            qDebug() << "Process output : " <<  process.readAllStandardOutput();
        if(enableDebugLogs)
            qDebug() << "Process error : " <<  process.readAllStandardError();

        if(process.readAllStandardError().isEmpty())
            return true;
        else
            return false;
    }
    else
    {
        qWarning() << "The command " + command + "is executed with negative code with error as " << process.readAllStandardError();
        return false;
    }
}

bool CanParser::loadCANModule(QString canDeviceName)
{
    if(enableDebugLogs)
        qDebug() << "Loading CAN module for " << canDeviceName;

    QString bringCanDownProcessCommand = "ip link set " + canDeviceName + " down";
    if(!executeCommandUsingProcess(bringCanDownProcessCommand))
        return false;

    QString bringCanUpProcessCommand = "ip link set " + canDeviceName + " up type can bitrate 1000000";
    return executeCommandUsingProcess(bringCanUpProcessCommand);
}

void CanParser::listAvailableCanBusPluginsAndDevices()
{
    QString errorString;

    const QStringList plugins = QCanBus::instance()->plugins();

    for (const auto &plugin : plugins)
    {
        const QList<QCanBusDeviceInfo> devices =
            QCanBus::instance()->availableDevices(plugin, &errorString);

        // Check for errors in fetching available devices
        if (!errorString.isEmpty())
        {
            if(enableDebugLogs)
                qDebug() << "[ERROR] Failed to retrieve available devices for plugin:"
                         << plugin << "Error:" << errorString;
        }
        else
        {
            // Log the successful retrieval of devices
            if(enableDebugLogs)
                qDebug() << "[INFO] Available devices for plugin:" << plugin;
            for (auto device : devices)
            {
                if(enableDebugLogs)
                    qDebug() << "Device Name:" << device.name();
            }
        }
    }

    if(enableDebugLogs)
        qDebug() << "[INFO] Number of available CAN Bus plugins:" << QCanBus::instance()->plugins().size();
    if(enableDebugLogs)
        qDebug() << "[INFO] Listing available CAN Bus plugins:";

    foreach (const QString &pluginName, plugins)
    {
        if(enableDebugLogs)
            qDebug() << "Plugin Name:" << pluginName;
    }
}

void CanParser::busStatus1()
{
    if (!m_canDeviceOne || !m_canDeviceOne->hasBusStatus())
    {
        if(enableDebugLogs)
            qDebug() << (tr("No CAN bus status available."));
        m_canDeviceOneBusStatusTimer->stop();
        return;
    }

    switch (m_canDeviceOne->busStatus())
    {
    case QCanBusDevice::CanBusStatus::Good:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Good.");
        break;
    case QCanBusDevice::CanBusStatus::Warning:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Warning.");
        break;
    case QCanBusDevice::CanBusStatus::Error:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Error.");
        break;
    case QCanBusDevice::CanBusStatus::BusOff:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Bus Off.");
        break;
    default:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Unknown.");
        break;
    }
}

void CanParser::busStatus2()
{
    if (!m_canDeviceTwo || !m_canDeviceTwo->hasBusStatus())
    {
        if(enableDebugLogs)
            qDebug() << (tr("No CAN bus status available."));
        m_canDeviceTwoBusStatusTimer->stop();
        return;
    }

    switch (m_canDeviceTwo->busStatus())
    {
    case QCanBusDevice::CanBusStatus::Good:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Good.");
        break;
    case QCanBusDevice::CanBusStatus::Warning:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Warning.");
        break;
    case QCanBusDevice::CanBusStatus::Error:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Error.");
        break;
    case QCanBusDevice::CanBusStatus::BusOff:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Bus Off.");
        break;
    default:
        if(enableDebugLogs)
            qDebug() << ("CAN bus status: Unknown.");
        break;
    }
}

static QString frameFlags(const QCanBusFrame &frame)
{
    QString result = QLatin1String(" --- ");

    if (frame.hasBitrateSwitch())
        result[1] = QLatin1Char('B');
    if (frame.hasErrorStateIndicator())
        result[2] = QLatin1Char('E');
    if (frame.hasLocalEcho())
        result[3] = QLatin1Char('L');

    return result;
}

void CanParser::processReceivedFramesForCan1()
{
    if (!m_canDeviceOne)
        return;
    QByteArray payLoadData;

    m_canDeviceOneMutex.lock();
    m_canDeviceOneAvailableFrames = m_canDeviceOne->framesAvailable();
    m_canDeviceOneMutex.unlock();

    while (m_canDeviceOneAvailableFrames)
    {
        QCanBusFrame currentFrame = m_canDeviceOne->readFrame();

        switch (currentFrame.frameType())
        {
        case QCanBusFrame::DataFrame:
        {
            quint32 frame_id = currentFrame.frameId();
            payLoadData = currentFrame.payload();

            processPayloadData(payLoadData,frame_id,"Device - Can0");
        }
        break;
        case QCanBusFrame::UnknownFrame:
        {
            break;
        }
        case QCanBusFrame::ErrorFrame:
        {
            break;
        }
        case QCanBusFrame::InvalidFrame:
        {
            break;
        }
        case QCanBusFrame::RemoteRequestFrame:
        {
            break;
        }
        }

        m_canDeviceOneMutex.lock();
        m_canDeviceOneAvailableFrames = m_canDeviceOne->framesAvailable();
        m_canDeviceOneMutex.unlock();
    }
}

void CanParser::processReceivedFramesForCan2()
{
    if (!m_canDeviceTwo)
        return;
    QByteArray payLoadData;

    m_canDeviceTwoMutex.lock();
    m_canDeviceTwoAvailableFrames = m_canDeviceTwo->framesAvailable();
    m_canDeviceTwoMutex.unlock();

    while (m_canDeviceTwoAvailableFrames)
    {
        QCanBusFrame currentFrame = m_canDeviceTwo->readFrame();

        switch (currentFrame.frameType())
        {
        case QCanBusFrame::DataFrame:
        {
            quint32 frame_id = currentFrame.frameId();
            payLoadData = currentFrame.payload();

            // as of now we use the same logic, once the updated doc comes we need to change it
            processPayloadData(payLoadData,frame_id,"Device - Can1");
        }
        break;
        case QCanBusFrame::UnknownFrame:
        {
            break;
        }
        case QCanBusFrame::ErrorFrame:
        {
            break;
        }
        case QCanBusFrame::InvalidFrame:
        {
            break;
        }
        case QCanBusFrame::RemoteRequestFrame:
        {
            break;
        }
        }

        m_canDeviceTwoMutex.lock();
        m_canDeviceTwoAvailableFrames = m_canDeviceTwo->framesAvailable();
        m_canDeviceTwoMutex.unlock();
    }
}

void CanParser::sendFrame(const QCanBusFrame &frame) const
{
    // to do
}

bool CanParser::writeFrameCanDeviceOne(uint frameId,QByteArray payload) const
{
    if(enableDebugLogs)
        qDebug() << "Started writeFrame for FrameID : " << frameId;

    bool isSuccess = false;
    if(enableDebugLogs)
        qDebug() << "[INFO] Preparing to write CAN frame.";
    if(enableDebugLogs)
        qDebug() << QString("[INFO] Frame details - Frame ID: 0x%1, Payload: %2")
                        .arg(QVariant(frameId).toString())
                        .arg(QVariant(payload).toString());

    QCanBusFrame frame;
    frame.setFrameId(frameId);  // CAN ID
    frame.setPayload(payload);  // Data payload


    if (!m_canDeviceOne.isNull() && frame.isValid())
    {
        if(enableDebugLogs)
            qDebug() << "Current CAN Device State:" << m_canDeviceOne.get()->state();
        if (m_canDeviceOne.get()->writeFrame(frame))
        {
            isSuccess = true;
            if(enableDebugLogs)
                qDebug() << "[SUCCESS] CAN frame written successfully. Frame ID: 0x"
                         << QVariant(frameId).toString();
        }
        else
        {
            if(enableDebugLogs)
                qDebug() << "[ERROR] Failed to write CAN frame. Frame ID: 0x"
                         << QVariant(frameId).toString();
        }
    }
    else
    {
        if(enableDebugLogs)
            qDebug() << "[ERROR] Invalid CAN frame. Frame ID: 0x"
                     << QVariant(frameId).toString();
    }

    if(enableDebugLogs)
        qDebug() << QString("[INFO] writeFrame completed. Payload: %1").arg(QString::fromStdString(payload.toStdString()));
    return isSuccess;
}

bool CanParser::writeFrameCanDeviceTwo(uint frameId,QByteArray payload) const
{
    if(enableDebugLogs)
        qDebug() << "Started writeFrame for FrameID : " << frameId;

    bool isSuccess = false;
    if(enableDebugLogs)
        qDebug() << "[INFO] Preparing to write CAN frame.";
    if(enableDebugLogs)
        qDebug() << QString("[INFO] Frame details - Frame ID: 0x%1, Payload: %2")
                        .arg(QVariant(frameId).toString())
                        .arg(QVariant(payload).toString());

    QCanBusFrame frame;
    frame.setFrameId(frameId);  // CAN ID
    frame.setPayload(payload);  // Data payload


    if (!m_canDeviceTwo.isNull() && frame.isValid())
    {
        if(enableDebugLogs)
            qDebug() << "Current CAN Device State:" << m_canDeviceTwo.get()->state();
        if (m_canDeviceTwo.get()->writeFrame(frame))
        {
            isSuccess = true;
            if(enableDebugLogs)
                qDebug() << "[SUCCESS] CAN frame written successfully. Frame ID: 0x"
                         << QVariant(frameId).toString();
        }
        else
        {
            if(enableDebugLogs)
                qDebug() << "[ERROR] Failed to write CAN frame. Frame ID: 0x"
                         << QVariant(frameId).toString();
        }
    }
    else
    {
        if(enableDebugLogs)
            qDebug() << "[ERROR] Invalid CAN frame. Frame ID: 0x"
                     << QVariant(frameId).toString();
    }

    if(enableDebugLogs)
        qDebug() << QString("[INFO] writeFrame completed. Payload: %1").arg(QString::fromStdString(payload.toStdString()));
    return isSuccess;
}

void CanParser::processResultOFCanWrite(QByteArray payload, bool writeSuccess)
{
    if (writeSuccess)
    {
        // if(enableDebugLogs)
        // if(enableDebugLogs)
            qDebug() << "Data written:" << payload.toHex();
    }
    else
    {
        // if(enableDebugLogs)
        // if(enableDebugLogs)
            qDebug() << "Data attempted to write:" << payload.toHex();
    }
}

void CanParser::processErrorsCan1(QCanBusDevice::CanBusError error) const
{
    switch (error)
    {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        if(enableDebugLogs)
            qDebug() << "CAN Bus Error " << m_canDeviceOne->errorString();
        break;
    default:
        break;
    }
}

void CanParser::processErrorsCan2(QCanBusDevice::CanBusError error) const
{
    switch (error)
    {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        if(enableDebugLogs)
            qDebug() << "CAN Bus Error " << m_canDeviceTwo->errorString();
        break;
    default:
        break;
    }
}

void CanParser::connectDevice1()
{
    if(enableDebugLogs)
        qDebug() << "[INFO] Preparing to establish connection for can0 in socketcan. About to connect to the CAN interface...";

    QString errorString;

    m_canDeviceOne.reset(QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                           QStringLiteral("can0"),
                                                           &errorString));

    if (!m_canDeviceOne)
    {
        if(enableDebugLogs)
            qDebug() << (tr("Error creating device '%1', reason: '%2'")
                             .arg("socketcan")
                             .arg(errorString));
        return;
    }
    if(enableDebugLogs)
        qDebug() << "[INFO] CAN device can0 instance created successfully. Preparing to connect to the CAN interface.";


    m_canDeviceOneFramesWrittenCount = 0;

    connect(m_canDeviceOne.get(), &QCanBusDevice::errorOccurred, this,
            &CanParser::processErrorsCan1);
    connect(m_canDeviceOne.get(), &QCanBusDevice::framesReceived, this,
            &CanParser::processReceivedFramesForCan1);
    connect(m_canDeviceOne.get(), &QCanBusDevice::framesWritten, this,
            &CanParser::processFramesWrittenForCan1);

    for (const ConfigurationItem &item : m_canDeviceOneConfigurations)
    {
        m_canDeviceOne->setConfigurationParameter(item.first, item.second);
    }

    //TODO: Uncomment below line to set at 500kbps. Below configuration doesn't work with peakcan.
    // m_canDevice->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant(500000));

    if (!m_canDeviceOne->connectDevice())
    {
        if(enableDebugLogs)
            qDebug() << (tr("Connection error: %1").arg(m_canDeviceOne->errorString()));
        m_canDeviceOne.reset();
    }
    else
    {
        if(enableDebugLogs)
            qDebug() << "[INFO] CAN device connected successfully on interface 'can0'. Ready to send and receive frames.";

        if (m_canDeviceOne->hasBusStatus())
        {
            m_canDeviceOneBusStatusTimer->start(2000);
        }
        else
        {
            if(enableDebugLogs)
                qDebug() << (tr("No CAN bus status available."));
        }
    }
    if(enableDebugLogs)
        qDebug() << "[INFO] Connection for can0 established successfully. Proceeding to write CAN frame.";
}

void CanParser::connectDevice2()
{
    if(enableDebugLogs)
        qDebug() << "[INFO] Preparing to establish connection for can1 in socketcan. About to connect to the CAN interface...";

    QString errorString;

    m_canDeviceTwo.reset(QCanBus::instance()->createDevice(QStringLiteral("socketcan"),
                                                           QStringLiteral("can1"),
                                                           &errorString));

    if (!m_canDeviceTwo)
    {
        if(enableDebugLogs)
            qDebug() << (tr("Error creating device '%1', reason: '%2'")
                             .arg("socketcan")
                             .arg(errorString));
        return;
    }
    if(enableDebugLogs)
        qDebug() << "[INFO] CAN device can1 instance created successfully. Preparing to connect to the CAN interface.";


    m_canDeviceTwoFramesWrittenCount = 0;

    connect(m_canDeviceTwo.get(), &QCanBusDevice::errorOccurred, this,
            &CanParser::processErrorsCan2);
    connect(m_canDeviceTwo.get(), &QCanBusDevice::framesReceived, this,
            &CanParser::processReceivedFramesForCan2);
    connect(m_canDeviceTwo.get(), &QCanBusDevice::framesWritten, this,
            &CanParser::processFramesWrittenForCan2);

    for (const ConfigurationItem &item : m_canDeviceTwoConfigurations)
    {
        m_canDeviceTwo->setConfigurationParameter(item.first, item.second);
    }

    //TODO: Uncomment below line to set at 500kbps. Below configuration doesn't work with peakcan.
    // m_canDevice->setConfigurationParameter(QCanBusDevice::BitRateKey, QVariant(500000));

    if (!m_canDeviceTwo->connectDevice())
    {
        if(enableDebugLogs)
            qDebug() << (tr("Connection error: %1").arg(m_canDeviceTwo->errorString()));
        m_canDeviceTwo.reset();
    }
    else
    {
        if(enableDebugLogs)
            qDebug() << "[INFO] CAN device connected successfully on interface 'can1'. Ready to send and receive frames.";

        if (m_canDeviceTwo->hasBusStatus())
        {
            m_canDeviceTwoBusStatusTimer->start(2000);
        }
        else
        {
            if(enableDebugLogs)
                qDebug() << (tr("No CAN bus status available."));
        }
    }
    if(enableDebugLogs)
        qDebug() << "[INFO] Connection for can1 established successfully. Proceeding to write CAN frame.";
}

void CanParser::disconnectDevice1()
{
    if (!m_canDeviceOne)
    {
        return;
    }
    m_canDeviceOneBusStatusTimer->stop();
    m_canDeviceOne->disconnectDevice();
}

void CanParser::disconnectDevice2()
{
    if (!m_canDeviceTwo)
    {
        return;
    }
    m_canDeviceTwoBusStatusTimer->stop();
    m_canDeviceTwo->disconnectDevice();
}

void CanParser::processFramesWrittenForCan1(qint64 count)
{
    m_canDeviceOneFramesWrittenCount += count;
}

void CanParser::processFramesWrittenForCan2(qint64 count)
{
    m_canDeviceTwoFramesWrittenCount += count;
}

void CanParser::processPayloadData(QByteArray payLoad, quint32 frameID, QString deviceName)
{
    if(enableDebugLogs)
        qDebug() << "Device Name is : " << deviceName;
    // extend it
    // switch(frameID)
    // {

    // }
}
