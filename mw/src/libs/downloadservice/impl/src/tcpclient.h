#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QHostInfo>
#include <iostream>
#include "comlibservices.h"

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    void ProcessServerMessage(QByteArray);
    void SendRegistrationPacket();
    quint8 isValidTypeID(uint8_t value);

public slots:
    void Connected();
    void Disconnected();
    void ReadyRead();
    void ErrorOccurred();
    void StateChanged();

signals:

private:
    QTcpSocket *m_Socket;
};

#endif // TCPCLIENT_H
