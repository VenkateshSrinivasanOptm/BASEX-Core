#ifndef CONNECTIONMANAGERSERVICEMESSAGES_H
#define CONNECTIONMANAGERSERVICEMESSAGES_H

#include <comlibservices.h>
#include <message.h>


struct ConnectWifiMessage : public Message
{
    ConnectWifiMessage() : Message(ConnectWifiMessageMacro)
    {
    }

    virtual ~ConnectWifiMessage() {}

    virtual void serialize(QDataStream &stream) const {}

    virtual void deserialize(QDataStream &stream) {}

};

struct DisconnectWifiMessage : public Message
{
    DisconnectWifiMessage() : Message(DisconnectWifiMessageMacro)
    {
    }

    virtual ~DisconnectWifiMessage() {}

    virtual void serialize(QDataStream &stream) const {}

    virtual void deserialize(QDataStream &stream) {}

};

#endif // CONNECTIONMANAGERSERVICEMESSAGES_H
