#include "subscriber.h"
#include "pubMessage.h"

#include <QDebug>


Subscriber::Subscriber()
{

}

MessagePtr Subscriber::subscriberDeserialize(quint16 messageType, QDataStream &stream)
{
    PubMessage *msg = new PubMessage(PUB_MESSAGE);
    msg->deserialize(stream);
    return MessagePtr(msg);
}

bool Subscriber::subscriberMessageHandler(MessagePtr message)
{
    QSharedPointer<PubMessage const> msg=message.staticCast<PubMessage const>();
    qDebug() << " incoming message " << msg->messageType << " " << msg->s << " " << msg->i;
    return true;
}
