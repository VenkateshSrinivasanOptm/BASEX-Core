#include "dtv.h"
#include<responsemessage.h>
#include<remindermessage.h>
#include<callbackgen.h>


#include <QDebug>

#define DATASYNC 1

Dtv::Dtv():
        cbus(CBusInterface::instance())
{

}

MessagePtr Dtv::deserialize(quint16 messageType, QDataStream &stream)
{
         Message *message = new ReminderResp(messageType);
         message->deserialize(stream);
         return MessagePtr(message);
}


bool Dtv::callback(quint16 requestId, MessagePtr reply)
{
   // if this is a long running task you have to run this in another thread
   QSharedPointer<ReminderResp const> s=reply.staticCast<ReminderResp const >();
   qDebug() << "reply " << requestId << " " << s->messageType << " " << s->errorCode;
   return true;
}

/*
 * this method sends a setReminder to the datasync service
 */
void Dtv::setReminder(quint16 programId, quint16 channelId)
{

    MessagePtr message = MessagePtr(new SetReminder(programId,channelId));
    QSharedPointer<CallBack> callBack =
            QSharedPointer<CallBack>(new CallBackGen<Dtv>(this,&Dtv::callback,&Dtv::deserialize));
    quint16 requestId;
    do {
//    qDebug() << "request Id " << requestId;
    requestId=cbus.callServiceWithCallback(DATASYNC,message,callBack);

//    qDebug() << "send request to DS" ;
    }while( requestId==0);

    // do something with requestId
}
