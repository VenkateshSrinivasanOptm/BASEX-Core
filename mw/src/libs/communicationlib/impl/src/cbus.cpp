#include "cbus.h"
#include "definitions.h"
#include "cbusconfigprivate.h"
#include "outgoingworker.h"
#include "listMessage.h"
#include <QDebug>
#include <iostream>
#include <performance.h>

CBus::CBus():
    externalComm(*this), inRequestId(0), outRequestId(0)
{
    init();
}

CBus::~CBus()
{
    stop();
}

void CBus::forwardObject(SendObjectPtr const &obj)
{
    switch(obj->type)
    {
    case NOREPLYCALL:
        externalComm.call(obj->module,obj->serviceName,obj->msg);
        break;

    case EXTERNALASYNCALL:
        externalComm.asynCall(obj->module,obj->serviceName,obj->requestId,obj->msg);
        break;

    case EXTERNALREPLY:
        externalComm.sendReply(obj->module,obj->requestId,obj->msg);
        break;

    case REGISTER:
        externalComm.sendRegisterService(obj->serviceName);
        break;

    case UNREGISTER:
        externalComm.sendUnRegisterService(obj->serviceName);
        break;

    case PUBLISH:
#ifdef DEBUG
        qDebug() << "forward message for other processes";
#endif
        externalComm.publish(obj->msg);
#ifdef DEBUG
        qDebug() << "forward message for own process";
#endif
        inCommingPublish(obj->msg);
        break;

    case INTERNALASYNCALL:
        inCommingAsyncCall(obj->serviceName,obj->requestId,obj->msg);
        break;

    case INTERNALREPLY:
        inCommingReply(obj->requestId,obj->msg);
        break;

    case REGISTERSERVICES:
        externalComm.sendServices(obj->module,obj->msg);
        break;

    case INTERNALNOREPLYCALL:
        inCommingNoReplyCall(obj->serviceName,obj->msg);
        break;

    case GETSERVICES:
        externalComm.getServices();
        break;

    }
}

bool CBus::addServices(quint8 name, Deserializer *d, MessageHandler *h)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " register service " << name;
#endif

    intServLock.lockForWrite();
    if(internalServices.contains(name))
    {
        intServLock.unlock();
        return false;
    }
    else
    {
        internalServices.insert(name,Callee(d,h));
        intServLock.unlock();
        sendRegisterService(name);
        return true;
    }
}

bool CBus::removeServices(quint8 name)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " unregister service " << name;
#endif

    intServLock.lockForWrite();
    quint8 i=internalServices.remove(name);
    intServLock.unlock();
    sendUnRegisterService(name);
    return i>0 ? true:false;
}

bool CBus::unsubscribe(SubscriberIntf *subscriber)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ ;
#endif

    quint16 nbr=0;
    subscriberLock.lockForWrite();
    QList<quint16> keys=messageSubscriberMap.uniqueKeys();
    foreach(quint16 const key,keys)
    {
        nbr+=messageSubscriberMap.remove(key,subscriber);
    }
    subscriberLock.unlock();
    return nbr>0 ? true : false;
}


quint16 CBus::generateUniqueOutRequestId()
{
    quint16 i;
    indexLock.lock();
    if(outRequestId>500) {
        outRequestId=0;
    }
    ++outRequestId;
    i=outRequestId;
    indexLock.unlock();
    return i;
}

quint16 CBus::generateUniqueInRequestId()
{
    quint16 i;
    indexLock.lock();
    if(inRequestId>500) {
        inRequestId=0;
    }
    ++inRequestId;
    i=inRequestId;
    indexLock.unlock();
    return i;
}

void CBus::subscribe(SubscriberIntf *subscriber, QList<quint16> const &messageType)
{
    quint16 type;
    subscriberLock.lockForWrite();
    foreach(type,messageType)
    {
        messageSubscriberMap.insert(type,subscriber);
#ifdef DEBUG
        qDebug() << __FUNCTION__ << ":" << __LINE__ << "subscribe for message " << type;
#endif \
    // externalComm->subscribe(type);
    }
    subscriberLock.unlock();
}

// callback implementations
void CBus::registerExternalService(quint8 service, QString const &moduleName)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__
             << " register external service " << service << " from process " <<moduleName;
#endif
    extServLock.lockForWrite();
    externalServices.insert(service,moduleName);
    extServLock.unlock();
}

void CBus::unregisterExternalService(quint8 service)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " unregister external service " << service;
#endif
    extServLock.lockForWrite();
    externalServices.remove(service);
    extServLock.unlock();
}

void CBus::inCommingNoReplyCall(quint8 service, QDataStream &stream)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__
             << " we have an incomming no reply call for service " << service;
#endif
    intServLock.lockForRead();
    QHash<quint8,Callee>::const_iterator itr=internalServices.constFind(service);
    if(itr!= internalServices.constEnd())
    {
        quint16 type;
        stream >> type;
        TRACE("CBUS: incomming noreply call for service " << (quint16)service << " type " << type);
        MessagePtr msg=(itr.value().d)->deserialize(type,stream);
        if(!msg.isNull()){
            (itr.value().h)->messageHandler(msg);
        }
        else
        {
            qDebug() << __FUNCTION__ << ":" << __LINE__ << " no message of type " << type << " defined";
        }
        TRACE("CBUS: end incomming noreply call for service "<< (quint16)service << " type " << type);
    }
    intServLock.unlock();  
}

void CBus::inCommingAsyncCall(QString const &rmodule, quint8 service, quint16 requestId, QDataStream &stream)
{
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__
             << " we have an asynchronous call for service " << service
             << " from process " << rmodule;
#endif
    intServLock.lockForRead();
    QHash<quint8,Callee>::const_iterator itr=internalServices.constFind(service);
    if(itr!= internalServices.constEnd())
    {
        quint16 type;
        stream >> type;
        TRACE("CBUS: incomming reply call for service "<< (quint16)service << " type " << type <<  " requestId " << requestId);
        MessagePtr msg=(itr.value().d)->deserialize(type,stream);
        TRACE("CBUS: end deserialization type " << type);
        if(msg.isNull())
        {
            qDebug() <<  __FUNCTION__ << ":" << __LINE__
                     << " no message of type " << type << " defined";
            intServLock.unlock();
            return;
        }
        quint16 index=generateUniqueInRequestId();
        inCommingCallsLock.lock();
        inCommingCalls.insert(index,inMessage(requestId,rmodule));
        inCommingCallsLock.unlock();
        if(!(itr.value().h)->messageHandler(index,msg))
        {
#ifdef DEBUG
            qDebug() << __FUNCTION__ << ":" << __LINE__
                     << " message of type " << type << " not handled ";
#endif
            inCommingCallsLock.lock();
            inCommingCalls.remove(index);
            inCommingCallsLock.unlock();
        }
        TRACE("CBUS: end incomming reply call for service "<< (quint16)service << " type "
                                                            << type <<  " requestId" << requestId);
    }
    intServLock.unlock();
}

void CBus::inCommingReply(quint16 requestId, QDataStream &stream)
{
#ifdef DEBUG
    qDebug() <<  __FUNCTION__ << ":" << __LINE__
             << " we have an incomming reply with requestId " << requestId;
#endif
    TRACE("CBUS: incomming reply with requestID "<< requestId);
    callBackFnsLock.lock();
    QHash<quint16,QSharedPointer<CallBack> >::iterator itr=callBackFns.find(requestId);
    if(itr==callBackFns.end())
    {
        callBackFnsLock.unlock();
        qDebug() <<  __FUNCTION__ << ":" << __LINE__ << " we didn't found the requestId " << requestId;
        return;
    }
    QSharedPointer<CallBack> callBackFn=itr.value();
    callBackFns.erase(itr);
    callBackFnsLock.unlock();
    quint16 type;
    stream >> type;
    TRACE("CBUS: start deserializing message " << type);
    MessagePtr msg=callBackFn->deserialize(type,stream);
    TRACE("CBUS: end deserializing message " << type);
    if(msg.isNull())
    {
        qDebug() <<  __FUNCTION__ << ":" << __LINE__
                 << " no message of type " << type << " defined";
        return;
    }
    callBackFn->execute(requestId,msg);
    TRACE("CBUS: end incomming reply with requestID "<< requestId);
}

void CBus::inCommingPublish(QDataStream &stream)
{
    quint16 type;
    stream >> type;
#ifdef DEBUG
    qDebug() <<  __FUNCTION__ << ":" << __LINE__
             <<  " we have an incomming publish with message type " << type;

    TRACE("CBUS: incomming publish of type "<< type);
#endif
    subscriberLock.lockForRead();
    QMultiHash<quint16,SubscriberIntf *>::const_iterator itr = messageSubscriberMap.constFind(type);
    QMultiHash<quint16,SubscriberIntf *>::const_iterator end = messageSubscriberMap.constEnd();
    MessagePtr msg;
    while((itr!=end) && (itr.key()==type))
    {
#ifdef DEBUG
        qDebug() <<  __FUNCTION__ << ":" << __LINE__ << " we have a subscriber for message " << type;
#endif
        if(msg.isNull())
        {
            msg=itr.value()->subscriberDeserialize(type,stream);
        }
        if(!msg.isNull())
        {
            itr.value()->subscriberMessageHandler(msg);
        }
        itr++;
    }
    subscriberLock.unlock();
    TRACE(" CBUS: incomming publish of type " << type);
}

void CBus::inCommingAsyncCall(quint8 service, quint16 requestId, MessagePtr const &msg)
{
#ifdef DEBUG
    qDebug() << " we have an internal asynchronous call for service " << service;
#endif
    intServLock.lockForRead();
    QHash<quint8,Callee>::const_iterator itr=internalServices.constFind(service);
    if(itr!= internalServices.constEnd())
    {
        quint16 index=generateUniqueInRequestId();
        inCommingCallsLock.lock();
        inCommingCalls.insert(index,inMessage(requestId));
        inCommingCallsLock.unlock();
        if(!(itr.value().h)->messageHandler(index,msg))
        {
            inCommingCallsLock.lock();
            inCommingCalls.remove(index);
            inCommingCallsLock.unlock();
        }
    }
    intServLock.unlock();

}

void CBus::inCommingNoReplyCall(quint8 service, MessagePtr const &msg){
#ifdef DEBUG
    qDebug() << " we have an internal noreply  call for service " << service;
#endif
    intServLock.lockForRead();
    QHash<quint8,Callee>::const_iterator itr=internalServices.constFind(service);
    if(itr!= internalServices.constEnd())
    {
        itr.value().h->messageHandler(msg);
    }
    intServLock.unlock();
}

void CBus::inCommingReply(quint16 requestId, MessagePtr const &msg)
{
#ifdef DEBUG
    qDebug() << " we have an internal reply with requestId " << requestId
             << " message type " << msg->messageType;
#endif
    callBackFnsLock.lock();
    QHash<quint16,QSharedPointer<CallBack> >::iterator itr=callBackFns.find(requestId);
    if(itr==callBackFns.end())
    {
        callBackFnsLock.unlock();
        return;
    }
    QSharedPointer<CallBack> callBackFn=itr.value();
    callBackFns.erase(itr);
    callBackFnsLock.unlock();
    callBackFn->execute(requestId,msg);
}

void CBus::inCommingPublish(MessagePtr const &msg)
{
    subscriberLock.lockForRead();
    QMultiHash<quint16,SubscriberIntf *>::const_iterator itr = messageSubscriberMap.constFind(msg->messageType);
    QMultiHash<quint16,SubscriberIntf *>::const_iterator end = messageSubscriberMap.constEnd();
    while((itr!=end) && (itr.key()==msg->messageType))
    {
        itr.value()->subscriberMessageHandler(msg);
        itr++;
    }
    subscriberLock.unlock();
}

void CBus::inCommingGetServices(QString const &moduleName)
{
#ifdef DEBUG
    qDebug() << " we have an incomming get services request from process " << moduleName;
#endif
    intServLock.lockForRead();
    QList<quint8> services=internalServices.keys();
    intServLock.unlock();
    ListMessage<quint8> *msg=new ListMessage<quint8>(0,services);
#ifdef DEBUG
    qDebug() << " add the services in queue for process " << moduleName;
#endif
    sendQueue.addToQueue(SendObjectPtr(new SendObject(REGISTERSERVICES,moduleName,MessagePtr(msg),0)));
}

void CBus::inCommingServices(QString const &moduleName, QList<quint8> services)
{
#ifdef DEBUG
    qDebug() << " we have list of incomming services from process" << moduleName;
#endif
    extServLock.lockForWrite();
    foreach(quint8 service,services)
    {
        externalServices.insert(service,moduleName);
    }
    extServLock.unlock();
}

bool CBus::callService(quint8 name, MessagePtr const &msg)
{
#ifdef DEBUG
    qDebug() << " call service " << name << " with message type " << msg->messageType;
#endif

    // check if we do a call to a service that is running in the same process
    intServLock.lockForRead();
    if(internalServices.contains(name))
    {
        // Callee callee(internalServices.value(name));
        intServLock.unlock();
        sendQueue.addToQueue(SendObjectPtr(new SendObject(INTERNALNOREPLYCALL,name,msg,0)));
        return true;
        //  return callee.h->messageHandler(msg); //TODO put this in a queue ?
    }
    intServLock.unlock();

    // check if there is an external service
    extServLock.lockForRead();
    if(externalServices.contains(name))
    {
        TRACE("CBUS: call service " << (quint16)name<< " type " << msg->messageType);
        QString module=externalServices.value(name);
        extServLock.unlock();
        sendQueue.addToQueue(SendObjectPtr(new SendObject(NOREPLYCALL,module,name,msg)));
        return true;
    }
    extServLock.unlock();
    return false;
}

quint16 CBus::asynCallService(quint8 name, MessagePtr const &msg, QSharedPointer<CallBack> callback)
{
#ifdef DEBUG
    qDebug() << " we have an asynchromous call for service " << name << " with message type " << msg->messageType;
#endif
    // check if we have an call to a internal service
    intServLock.lockForRead();
    if(internalServices.contains(name))
    {

        intServLock.unlock();
        quint16 requestId=generateUniqueOutRequestId();
        callBackFnsLock.lock();
        callBackFns.insert(requestId,callback);
        callBackFnsLock.unlock();
        sendQueue.addToQueue(SendObjectPtr(new SendObject(INTERNALASYNCALL,name,msg,requestId)));
        return requestId;
    }
    intServLock.unlock();

    // check if we have a call to a external service
    extServLock.lockForRead();
    if(externalServices.contains(name))
    {

        QString module=externalServices.value(name);
        extServLock.unlock();
        quint16 requestId=generateUniqueOutRequestId();
        TRACE("CBUS: reply call to  service " << (quint16)name
                                              << " type " << msg->messageType << " requestId " << requestId);
        callBackFnsLock.lock();
        callBackFns.insert(requestId,callback);
        callBackFnsLock.unlock();
        sendQueue.addToQueue(SendObjectPtr(new SendObject(EXTERNALASYNCALL,module,name,msg,requestId)));
        return requestId;
    }
    extServLock.unlock();
    return 0;
}

void CBus::push(MessagePtr const &msg)
{
#ifdef DEBUG
    qDebug() << "publish message " << msg->messageType;
#endif
    TRACE("CBUS: publish message of type "<< msg->messageType);
    sendQueue.addToQueue(SendObjectPtr(new SendObject(PUBLISH,msg)));
}

void CBus::sendReply(quint16 requestId, MessagePtr const &message)
{
#ifdef DEBUG
    qDebug() << "send reply message type " << message->messageType << " requestId  " << requestId;
#endif
    inCommingCallsLock.lock();
    QHash<quint16,inMessage>::iterator itr=inCommingCalls.find(requestId);
    if(itr==inCommingCalls.end())
    {
        inCommingCallsLock.unlock();
        qDebug() << " requestId not found: " << requestId;
        return;
    }
    inMessage inmsg=itr.value();
    TRACE("CBUS: send reply for request " << inmsg.requestId);
    inCommingCalls.erase(itr);
    inCommingCallsLock.unlock();
    if(inmsg.internalMsg) {
// we have to send this reply to an internal service
#ifdef DEBUG
        qDebug() << " add reply with requestID " << requestId <<  "to queue for sending to the same process";
#endif
        sendQueue.addToQueue(SendObjectPtr((new SendObject(INTERNALREPLY,message,inmsg.requestId))));
    }
    else
    {
        // we have to send this reply to an external service
#ifdef DEBUG
        qDebug() << " add reply with requestID " << requestId <<  "to queue for sending to an external process "
                 <<  inmsg.module;
#endif
        sendQueue.addToQueue(SendObjectPtr((new SendObject(EXTERNALREPLY,inmsg.module,message,inmsg.requestId))));
    }

}

void CBus::sendRegisterService(quint8 name)
{
#ifdef DEBUG
    qDebug() << "register service " << name << " to other processes";
#endif
    sendQueue.addToQueue(SendObjectPtr(new SendObject(REGISTER,name)));

}

void CBus::sendUnRegisterService(quint8 name)
{
#ifdef DEBUG
    qDebug() << "unregister service " << name << " to other processes";
#endif
    sendQueue.addToQueue(SendObjectPtr(new SendObject(UNREGISTER,name)));
}

void CBus::init()
{
    CBusConfigPrivate::instance().setRunning();
    quint8 nbr=CBusConfigPrivate::instance().getNbrOfOutWorkers();
#ifdef DEBUG
    qDebug() << "start threads for outgoing request " << nbr;
#endif

    for(quint8 i=0; i<nbr; ++i)
    {
        OutGoingWorker *thread=new OutGoingWorker(sendQueue,externalComm,*this);
        thread->start();
        OutworkerThread.append(QSharedPointer<OutGoingWorker>(thread));
    }

    // ask for registered services
    sendQueue.addToQueue(SendObjectPtr(new SendObject(GETSERVICES)));
    // externalComm.getServices();

    // start 0Mq incomming threads
    externalComm.init();
}

void CBus::stop()
{

    foreach(QSharedPointer<OutGoingWorker> const &thread,OutworkerThread)
    {
        thread->stopRunning();
    }

    // notify worker threads to stop
    sendQueue.clearQueue();
}
