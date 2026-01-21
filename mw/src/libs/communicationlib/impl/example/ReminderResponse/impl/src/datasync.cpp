#include "datasync.h"
#include <remindermessage.h>
#include <responsemessage.h>

#include<QDebug>

#define DATASYNC 1

DataSync::DataSync():
        cbus(CBusInterface::instance())
{
    connect(this,SIGNAL(setReminder(quint16,quint16,quint16)),
            this,SLOT(reminder(quint16,quint16,quint16)));

    // Register our service
    cbus.registerService(DATASYNC,this,this);
}

MessagePtr DataSync::deserialize(quint16 messageType, QDataStream &stream)
{
    if(messageType==DTV_SETREMINDER)
    {
         SetReminder *message = new SetReminder(DTV_SETREMINDER);
         message->deserialize(stream);
         return MessagePtr(message);
    }

    return MessagePtr();
}


/**
 *   this method runs in the thread of the communication library
 *   always use slots in the implementations, so that it will be
 *   executed in the thread of the services
 **/
bool DataSync::messageHandler(quint16 requestId, MessagePtr message) {

    if(message->messageType==DTV_SETREMINDER){
        QSharedPointer<SetReminder const> reminder=message.staticCast<SetReminder const>();
        emit setReminder(requestId,reminder->channelId,reminder->programId);
        return true;
    }
    return false;
}


void DataSync::reminder(quint16 requestId, quint16 channelId, quint16 programId)
{
    // send a response back

    qDebug() << "setreminder" << channelId << " " << programId;
    Message *resp=new ReminderResp(DATASYNC_REMINDERRESP,200);
    qDebug() << "send response  for request " << requestId;
    cbus.sendReply(requestId,MessagePtr(resp));
}
