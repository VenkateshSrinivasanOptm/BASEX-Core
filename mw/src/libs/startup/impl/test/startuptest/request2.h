#ifndef REQUEST2_H
#define REQUEST2_H

#include<QTest>

#include <deserializer.h>
#include <messagehandler.h>
#include <subscriberIntf.h>

class CBusInterface;

class Request2:
        public Deserializer, public MessageHandler,
        public SubscriberIntf
{

public:
    Request2();
    bool messageHandler(quint16 requestId, MessagePtr message);
    bool messageHandler(MessagePtr message);
    MessagePtr deserialize(quint16 type, QDataStream &stream);
    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);
    bool subscriberMessageHandler(MessagePtr message);
    void test();

private:

    CBusInterface &cbus;
    void error(quint16 line);
    bool unconfigured;
    quint8 step;
};

#endif // REQUEST2_H
