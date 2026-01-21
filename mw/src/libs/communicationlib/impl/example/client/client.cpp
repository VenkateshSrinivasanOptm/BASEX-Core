#include "client.h"

#include<cbusinterface.h>
#include<responsemessage.h>
#include<callbackgen.h>
#include<QDebug>

Client::Client(quint16 _total):
        cbus(CBusInterface::instance()),
        number(0), total(_total)
{
    t.start();
    sendRequest();
}

MessagePtr Client::deserialize(quint16 type, QDataStream &stream)
{
    ResponseMessage *resp = new ResponseMessage();
    resp->deserialize(stream);
    return MessagePtr(resp);
}

bool Client::messageHandler(quint16 requestId, MessagePtr message)
{
    if(number<total)
    {
        ++number;
        sendRequest();
    }
    else
    {
        qDebug() << "time " << t.elapsed();
    }
    return true;
}

void Client::sendRequest()
{
    qDebug() << "Madhur ";
    cbus.callServiceWithCallback(SERVER_ID,MessagePtr(new RequestMessage()),QSharedPointer<CallBack>(
            new CallBackGen<Client>(this,&Client::messageHandler, &Client::deserialize)));

}
