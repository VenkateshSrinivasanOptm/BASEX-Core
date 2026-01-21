#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include<QByteArray>
#include<QDataStream>

#include<message.h>

#define DATASYNC_REMINDERRESP 2


struct ReminderResp: public Message
{
    quint8 errorCode;

    ReminderResp(quint16 type):Message(type) {}
    ReminderResp(quint16 type, quint8 _errorCode):
            Message(type), errorCode(_errorCode) {}
    virtual ~ReminderResp(){}
    virtual void serialize(QDataStream &stream) const
            {

               stream << errorCode;
            }
   virtual void deserialize(QDataStream &stream)
            {
              stream >> errorCode;
            }
};
#endif // RESPONSEMESSAGE_H
