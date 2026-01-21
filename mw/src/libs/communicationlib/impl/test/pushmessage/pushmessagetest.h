#ifndef PUSHMESSAGETEST_H
#define PUSHMESSAGETEST_H

#include<QTest>
#include <subscriberIntf.h>

struct PushMessage: public Message
{
    PushMessage() : Message(1){}
    virtual ~PushMessage() {}
    void serialize(QDataStream &) const {}
    void deserialize(QDataStream &){}
};

class CBusInterface;

class PushMessageTest:public QObject,
   public SubscriberIntf

{
    Q_OBJECT
public:
    PushMessageTest();
    virtual ~PushMessageTest(){}

    MessagePtr subscriberDeserialize(quint16 messageType, QDataStream &stream);
    bool subscriberMessageHandler(MessagePtr message);

private slots:
    void initTestCase();
    void pushTest();

private:
    CBusInterface &cbus;
    void test(MessagePtr message);
};

#endif // PUSHMESSAGETEST_H
