#ifndef CBUS_H
#define CBUS_H

#include<QHash>
#include<QMultiHash>
#include<QMutex>
#include<QReadWriteLock>

#include "callee.h"
#include "cbuscallbackintf.h"
#include "callback.h"
#include "zeromqcommunication.h"
#include "subscriberIntf.h"
#include "queue.h"
#include "sendobject.h"
#include "cbusforwardintf.h"

class QMutex;
class OutGoingWorker;

class CBus: public CBusCallBackIntf, public CBusForwardIntf
{
public:

    CBus();
    virtual ~CBus();
    bool addServices(quint8 name,Deserializer *d, MessageHandler *h);
   // bool isRegistered(quint8 name);
    void subscribe(SubscriberIntf *subscriber, QList<quint16> const &messageType);
    bool removeServices(quint8 name);
    bool unsubscribe(SubscriberIntf *subscriber);

    // implements the calback interface
    // these methods are called when a message is arrived from an external service
    void registerExternalService(quint8 service, QString const &moduleName);
    void unregisterExternalService(quint8 service);
    void inCommingNoReplyCall(quint8 service, QDataStream &stream);
    void inCommingAsyncCall(QString const &rmodule, quint8 service, quint16 requestId, QDataStream &stream);
    void inCommingReply(quint16 requestId, QDataStream &stream);
    void inCommingPublish(QDataStream &stream);
    void inCommingGetServices(QString const &moduleName);
    void inCommingServices(QString const &moduleName, QList<quint8> services);


    // these methods are called from the services and put the messages in the queue
    bool callService(quint8 name, MessagePtr const &msg);
    quint16 asynCallService(quint8 name, MessagePtr const &msg, QSharedPointer<CallBack> callback);
    void push(MessagePtr const &msg);
    void sendReply(quint16 requestId, const MessagePtr &message);
    void forwardObject(SendObjectPtr const &obj);


private:


    Q_DISABLE_COPY(CBus);

    struct inMessage {
        bool    internalMsg;
        quint16 requestId;
        QString module;
        // this constructor is used for messages from other processes
        inMessage(quint16 _requestId, QString const &_module):
                internalMsg(false),requestId(_requestId), module(_module) {}
        // this constructor is used for messages from the same process
        inMessage(quint16 _requestId):
                internalMsg(true), requestId(_requestId) {}
    };

    //for external communication we use 0mq
    // if another mehod will be used, this has to be changed!!!
    ZeroMqCommunication externalComm;

    QHash<quint8,Callee> internalServices;

    //this table contains the mapping between a outgoing asyncall and the callback function
    // it is used when a reply comes in.
    QHash<quint16,QSharedPointer<CallBack> > callBackFns;

    //this table contains the mapping between services running in another process and module name
    //when a service register himself  a register message is send to all the connected modules
    //the first element of the list is the service, the second is the module name
    QHash<quint8,QString> externalServices;

    //this table contains the mapping between a incomming asyncall and the module name
    //it is used for sending a reply to the calling module
    QHash<quint16,inMessage> inCommingCalls;

    //this table contains the mapping between a messageType and a subscriber
    QMultiHash<quint16,SubscriberIntf *> messageSubscriberMap;

    quint16 inRequestId;
    quint16 outRequestId;

    QMutex indexLock;
    QReadWriteLock intServLock;
    QReadWriteLock extServLock;
    QMutex inCommingCallsLock;
    QMutex callBackFnsLock;
    QReadWriteLock subscriberLock;

    // queue the outgoing messages
    Queue<SendObject const> sendQueue;

    quint16 generateUniqueOutRequestId();
    quint16 generateUniqueInRequestId();

    // these methods are called when a message is arrived from an internal service
    void inCommingAsyncCall(quint8 service, quint16 requestId, MessagePtr const &msg);
    void inCommingReply(quint16 requestId, MessagePtr const &msg);
    void inCommingPublish(MessagePtr const &msg);
    void inCommingNoReplyCall(quint8 service, MessagePtr const &msg);

    // register the service on 0mq
    void sendRegisterService(quint8 name);
    void sendUnRegisterService(quint8 name);

    void init();
    void stop();

    QList< QSharedPointer<OutGoingWorker> > OutworkerThread;




};

#endif // CBUS_H
