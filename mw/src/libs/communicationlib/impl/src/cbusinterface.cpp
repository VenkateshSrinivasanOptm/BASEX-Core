#include "cbusinterface.h"
#include "cbus.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "callback.h"
#include "subscriberIntf.h"

CBusInterface::CBusInterface()
{
    cbus= new CBus();
}

CBusInterface::~CBusInterface()
{
    //TOD stop running threads on CBUS
  delete cbus;
}

bool CBusInterface::callService(quint8 serviceName, MessagePtr const &message)
{
    return cbus->callService(serviceName, message);
}

quint16 CBusInterface::callServiceWithCallback(quint8 serviceName, MessagePtr const &message,
                            QSharedPointer<CallBack> callback)
{
     return cbus->asynCallService(serviceName,message,callback);
}

void CBusInterface::push(MessagePtr const &message)
{
    cbus->push(message);
}

bool CBusInterface::registerService(quint8 serviceName, MessageHandler *handler,
                                    Deserializer *deserializer)
{
    return cbus->addServices(serviceName,deserializer,handler);
}


void CBusInterface::subscribe(SubscriberIntf *subscriber, QList<quint16> const &messageType)
{
   cbus->subscribe(subscriber,messageType);
}

void CBusInterface::sendReply(quint16 requestId, MessagePtr const &message)
{
   cbus->sendReply(requestId,message);
}


bool CBusInterface::unsubscribe(SubscriberIntf *subscriber)
{
    return cbus->unsubscribe(subscriber);
}

bool CBusInterface::unregisterService(quint8 serviceName)
{
    return cbus->removeServices(serviceName);
}
