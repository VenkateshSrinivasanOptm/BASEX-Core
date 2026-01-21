#ifndef UNASSIGNEDMESSAGE_H
#define UNASSIGNEDMESSAGE_H

#include<message.h>
#include<comlibservices.h>

struct UnAssignedMessage: public Message
{
    UnAssignedMessage(): Message(BLSTARTUP_UNASSIGNED) {}
    virtual ~UnAssignedMessage() {}
    void serialize(QDataStream &) const {}
    void deserialize(QDataStream &) {}
};
#endif // UNASSIGNEDMESSAGE_H
