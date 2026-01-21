#ifndef REMINDERMESSAGE_H
#define REMINDERMESSAGE_H

#include<QByteArray>
#include<QDataStream>

#include<message.h>

#define DTV_SETREMINDER 1


struct SetReminder: public Message
{
    quint16 channelId;
    quint16 programId;

    SetReminder(quint16 type):Message(type) {}
    SetReminder(quint16 _channelId, quint16 _programId):
            Message(DTV_SETREMINDER), channelId(_channelId), programId(_programId) {}
    virtual ~SetReminder(){}
    virtual void serialize(QDataStream &stream) const
            {

               stream <<  channelId << programId;

            }
   virtual void deserialize(QDataStream &stream)
            {

              stream >> channelId >> programId;
            }
};





#endif // REMINDERMESSAGE_H
