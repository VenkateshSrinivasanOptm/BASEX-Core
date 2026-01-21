#include "pushmessagetest.h"
#include<cbusinterface.h>

#include <QDebug>

PushMessageTest::PushMessageTest():
        cbus(CBusInterface::instance())
{
}

void PushMessageTest::initTestCase()
{
    QTest::qSleep(3000); // wait for starting the threads
    QList<quint16> list;
    list << 1;
    cbus.subscribe(this,list);
}

void PushMessageTest::pushTest()
{
    PushMessage *msg=new PushMessage();
    cbus.push(MessagePtr(msg));
    QTest::qSleep(4000);
}

MessagePtr PushMessageTest::subscriberDeserialize(quint16 messageType, QDataStream &stream)
{
    return MessagePtr();
}

bool PushMessageTest::subscriberMessageHandler(MessagePtr message)
{
    qDebug() << "we have an incomming message " << message->messageType;
    test(message);
    return true;
}

void PushMessageTest::test(MessagePtr message)
{
    QVERIFY(message->messageType == 1);
}

QTEST_MAIN(PushMessageTest)
