#include "wifiservice.h"
#include "wificbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>
#include <QProcess>

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QThread>


#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QRegularExpression>
#include "WifiSignalStrengthMessage.h"

Wifiservice::Wifiservice() : m_CBusComm(new WifiCBusComm()),m_initialConnect(true),m_signalStrength(-1),m_retryCount(0)
{
    QObject::connect(&m_fetchNetworkStatusTimer, &QTimer::timeout, this, &Wifiservice::analyseNetworkStatus);

}

void Wifiservice::checkNetworkInterfaces(const QString &command)
{
    // to do
}

Wifiservice::~Wifiservice()
{
    disconnectWifi();
}

Wifiservice &Wifiservice::getInstance()
{
    static Wifiservice instance;
    return instance;
}

void Wifiservice::initialise()
{
    // Publish
    if(enableDebugLogs)
        qDebug() << "Wifiservice " << __FUNCTION__;

    m_CBusComm->initialise();

    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));

    connect(m_CBusComm.get(), SIGNAL(connectWifi()),this, SLOT(onConnectWifi()));
    connect(m_CBusComm.get(), SIGNAL(disconnectWifi()),this, SLOT(onDisConnectWifi()));

}

void Wifiservice::processStart(QSharedPointer<const InitializationMessage>)
{
    if(enableDebugLogs)
        qDebug() << __FUNCTION__;
}


void Wifiservice::restartWifi()
{
    // restart the wpa_supplicant
    executeCommandWithDebug("sudo systemctl restart wpa_supplicant");
}

void Wifiservice::updateWifiSSIDAndPassword_Slot()
{
    bool ifSsidAndPasswordChanged = false;
    if(ifSsidAndPasswordChanged)
    {
        updateWpaSupplicantConfig();
        disconnectWifi();
        connectWifi();
    }
    else
    {
        if(m_initialConnect)
        {
            m_initialConnect = false;
            connectWifi();
        }
    }
}

void Wifiservice::updateWpaSupplicantConfig()
{
    // Define the path to the existing wpa_supplicant.conf file
    QString filePath = "/etc/wpa_supplicant.conf";
    // QString filePath = QCoreApplication::applicationDirPath() + "/wpa_supplicant.conf";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "Failed to open" << filePath << " for reading and writing.";
        qDebug() << "Error as " << file.errorString();
        return;
    }

    // Read the file content
    QString content = file.readAll();
    file.close();
    if (enableDebugLogs)
        qDebug() << "Original content of wpa_supplicant.conf:\n" << content;

    // Ensure WiFi credentials are valid
    if (m_WIFI_SSID.isEmpty() || m_WIFI_PASSWORD.isEmpty() || m_WIFI_PASSWORD.length() < 8 || m_WIFI_PASSWORD.length() > 63) {
        qWarning() << "Invalid SSID or Password! SSID must not be empty and Password must be 8-63 characters.";
        return;
    }

    // Update SSID and PSK only inside the network block
    QRegularExpression ssidRegex(R"(ssid=\"[^\"]*\")");
    QRegularExpression pskRegex(R"(psk=\"[^\"]*\")");

    bool ssidUpdated = false, pskUpdated = false;
    QStringList lines = content.split("\n");

    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].contains(ssidRegex)) {
            lines[i] = QString("    ssid=\"%1\"").arg(m_WIFI_SSID);
            ssidUpdated = true;
        }
        if (lines[i].contains(pskRegex)) {
            lines[i] = QString("    psk=\"%1\"").arg(m_WIFI_PASSWORD);
            pskUpdated = true;
        }
    }

    // Ensure SSID and PSK are inside `network={}` if not found
    if (!ssidUpdated || !pskUpdated) {
        for (int i = 0; i < lines.size(); ++i) {
            if (lines[i].trimmed() == "network={") {
                if (!ssidUpdated) {
                    lines.insert(i + 1, QString("    ssid=\"%1\"").arg(m_WIFI_SSID));
                    ssidUpdated = true;
                }
                if (!pskUpdated) {
                    lines.insert(i + 2, QString("    psk=\"%1\"").arg(m_WIFI_PASSWORD));
                    pskUpdated = true;
                }
                break;
            }
        }
    }

    // Join the updated content
    content = lines.join("\n");

    // Debugging modified content
    if (enableDebugLogs)
        qDebug() << "Modified content to write:\n" << content;

    // Write back the modified content
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream out(&file);
        out << content;
        file.close();
        if (enableDebugLogs)
            qDebug() << "wpa_supplicant.conf file updated successfully.";
    }
    else
    {
        qWarning() << "Failed to open" << filePath << " for writing.";
    }

}

void Wifiservice::printConnectedSSID(QStringList lines)
{
    if(enableDebugLogs)
        qDebug() << "Executing 'iw dev mlan0 link' to check the connected network...";

    if(enableDebugLogs)
        qDebug() << "'iw dev mlan0 link' command executed successfully.";

    QString ssid;
    for (const QString &line : lines)
    {
        if (line.contains("SSID:"))
        {
            ssid = line.split("SSID:")[1].trimmed();  // Extract SSID and remove spaces
            if(enableDebugLogs)
                qDebug() << "Connected SSID: " << ssid;
            break;
        }
    }

    if (ssid.isEmpty())
    {
        qWarning() << "No SSID found in the output.";
    }
}

void Wifiservice::fetchSignalStrength(QStringList lines)
{
    if(enableDebugLogs)
        qDebug() << "Fetching signal strength from 'iw dev mlan0 link' output...";

    int strength = 0;
    bool strengthFound = false;

    for (const QString &line : lines)
    {
        if (line.contains("signal:"))
        {
            QRegularExpression regex(R"(\bsignal:\s*(-?\d+)\s*dBm\b)");
            QRegularExpressionMatch match = regex.match(line);

            if (match.hasMatch())
            {
                bool ok;
                strength = match.captured(1).toInt(&ok);

                if (ok)
                {
                    if(enableDebugLogs)
                        qDebug() << "Matched signal strength:" << strength << "dBm";
                    strengthFound = true;
                    break;
                }
            }
        }
    }

    if (strengthFound)
    {
        if (strength != m_signalStrength)
        {
            if(enableDebugLogs)
                qDebug() << "Signal strength has changed.";

            if (strength > -50)
            {
                m_signalStrength = 4;
                if(enableDebugLogs)
                    qDebug() << "Signal strength is strong: " << m_signalStrength;  // Debugging assigned signal strength value
            }
            else if (strength > -60)
            {
                m_signalStrength = 3;
                if(enableDebugLogs)
                    qDebug() << "Signal strength is good: " << m_signalStrength;  // Debugging assigned signal strength value
            }
            else if (strength > -70)
            {
                m_signalStrength = 2;
                if(enableDebugLogs)
                    qDebug() << "Signal strength is moderate: " << m_signalStrength;  // Debugging assigned signal strength value
            }
            else if (strength > -80)
            {
                m_signalStrength = 1;
                if(enableDebugLogs)
                    qDebug() << "Signal strength is weak: " << m_signalStrength;  // Debugging assigned signal strength value
            }
        }
        else
        {
            if(enableDebugLogs)
                qDebug() << "Signal strength is unchanged: " << m_signalStrength;  // If the signal strength is the same as before
        }

        QSharedPointer<WifiSignalStrengthMessage> WifiSignalStrengthMessageValue = QSharedPointer<WifiSignalStrengthMessage>(new WifiSignalStrengthMessage(m_signalStrength));
        m_CBusComm->push(WifiSignalStrengthMessageValue);

        if(enableDebugLogs)
            qDebug() << "Signal strength message pushed with value:" << m_signalStrength;
    }
    else
    {
        qWarning() << "No signal strength found in the output.";

        m_signalStrength = -1;
        QSharedPointer<WifiSignalStrengthMessage> WifiSignalStrengthMessageValue = QSharedPointer<WifiSignalStrengthMessage>(new WifiSignalStrengthMessage(m_signalStrength));
        m_CBusComm->push(WifiSignalStrengthMessageValue);
    }
}

void Wifiservice::analyseNetworkStatus()
{
    if(enableDebugLogs)
        qDebug() << "Starting to execute 'iw dev mlan0 link' command to analyse connected network details...";

    if(enableDebugLogs)
        qDebug() << "Retry Count : " << m_retryCount;

    if(enableDebugLogs)
        qDebug() << "Signal Strength: " << m_signalStrength;

    if(m_signalStrength == -1 && m_retryCount == 4)
    {

        qDebug() << "Attempting to Retry connetcion by disconnecting and connecting";
        m_fetchNetworkStatusTimer.stop();
        m_retryCount = 0;
        disconnectWifi();
        connectWifi();
    }

    m_retryCount++;

    // Initialize QProcess to run 'iw dev mlan0 link'
    QProcess iwProcess;
    iwProcess.start("iw", {"dev", "mlan0", "link"});
    iwProcess.waitForFinished();

    // Check if the command executed successfully
    if (iwProcess.exitCode() == 0)
    {
        if(enableDebugLogs)
            qDebug() << "iw dev mlan0 link executed successfully.";

        QByteArray output = iwProcess.readAllStandardOutput();
        //     if(enableDebugLogs)
        qDebug() << "Connected network details:";

        // output = "Connected to c8:9e:43:0a:54:10 (on mlan0)\n\tSSID: OptM\n\tfreq: 5180\n\tRX: 8166 bytes (105 packets)\n\tTX: 2956 bytes (23 packets)\n\tsignal: -73 dBm\n\trx bitrate: 6.0 MBit/s\n\ttx bitrate: 13.0 MBit/s VHT-MCS 1 VHT-NSS 1\n\n\tbss flags:\tshort-slot-time\n\tdtim period:\t2\n\tbeacon int:\t100\n";
        QString outputStr = QString::fromStdString(output.toStdString());

        QStringList splittedLines = outputStr.split("\n");

        // foreach (QString line, splittedLines) {
        //     line.replace("\t", "");
        //         if(enableDebugLogs)
        // qDebug().noquote() << line;
        // }

        printConnectedSSID(splittedLines);
        fetchSignalStrength(splittedLines);
    }
    else
    {
        qWarning() << "Failed to execute iw dev mlan0 link:";
        qWarning() << iwProcess.readAllStandardError();

        qWarning() << "Exit code:" << iwProcess.exitCode();
    }
}

void Wifiservice::onConnectWifi()
{
    connectWifi();
}

void Wifiservice::onDisConnectWifi()
{
    disconnectWifi();
}

bool Wifiservice::executeCommandUsingProcess(QString command)
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

void Wifiservice::startNetworkChecks()
{
    if(enableDebugLogs)
        qDebug() << "Starting periodic network checks every 10 seconds.";
    m_fetchNetworkStatusTimer.start(10000); // Call showConnectedNetwork() every 5 seconds
}

void Wifiservice::executeCommandWithDebug(QString command)
{
    if(executeCommandUsingProcess(command))
    {
        if(enableDebugLogs)
            qDebug() << command << " Execution is Success";
    }
    else
    {
        if(enableDebugLogs)
            qDebug() << command << " Execution is not Success";
    }
}
#include <QDebug>

bool connectToWiFi(const QString &command)
{
    QProcess process;

    qDebug() << "Executing command:" << command;
    process.start(command);
    if (!process.waitForFinished(5000)) {  // Wait up to 5 seconds for execution
        qDebug() << "Failed to start wpa_supplicant!";
        return false;
    }

    // Start a process to monitor system logs
    QProcess logProcess;
    logProcess.start("dmesg -w");  // `-w` keeps dmesg running continuously
    if (!logProcess.waitForStarted()) {
        qDebug() << "Failed to start log monitoring!";
        return false;
    }

    // Read log output continuously
    while (logProcess.waitForReadyRead(-1)) {  // Infinite wait for new lines
        QByteArray output = logProcess.readAllStandardOutput();
        QString logText = QString::fromUtf8(output);
        qDebug().noquote() << logText;  // Print logs

        if (logText.contains("Connected to bssid")) {
            qDebug() << "WiFi successfully connected!";
            logProcess.kill();  // Stop log monitoring
            return true;
        }
    }

    logProcess.kill();
    return false;
}

void Wifiservice::setIPAndGateway()
{
    QString wifiConfigFileName = QCoreApplication::applicationDirPath() + "/wificonfig.json";

    QFile file(wifiConfigFileName); // Replace with the actual path
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open wificonfig.json";
        return;
    }

    // Parse the JSON content
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isObject())
    {
        qDebug() << "Invalid JSON format in wificonfig.json";
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonObject wifiObj = rootObj["wifi"].toObject();

    QString ipAddress = wifiObj["ip"].toString();
    QString gateway   = wifiObj["gateway"].toString();

    qDebug() << "Extracted ip : " << ipAddress;
    qDebug() << "Extracted gateway : " << gateway;

    if (ipAddress.isEmpty() || gateway.isEmpty())
    {
        qDebug() << "Invalid IP or Gateway in JSON";
        return;
    }

    // Set IP Address on mlan0
    QString setIpCmd = QString("sudo ip addr add %1 dev mlan0").arg(ipAddress);
    qDebug() << "IP command  : " << setIpCmd;
    executeCommandWithDebug(setIpCmd);

    // Set Default Gateway on mlan0
    QString setRouteCmd = QString("sudo ip route add default via %1 dev mlan0").arg(gateway);
    qDebug() << "Route Gateway command  : " << setIpCmd;
    executeCommandWithDebug(setRouteCmd);
}

void Wifiservice::startWpaSupplicant()
{
    // Read the output after some delay (so the process has time to generate output)

    QString command = "wpa_supplicant -i mlan0 -c /etc/wpa_supplicant.conf";

    connect(&m_wpaSupplicantProcess, &QProcess::readyReadStandardOutput, this, [&]()
    {
        QString output = m_wpaSupplicantProcess.readAllStandardOutput();
        if(enableDebugLogs)
            qDebug() << "Process output: " << output;

        if(output.contains("connected", Qt::CaseInsensitive))
        {
            if(enableDebugLogs)
                qDebug() << "WiFi successfully connected! Starting network checks...";

            setIPAndGateway();
            startNetworkChecks();
        }
    });

    connect(&m_wpaSupplicantProcess, &QProcess::readyReadStandardError, this, [&]()
    {
        QString errorOutput = m_wpaSupplicantProcess.readAllStandardError();
        if(enableDebugLogs)
            qDebug() << "Process error: " << errorOutput;
    });

    connect(&m_wpaSupplicantProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus status) {
        qDebug() << "Process exited with code: " << exitCode;
    });

    if(enableDebugLogs)
        qDebug() << "Starting process: " << command;

    m_wpaSupplicantProcess.start("sh", QStringList() << "-c" << command);
}

void Wifiservice::processOutput()
{
    // QString output = m_wpaSupplicantProcess.readAllStandardOutput();
    // if(enableDebugLogs)
    //     qDebug() << "Process output: " << output;

    // if(output.contains("connected", Qt::CaseInsensitive))
    // {
    //     if(enableDebugLogs)
    //         qDebug() << "WiFi successfully connected! Starting network checks...";
    //     startNetworkChecks();
    // }
}

void Wifiservice::processError()
{
    // QString errorOutput = m_wpaSupplicantProcess.readAllStandardError();
    // if(enableDebugLogs)
    //     qDebug() << "Process error: " << errorOutput;
}


void Wifiservice::connectWifi()
{
    if(enableDebugLogs)
        qDebug() << "Initializing Wi-Fi connection...";

    // Load Wi-Fi module
    executeCommandWithDebug("modprobe moal mod_para=nxp/wifi_mod_para.conf");


    // Unblock all Wi-Fi devices
    executeCommandWithDebug("rfkill unblock all");

    // rfkill list all
    executeCommandWithDebug("rfkill list all");

    // Connect to Wi-Fi network
    startWpaSupplicant();
}

void Wifiservice::bringInterfaceDown()
{
    if(enableDebugLogs)
        qDebug() << "Making wifi down...";

    executeCommandWithDebug("sudo ip link set mlan0 down");
}

void Wifiservice::killWpaSupplicant()
{
    if(enableDebugLogs)
        qDebug() << "Disconnecting Wi-Fi by killing wpa_supplicant...";

    executeCommandWithDebug("sudo killall wpa_supplicant");
}

void Wifiservice::disconnectWifi()
{
    if(enableDebugLogs)
        qDebug() << "Starting Wi-Fi disconnect process...";

    // Bring the interface down
    bringInterfaceDown();

    executeCommandWithDebug("sudo rm -rf /var/run/wpa_supplicant");
    executeCommandWithDebug("sudo rm -f /var/run/wpa_supplicant/mlan0");

    // Kill wpa_supplicant to disconnect Wi-Fi
    killWpaSupplicant();

    m_signalStrength = -1;
    QSharedPointer<WifiSignalStrengthMessage> WifiSignalStrengthMessageValue = QSharedPointer<WifiSignalStrengthMessage>(new WifiSignalStrengthMessage(m_signalStrength));
    m_CBusComm->push(WifiSignalStrengthMessageValue);
}
