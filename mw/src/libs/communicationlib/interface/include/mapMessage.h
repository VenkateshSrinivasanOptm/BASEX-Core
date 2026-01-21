#ifndef MAPMESSAGE_H
#define MAPMESSAGE_H

#include<message.h>
#include<QMap>

template<typename key, typename T> struct MapMessage: public Message
{

    QMap<key,T> map;
    MapMessage(quint16 _type): Message(_type) {}
    MapMessage(quint16 _type, QMap<key,T> const &_map):
            Message(_type), map(_map) {}
    virtual ~MapMessage(){}
    virtual void serialize(QDataStream stream) const
            {
               stream << map;
            }
   virtual void deserialize(QDataStream stream)
            {
              stream >> map;
            }
};

#endif // MAPMESSAGE_H
