#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include<message.h>

class Deserializer
{
public:
    virtual ~Deserializer(){}

    /**
      * When a message needs to be deserialized,this method is called
      **/
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &stream)=0;
};

#endif // DESERIALIZER_H
