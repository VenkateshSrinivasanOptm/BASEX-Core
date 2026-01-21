#ifndef SYNCHRONIZEDDATAMESSAGE_H
#define SYNCHRONIZEDDATAMESSAGE_H

#include<message.h>
#include<QMetaType>

/**
  * this message will be send to the services for start downloading of the unicast messages.
  */
/*
struct SynchronizedDataReq: public Message
{
    SynchronizedDataReq(): Message(BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_REQ) {}
    virtual ~SynchronizedDataReq() {}
    void serialize(QDataStream &) const {}
    void deserialize(QDataStream &) {}
};

struct SynchronizedDataResp: public Message
{
    enum Type {
        NO_ERROR,
        NETWORK_ERROR,
        UNKNOWN_ERROR
    };

    Type error;
    SynchronizedDataResp(): Message(BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_RESP) {}
    SynchronizedDataResp(Type _error): Message(BLSTARTUP_SYNCHRONIZED_UNICAST_DOCUMENTS_RESP), error(_error) {}
    virtual ~SynchronizedDataResp() {}
    void serialize(QDataStream & stream) const
    {
       stream << error;
    }
    void deserialize(QDataStream &stream)
    {
       quint8 i;
       stream >> i;
       error=(Type)i;
    }
};

Q_DECLARE_METATYPE(SynchronizedDataResp::Type)
*/
#endif // SYNCHRONIZEDDATAMESSAGE_H
