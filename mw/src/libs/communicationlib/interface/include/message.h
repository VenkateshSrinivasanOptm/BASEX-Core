#ifndef MESSAGE_H
#define MESSAGE_H

#include<QSharedPointer>
#include<QDataStream>


struct Message;
typedef QSharedPointer<Message const> MessagePtr;

struct Message{

    quint16 messageType;
    Message(){}
    Message(quint16 _messageType) : messageType(_messageType) {}
    virtual ~Message() {}
    virtual void serialize(QDataStream &stream) const=0;
    virtual void deserialize(QDataStream &stream)=0;
};

#endif // MESSAGE_H
