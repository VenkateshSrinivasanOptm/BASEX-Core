#ifndef CBUSINTERFACE_H
#define CBUSINTERFACE_H

#include<message.h>
#include <QDebug>

class CBus;
class Deserializer;
class MessageHandler;
class ReplyMessageHandler;
class CallBack;
class SubscriberIntf;

/**
  * this class defines the interface between the applications and the communication library.
  * It must be initiated at startup of the application.
  **/

class CBusInterface
{

public:

    static CBusInterface & instance() {
            static CBusInterface instance;
            return instance;
    }

    /**
      * sends a noreply message to a service
      **/
    bool callService(quint8 serviceName, MessagePtr const &message);

    /**
      * this method sends a message a service and returns. It returns 0 if the service is not available otherwise it
      * returns a requestId.
      * When the reply message is received, the CallBackFn in callBack object is called. Use the CallBackGen template class
      * for defining the CallBackFn function. The CallBackFn  method should return immediately since it runs in the communication
      * library thread.
      * When the reply message needs to be deserialized, the DeserializeFn in callBack object is called
     **/

    quint16 callServiceWithCallback(quint8 serviceName, MessagePtr const &message, QSharedPointer<CallBack> callback);
    /**
      * publish a message
      * the message is sends to all the services that are subscribe on the messageType that is provided
      * in the message
      *
      **/
    void push(MessagePtr const &message);

    /**
      * Registrate a service so that he can received messages from other services. When a message is received,
      * the method messageHandler  is called in the receiver object.
      * The deserializer object is used for deserializing the received message.
      * it returns false in case the service was already registrated
      **/
    bool registerService(quint8 serviceName, MessageHandler *receiver, Deserializer *deserializer);

    /**
      * subscribe the service to receives message
      **/
    void subscribe(SubscriberIntf *subscriber,QList<quint16> const &messageType);
    /**
      * send a reply message
      **/
    void sendReply(quint16 requestId, MessagePtr const &message);


    /**
      * unsubscribe the service
      */
    bool unsubscribe(SubscriberIntf *subscriber);

    /**
      * unregister the services
      **/
    bool unregisterService(quint8 serviceName);

private:

    CBusInterface();
    virtual ~CBusInterface();
    Q_DISABLE_COPY(CBusInterface);
    CBus *cbus;


};

#endif // CBUSINTERFACE_H
