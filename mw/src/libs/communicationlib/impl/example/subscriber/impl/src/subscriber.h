#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include<subscriberIntf.h>

class Subscriber: public SubscriberIntf
{
public:
    Subscriber();
    virtual ~Subscriber() {}
    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);
    bool subscriberMessageHandler(MessagePtr message);
};

#endif // SUBSCRIBER_H
