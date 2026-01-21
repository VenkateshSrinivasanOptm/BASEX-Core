#ifndef STRINGMESSAGE_H
#define STRINGMESSAGE_H

#include<message.h>

struct StringMessage: public Message
{

    QString s;
    StringMessage(quint16 _type): Message(_type) {}
    StringMessage(quint16 _type, QString const &_s):
            Message(_type), s(_s) {}
    virtual ~StringMessage(){}
    virtual void serialize(QDataStream &stream) const
            {
               stream << s;
            }
   virtual void deserialize(QDataStream &stream)
            {
              stream >> s;
            }
};
#endif // STRINGMESSAGE_H
