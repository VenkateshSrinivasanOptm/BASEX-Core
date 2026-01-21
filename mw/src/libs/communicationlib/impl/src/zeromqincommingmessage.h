#ifndef ZEROMQINCOMINGMESSAGE_H
#define ZEROMQINCOMINGMESSAGE_H

#include<QByteArray>

class CBusCallBackIntf;

class ZeroMqIncommingMessage
{
public:
    ZeroMqIncommingMessage(CBusCallBackIntf &_cbus);
    virtual ~ZeroMqIncommingMessage(){}
    void handleZmqIncommingMessage(QByteArray const &data);
    void handleZmqIncommingPublishedMessage(QByteArray const &data);

private:
    Q_DISABLE_COPY(ZeroMqIncommingMessage)
    CBusCallBackIntf &cbus;
};

#endif // ZEROMQINCOMINGMESSAGE_H
