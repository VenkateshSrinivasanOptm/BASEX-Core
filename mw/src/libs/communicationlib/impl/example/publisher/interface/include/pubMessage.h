#ifndef PUBMESSAGE_H
#define PUBMESSAGE_H

#include <message.h>

#define PUB_MESSAGE 1

struct PubMessage: public Message
{
    QString s;
    quint16 i;

    PubMessage(quint16 type):Message(type) {}
    PubMessage(quint16 _type, QString _s, quint16 _i):
            Message(_type), s(_s), i(_i) {}
    virtual ~PubMessage(){}
    virtual void serialize(QDataStream &stream) const
            {

               stream <<  s << i;

            }
   virtual void deserialize(QDataStream &stream)
            {

              stream >> s >> i;
            }
};

#endif // PUBMESSAGE_H
