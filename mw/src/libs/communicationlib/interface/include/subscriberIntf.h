#ifndef SUBSCRIBERINTF_H
#define SUBSCRIBERINTF_H

#include<message.h>

class SubscriberIntf {

public:

    virtual ~SubscriberIntf() {}

    /**
      * this method is called when an incoming mesage needs to be deserialized
      **/
    virtual MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream)=0;

    /**
      * This method is called when a  message is received .
      * Since this method is called in the thread of the communication library
      * this method should return immediately
      **/
    virtual bool subscriberMessageHandler(MessagePtr message)=0;


};

#endif // SUBSCRIBERINTF_H
