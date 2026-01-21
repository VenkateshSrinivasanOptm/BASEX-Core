#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QHostInfo>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    void ProcessServerMessage(QByteArray);
    void SendRegistrationPacket();

public slots:
    void Connected();
    void Disconnected();
    void ReadyRead();
    void SendHeartBeat();
    void SendSystemHealthInfo();
    void SendBatteryHealthInfo();
    void ErrorOccurred();
    void StateChanged();


private:
    QTcpSocket *m_Socket;
    QTimer m_HeartBeattimer;
    QTimer m_SystemHealthtimer;
    QTimer m_BatteryHealthtimer;
    bool m_registerToService;
};

#endif // TCPCLIENT_H
