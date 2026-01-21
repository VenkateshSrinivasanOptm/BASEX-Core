#ifndef RESPONSEMESSAGE_H
#define RESPONSEMESSAGE_H

#include<message.h>

#define SERVER_ID 1

#define REQUEST_ID 100
#define RESPONSE_ID 101

struct RequestMessage : public Message
{
    RequestMessage() : Message(REQUEST_ID) {}
    virtual ~RequestMessage() {}
    void serialize(QDataStream &stream) const {}
    void deserialize(QDataStream &stream) {}
};

struct ResponseMessage : public Message
{
    QString data;
    ResponseMessage() : Message(RESPONSE_ID) {}
    ResponseMessage(QString const &_data): Message(RESPONSE_ID), data(_data) {}
    virtual ~ResponseMessage() {}
    void serialize(QDataStream &stream) const
    {
        stream << data;
    }
    void deserialize(QDataStream &stream)
    {
        stream >> data;
    }
};

#endif // RESPONSEMESSAGE_H
