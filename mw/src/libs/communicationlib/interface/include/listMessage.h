#ifndef LISTMESSAGE_H
#define LISTMESSAGE_H

#include<message.h>
#include<QList>

template<typename T> struct ListMessage: public Message
{

    QList<T> list;
    ListMessage(quint16 _type): Message(_type) {}
    ListMessage(quint16 _type, QList<T> const &_list):
            Message(_type), list(_list) {}
    virtual ~ListMessage(){}
    virtual void serialize(QDataStream &stream) const
            {
               stream << list;
            }
   virtual void deserialize(QDataStream &stream)
            {
              stream >>  list;
            }
};

#endif // LISTMESSAGE_H
