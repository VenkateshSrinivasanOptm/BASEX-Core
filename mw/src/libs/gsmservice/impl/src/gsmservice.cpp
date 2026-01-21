#include "gsmservice.h"
#include "gsmcbuscomm.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <appmanif.h>
#include "Utility.h"
#include <QByteArray>
#include<QRegularExpression>

Gsmservice::Gsmservice() : m_CBusComm(new GsmCBusComm()),m_serial(new QSerialPort())
 {
    connect(m_serial.data(), &QSerialPort::readyRead, this, &Gsmservice::readData);
    connect(m_serial.data(), &QSerialPort::errorOccurred, this, &Gsmservice::handleError);

    m_serial->setPortName("/dev/chn/2");
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);

    if (m_serial->open(QIODevice::ReadWrite)) {

    }
    else
    {
        qDebug() << "failed to open serial port " << m_serial->errorString();
    }
    m_signalStrength.setInterval(10000);
}

void Gsmservice::readData()
{
    qDebug() << __FUNCTION__;
    quint32 bytes = m_serial->bytesAvailable();
    qDebug() << "bytes received from gsm" << bytes;
    qDebug() << "**************************************";
    QByteArray response = m_serial->readAll();
    qDebug() << response;

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
    {
        static QVector<int> testValues = {5, 31, 5, 31, 5, 31, 5, 31, 5, 31, 5, 31};
        static int index = 0; // Start index

        int testValue = testValues[index];  // Pick value from test sequence
        MessagePtr message = QSharedPointer<GsmSignalStrengthUpdate>(new GsmSignalStrengthUpdate(testValue));
        m_CBusComm->push(message);

        index = (index + 1) % testValues.size(); // Move to the next value
    });
    timer->setSingleShot(false);
    timer->start(120000);  // 2 mins

    // if(response.indexOf("CSQ")) //AT+CSQ Response
    // {
    //     //parsing gsm strength response
    //     QString data = QString(response).trimmed();
    //     QRegularExpression regex(R"(\+CSQ:\s*(\d+),(\d+))");
    //     QRegularExpressionMatch match = regex.match(data);

    //     int rssi,ber;
    //     if (match.hasMatch())
    //     {
    //         rssi = match.captured(1).toInt();
    //         ber = match.captured(2).toInt();

    //         qDebug() << "Extracted RSSI:" << rssi;
    //         qDebug() << "Extracted BER:" << ber;
    //     }

    //     MessagePtr message = QSharedPointer<GsmSignalStrengthUpdate>(new GsmSignalStrengthUpdate(rssi));
    //     //broad cast gsm signal strength
    //     m_CBusComm->push(message);

    // }
    // else
    // {
    //     qDebug() << " CSQ not found";
    // }
}

void Gsmservice::writeData(QByteArray data)
{
    qDebug() << __FUNCTION__;
    m_serial->write(data);
}

void Gsmservice::handleError()
{
    qDebug() << __FUNCTION__ << m_serial->errorString();
}

void Gsmservice::CheckSignalStrength()
{
    str = "AT+CSQ\r";
    qDebug() << __FUNCTION__ << str;
    writeData(str.toUtf8());
}

Gsmservice::~Gsmservice() {}

Gsmservice& Gsmservice::getInstance() {
    static Gsmservice instance;
    return instance;
}

void Gsmservice::initialise() {

    // Publish
    m_CBusComm->initialise();
    qDebug() << "Gsmservice " << __FUNCTION__;

    connect(m_CBusComm.data(), SIGNAL(startupService(QSharedPointer<InitializationMessage const>)),
            this, SLOT(processStart(QSharedPointer<InitializationMessage const>)));

    connect(&m_signalStrength,&QTimer::timeout,this,&Gsmservice::CheckSignalStrength);

    //starting timer to check signal strength
    m_signalStrength.start();
}

void Gsmservice::processStart(QSharedPointer<const InitializationMessage>)
{
    qDebug() << __FUNCTION__;

    //starting timer to check signal strength
    //m_signalStrength.start();
}
