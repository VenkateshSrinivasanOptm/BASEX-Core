#include "server.h"

#include<cbusinterface.h>
#include"responsemessage.h"

Server::Server(qint32 i):
        cbus(CBusInterface::instance()),
        data(QString(i,'A'))
{
    cbus.registerService(SERVER_ID,this,this);
}

MessagePtr Server::deserialize(quint16 type, QDataStream &stream)
{
    if(type==REQUEST_ID)
    {
        return MessagePtr(new RequestMessage());
    }

    return MessagePtr();
}

bool Server::messageHandler(MessagePtr message)
{
    return false;
}

bool Server::messageHandler(quint16 id, MessagePtr message)
{
    if(message->messageType==REQUEST_ID)
    {
        cbus.sendReply(id, MessagePtr(new ResponseMessage(data)));
        return true;
    }
    return false;
}

