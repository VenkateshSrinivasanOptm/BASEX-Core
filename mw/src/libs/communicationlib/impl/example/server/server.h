#ifndef SERVER_H
#define SERVER_H

#include<deserializer.h>
#include<messagehandler.h>

class CBusInterface;

class Server:
        public Deserializer, public MessageHandler
{

public:
    Server(qint32 i);
    MessagePtr deserialize(quint16 messageType, QDataStream &stream);
    bool messageHandler(MessagePtr message);
    bool messageHandler(quint16 requestId,MessagePtr message);

private:

    CBusInterface &cbus;
    QString data;

};

#endif // SERVER_H
