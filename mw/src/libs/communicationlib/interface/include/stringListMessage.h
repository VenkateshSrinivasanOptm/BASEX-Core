#ifndef STRINGLLISTMESSAGE_H
#define STRINGLLISTMESSAGE_H

#include<message.h>

struct StringListMessage: public Message
{

    QStringList list;
    StringListMessage(quint16 _type): Message(_type) {}
    StringListMessage(quint16 _type, QStringList const &_list):
            Message(_type), list(_list) {}
    virtual ~StringListMessage(){}
    virtual void serialize(QDataStream &stream) const
            {
               stream << list;
            }
   virtual void deserialize(QDataStream &stream)
            {
              stream >>  list;
            }
};

#endif // STRINGLLISTMESSAGE_H
