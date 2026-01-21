#ifndef CLIENT_H
#define CLIENT_H

#include<message.h>
#include<QTime>

class CBusInterface;

class Client
{
public:
    Client(quint16 t);
    MessagePtr  deserialize(quint16 type, QDataStream &stream);
    bool messageHandler(quint16 requestId, MessagePtr message);

 private:
    void sendRequest();
    CBusInterface &cbus;
    quint16 number;
    quint16 total;
    QTime t;

};

#endif // CLIENT_H
