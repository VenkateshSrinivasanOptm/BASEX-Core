#ifndef AUTOCONFIGMESSAGE_H
#define AUTOCONFIGMESSAGE_H

#include<message.h>
#include<comlibservices.h>
/*
struct AutoConfigReq : public Message
{
    QString accountNumber;
    QString pin;
    QString stbName;
    AutoConfigReq(): Message(BLSTARTUP_AUTOCONFIG_REQ) {}
    AutoConfigReq(QString _accountNumber, QString _pin , QString _stbName):
       Message(BLSTARTUP_AUTOCONFIG_REQ), accountNumber(_accountNumber), pin(_pin), stbName(_stbName) {}
    virtual ~AutoConfigReq(){}
    virtual void serialize(QDataStream &stream) const
    {
        stream << accountNumber << pin << stbName;
    }
    virtual void deserialize(QDataStream &stream)
    {
        stream >> accountNumber >> pin >> stbName;
    }
 };

struct AutoConfigResponse : public Message
{
    enum Type{
        NO_ERROR,
        INVALID_PIN,
        INVALID_ACCOUNT_NUMBER,
        REGISTRATION_FAILED,
        ACCOUNT_LOCKED,
        NETWORK_ERROR,
        UNKNOWN_ERROR
    };

    Type error;
    AutoConfigResponse(): Message(BLSTARTUP_AUTOCONFIG_RESP) {}
    AutoConfigResponse(Type _error):
        Message(BLSTARTUP_AUTOCONFIG_RESP), error(_error) {}
    virtual ~AutoConfigResponse(){}
    virtual void serialize(QDataStream &stream) const
    {
        stream << error;
    }
    virtual void deserialize(QDataStream &stream)
    {
        quint8 i;
        stream >> i;
        error=(Type)i;
    }
};
*/
#endif // AUTOCONFIGMESSAGE_H
