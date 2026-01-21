#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{
    m_Socket = new QTcpSocket(this);
    connect(m_Socket,&QTcpSocket::connected,this,&TcpClient::Connected);
    connect(m_Socket,&QTcpSocket::disconnected,this,&TcpClient::Disconnected);
    connect(m_Socket,&QTcpSocket::readyRead,this,&TcpClient::ReadyRead);
    connect(m_Socket,&QTcpSocket::errorOccurred,this,&TcpClient::ErrorOccurred);
    connect(m_Socket,&QTcpSocket::stateChanged,this,&TcpClient::StateChanged);

    m_Socket->connectToHost(QHostAddress("192.168.231.130"),15000);
    if (m_Socket->waitForConnected(3000))
        qDebug("Connected! to Server");
}

void TcpClient::SendRegistrationPacket()
{
    QByteArray data;
    m_Socket->write(data);
    m_Socket->waitForBytesWritten();
    m_Socket->flush();

}


void TcpClient::Connected()
{
#ifdef DEBUG
    qDebug() << __FUNCTION__;
#endif
    qDebug() << "Connected To Server  " << sender()->objectName(); //QHostInfo::hostName();
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
}

void TcpClient::ReadyRead()
{
    qDebug() << __FUNCTION__ << m_Socket->bytesAvailable();
    quint64 bytes = m_Socket->bytesAvailable();

    while(bytes){
        // QByteArray data = m_Socket->read(SERVERPACKET_SIZE);
        // ProcessServerMessage(data);
        // bytes -= SERVERPACKET_SIZE;
    }
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
        qDebug() << " QAbstractSocket::NetworkError";
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
    }
}
