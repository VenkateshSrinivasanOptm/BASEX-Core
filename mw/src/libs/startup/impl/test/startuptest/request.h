#ifndef REQUEST_H
#define REQUEST_H

#include<QTest>

#include <deserializer.h>
#include <messagehandler.h>
#include <subscriberIntf.h>

class CBusInterface;

class Request:
        public Deserializer, public MessageHandler,
        public SubscriberIntf
{

public:
    Request();
    virtual ~Request();
    bool messageHandler(quint16 requestId, MessagePtr message);
    bool messageHandler(MessagePtr message);
    MessagePtr deserialize(quint16 type, QDataStream &stream);
    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);
    bool subscriberMessageHandler(MessagePtr message);
    void test();

private:

    CBusInterface &cbus;
    quint8 step;
    void error(quint16 line);
    bool configured;
    bool dtvlineup;
    bool epg;
};

#endif // REQUEST_H
