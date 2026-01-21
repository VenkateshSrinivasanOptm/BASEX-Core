#ifndef APPMANMESSAGES_H
#define APPMANMESSAGES_H
#include "message.h"
#include "comlibservices.h"

/*************************************************************************************************
  * Messages using the communications library
  *
  * A message interface is required between the Package Manager and the applications manager regard
  * restart after a software download.
  * When the request requires UI permission, the Application Manager uses its stdio message channel to
  * communicate with the UI. The Application Manager handles the retries for this and responds back
  * to teh Package Manager when permision is obtained.
  */

// message PM-AM to request UI permission to reboot. If reboot is granted then the AM will take over.

struct AskUIForRebootRequest: public Message
{
    quint8 restartMode;
    AskUIForRebootRequest():Message(APPMAN_SERVICE_ASK_UI_FOR_REBOOT_REQ) {}
    AskUIForRebootRequest(quint8 _restartMode):Message(APPMAN_SERVICE_ASK_UI_FOR_REBOOT_REQ), restartMode(_restartMode) {}

    virtual ~AskUIForRebootRequest(){}
    virtual void serialize(QDataStream &stream) const
    {
        stream << restartMode;
    }

    virtual void deserialize(QDataStream &stream)
    {
        stream >> restartMode;
    }
};


// PM -> AM. message to force a reboot of the AM.

struct ForceRebootForNewSw: public Message
{
    ForceRebootForNewSw():Message(APPMAN_SERVICE_FORCE_REBOOT_REQ) {}
    virtual ~ForceRebootForNewSw(){}
    virtual void serialize(QDataStream &/*stream*/) const
    {
    }

    virtual void deserialize(QDataStream &/*stream*/)
    {
    }
};

#endif // APPMANMESSAGES_H
