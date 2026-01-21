#ifndef SHORTMESSAGE_H
#define SHORTMESSAGE_H

#include<message.h>

struct shortMessage: public Message
{

    quint8 i;
    shortMessage(quint16 _type):  Message(_type){}
    shortMessage(quint16 _type, quint8 _i):
            Message(_type), i(_i) {}
    virtual ~shortMessage(){}
    virtual void serialize(QDataStream &stream) const
            {
               stream << i;
            }
   virtual void deserialize(QDataStream &stream)
            {
              stream >> i;
            }
};

#endif // SHORTMESSAGE_H
