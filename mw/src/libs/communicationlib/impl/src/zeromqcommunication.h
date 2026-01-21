#ifndef ZEROMQCOMMUNICATION_H
#define ZEROMQCOMMUNICATION_H

#include<zmq.hpp>
#include<message.h>
#include<QHash>
#include "zeromqincommingmessage.h"
#include "zeromqsenderthread.h"
#include "zeromqsubscriberserver.h"


class CBusCallBackIntf;
class CBusConfigPrivate;

/**
  * This class starts the 0MQ stuff and is de glue between cbus and 0mq
  **/

class ZeroMqCommunication
{
public:

    ZeroMqCommunication(CBusCallBackIntf &cbus);
    virtual ~ZeroMqCommunication();
    void asynCall(QString const &rmodule,quint8 service,quint16 requestId, MessagePtr const &msg);
    void call(QString const &module, quint8 service, MessagePtr const &msg);
    void sendReply(QString const &module, quint16 requestId, MessagePtr const &msg);
    void sendRegisterService(quint8 service);
    void publish(MessagePtr const &msg);
    void subscribe(quint16 messagetype);
    void getServices();
    void sendServices(QString const &module, MessagePtr const &msg);
    void sendUnRegisterService(quint8 service);
    void init();

private:

    zmq::context_t ctx;
    QThread *server;
    ZeroMqIncommingMessage messageHandler;
    CBusConfigPrivate &config;
    QString moduleName;
    QHash<QString,ZeroMqBasicClient *> clients;
    ZeroMqSenderThread senderThread;
    ZeroMqSubscriberServer subscriberServer;

};

#endif // ZEROMQCOMMUNICATION_H
