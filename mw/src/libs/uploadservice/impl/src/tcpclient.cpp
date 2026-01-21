#include "tcpclient.h"
#include <QtEndian> // For endianness conversion

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{
    m_Socket = new QTcpSocket(this);
    m_Socket->setObjectName("uploadservice");
    connect(m_Socket,&QTcpSocket::connected,this,&TcpClient::Connected);
    connect(m_Socket,&QTcpSocket::disconnected,this,&TcpClient::Disconnected);
    connect(m_Socket,&QTcpSocket::readyRead,this,&TcpClient::ReadyRead);
    connect(m_Socket,&QTcpSocket::errorOccurred,this,&TcpClient::ErrorOccurred);
    connect(m_Socket,&QTcpSocket::stateChanged,this,&TcpClient::StateChanged);
    connect(&m_HeartBeattimer,&QTimer::timeout,this,&TcpClient::SendHeartBeat);
    connect(&m_SystemHealthtimer,&QTimer::timeout,this,&TcpClient::SendSystemHealthInfo);
    connect(&m_BatteryHealthtimer,&QTimer::timeout,this,&TcpClient::SendBatteryHealthInfo);

    m_HeartBeattimer.setInterval(30000);
    m_SystemHealthtimer.setInterval(60000);
    m_BatteryHealthtimer.setInterval(10000);

    m_Socket->connectToHost(QHostAddress("15.206.144.17"),8080);
    //m_Socket->connectToHost("optm-dev.yu-ma.in",8080);
    if (m_Socket->waitForConnected(3000)){
        qDebug("Connected! to Server");
    }
}

void TcpClient::ProcessServerMessage(QByteArray data)
{
}

void TcpClient::SendRegistrationPacket()
{
    qDebug() << " bytes written " << m_Socket->write(QByteArray::fromHex("rawdata"));
    m_Socket->waitForBytesWritten();
    m_Socket->flush();
}

void TcpClient::Connected()
{
#ifdef DEBUG
    qDebug() << __FUNCTION__;
#endif
    qDebug() << "Connected To Server  " << sender()->objectName();
    if(!m_Socket->isOpen())
    {
        return;
    }
    if(m_Socket->state() == QAbstractSocket::ConnectedState)
    {
        SendRegistrationPacket();
    }
}

void TcpClient::Disconnected()
{
    qDebug() << __FUNCTION__ << m_Socket->state();
    m_registerToService = false;
    m_HeartBeattimer.stop();
    //m_SystemHealthtimer.stop();
    //m_BatteryHealthtimer.stop();
}

void TcpClient::ReadyRead()
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ ;
#endif
    quint64 dataSize = m_Socket->bytesAvailable();
    qDebug() << "bytes available " << dataSize;
    while(dataSize){
        QByteArray data = m_Socket->read(sizeof(1));
        ProcessServerMessage(data);
    }
}

void TcpClient::SendHeartBeat()
{
    qDebug() << __FUNCTION__;
}

void TcpClient::SendSystemHealthInfo()
{
    qDebug() << __FUNCTION__;
}

void TcpClient::SendBatteryHealthInfo()
{
    qDebug() << __FUNCTION__;
}

void TcpClient::ErrorOccurred()
{
    qDebug() << __FUNCTION__ << m_Socket->errorString();
    switch(m_Socket->error())
    {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << " QAbstractSocket::ConnectionRefusedError ";
        break;

    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << " QAbstractSocket::RemoteHostClosedError ";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug() << " QAbstractSocket::HostNotFoundError ";
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug() << " QAbstractSocket::SocketAccessError ";
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug() << " QAbstractSocket::SocketResourceError";
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug() << " QAbstractSocket::SocketTimeoutError";
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug() << " QAbstractSocket::SocketTimeoutError";
        break;

    case QAbstractSocket::NetworkError:
        // qDebug() << " QAbstractSocket::NetworkError occured trying to connect again";
        // m_Socket->connectToHost(QHostAddress("15.206.144.17"),8080);
        // if (m_Socket->waitForConnected(3000)){
        //     qDebug("Connected! to Server");
        // }
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug() << " QAbstractSocket::AddressInUseError";
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug() << " QAbstractSocket::SocketAddressNotAvailableError";
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug() << " QAbstractSocket::UnsupportedSocketOperationError";
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug() << " QAbstractSocket::ProxyAuthenticationRequiredError";
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug() << " QAbstractSocket::SslHandshakeFailedError";
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug() << " QAbstractSocket::UnfinishedSocketOperationError";
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug() << " QAbstractSocket::ProxyConnectionRefusedError";
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug() << " QAbstractSocket::ProxyConnectionClosedError";
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug() << " QAbstractSocket::ProxyConnectionTimeoutError";
        break;
    case QAbstractSocket::ProxyNotFoundError:
        qDebug() << " QAbstractSocket::ProxyNotFoundError";
        break;
    case QAbstractSocket::ProxyProtocolError:
        qDebug() << " QAbstractSocket::ProxyProtocolError";
        break;
    case QAbstractSocket::OperationError:
        qDebug() << " QAbstractSocket::OperationError";
        break;
    case QAbstractSocket::SslInternalError:
        qDebug() << " QAbstractSocket::SslInternalError";
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        qDebug() << " QAbstractSocket::SslInvalidUserDataError";
        break;
    case QAbstractSocket::TemporaryError:
        qDebug() << " QAbstractSocket::TemporaryError";
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug() << " QAbstractSocket::UnknownSocketError";
        break;
    }
}

void TcpClient::StateChanged()
{
    qDebug() << __FUNCTION__ << m_Socket->state();
    if(QAbstractSocket::UnconnectedState)
    {
        m_registerToService = false;
        m_HeartBeattimer.stop();
        //m_SystemHealthtimer.stop();
       // m_BatteryHealthtimer.stop();
    }
}







