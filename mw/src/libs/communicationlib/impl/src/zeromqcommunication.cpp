#include "zeromqcommunication.h"
#include "zeromqmultipleserver.h"
#include "zeromqsingleserver.h"
#include "zeromqclient.h"
#include "cbusconfigprivate.h"
#include "cbuscallbackintf.h"
#include "definitions.h"

#include<QDebug>
#include<QIODevice>
#include<performance.h>
#include <iostream>

ZeroMqCommunication::ZeroMqCommunication(CBusCallBackIntf &_cbus):
    ctx(1), server(NULL), messageHandler(_cbus),
    config(CBusConfigPrivate::instance()),
    clients(QHash<QString,ZeroMqBasicClient *>()),
    senderThread(ctx,clients), subscriberServer(ctx,messageHandler)
{
}

ZeroMqCommunication::~ZeroMqCommunication()
{
    if(server) delete server;
    // todo stop senderThread
    //   QHash<QString,ZeroMqClient *>::iterator end=
    //    for()

}

void ZeroMqCommunication::init()
{
#ifdef DEBUG
    qDebug() << "initialize 0mq " ;
#endif
    // start the clients for sending messages to other modules
    senderThread.start();

    // start the subscriber server for receiving subscribed messages
    subscriberServer.start();

    CBusConfigPrivate::StringTriple  const &module(config.getModule());

    qDebug() << module.x << " " << module.y << " " <<module.z;
    moduleName=module.x;

    if(!(module.y.isNull() || module.y.isEmpty()))
    {
        quint8 nbrOf0mqWorkers=config.getNbrOfInWorkers();
        if(nbrOf0mqWorkers==1) {
            // create a single server
#ifdef DEBUG
            qDebug() << " create a single 0mq server with address " << module.y;
#endif
            server = new ZeroMqSingleServer(module.y,ctx,messageHandler);
            server->start();
        }
        else
        {
            //create a multiple server
#ifdef DEBUG
            qDebug() << " start a multiple server with " << nbrOf0mqWorkers << "threads.";
#endif
            server = new ZeroMqMultipleServer(nbrOf0mqWorkers,module.y,ctx,messageHandler);
            server->start();
        }
    }


}


void ZeroMqCommunication::asynCall(QString const &rmodule,quint8 service, quint16 requestId,
                                   MessagePtr const &msg)
{
    //TODO move these stuff to clients and let them do the serialization in a thread
#ifdef DEBUG
    qDebug() << "asyn call " << rmodule << " " << service << " " << "id " << requestId;
#endif
    QString moduleName(config.getModule().x);
    QHash<QString,ZeroMqBasicClient *>::const_iterator itr=clients.find(rmodule);
    if(itr!=clients.constEnd()) {
        TRACE("CBUS: start serialization async call for service " << (quint16)service);
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        quint8 i=EXTERNALASYNCALL;
        stream << i << service << moduleName << requestId << msg->messageType;
        msg->serialize(stream);
        TRACE("CBUS: end serialization async call for service " << (quint16)service);
        senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
    }
}

void ZeroMqCommunication::call(QString const &module, quint8 service, MessagePtr const &msg)
{
    QHash<QString,ZeroMqBasicClient *>::const_iterator itr=clients.find(module);
    if(itr!=clients.constEnd()) {
        TRACE("CBUS: start serialization call for service " << (quint16)service);
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        quint8 i= NOREPLYCALL;
        stream << i <<  service << msg->messageType;
        msg->serialize(stream);
        TRACE("CBUS: end serialization call for service " << (quint16)service);
        senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
    }
}

void ZeroMqCommunication::sendReply(QString const &module, quint16 requestId, MessagePtr const &msg)
{
    QHash<QString,ZeroMqBasicClient *>::const_iterator itr=clients.find(module);
    if(itr!=clients.constEnd()) {
        TRACE("CBUS: start serialization reply for module " << module.toStdString());
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        quint8 i= EXTERNALREPLY;
        stream << i << requestId << msg->messageType;
        msg->serialize(stream);
        TRACE(" CBUS: end serialization reply for module " << module.toStdString());
        senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
    }
}

void ZeroMqCommunication::sendRegisterService(quint8 service)
{
    QString moduleName(config.getModule().x);
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    quint8 i= REGISTER;
    stream << i << service << moduleName;
    QHash<QString, ZeroMqBasicClient *>::const_iterator itr=clients.constBegin();
    QHash<QString, ZeroMqBasicClient *>::const_iterator endItr=clients.constEnd();
    while(itr != endItr)
    {
        if(itr.key()!= moduleName)
        {
#ifdef DEBUG
            qDebug() << " register service " << service << " to process " << itr.key();
#endif
            senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
        }
        ++itr;
    }
}

void ZeroMqCommunication::sendUnRegisterService(quint8 service)
{
    QString moduleName(config.getModule().x);
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    quint8 i= UNREGISTER;
    stream << i << service;
    QHash<QString, ZeroMqBasicClient *>::const_iterator itr=clients.constBegin();
    QHash<QString, ZeroMqBasicClient *>::const_iterator endItr=clients.constEnd();
    while(itr != endItr)
    {
        if(itr.key()!= moduleName)
        {
#ifdef DEBUG
            qDebug() << " unregister service " << service << " to process " << itr.key();
#endif
            senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
        }
        ++itr;
    }
}

void ZeroMqCommunication::getServices()
{
#ifdef DEBUG
    qDebug() << " get services request ";
#endif
    QString moduleName(config.getModule().x);
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    quint8 i=GETSERVICES;
    stream << i << moduleName;
    QHash<QString, ZeroMqBasicClient *>::const_iterator itr=clients.constBegin();
    QHash<QString, ZeroMqBasicClient *>::const_iterator endItr=clients.constEnd();
    while(itr != endItr)
    {
        if(itr.key()!= moduleName)
        {
            senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
        }
        ++itr;
    }
}

void ZeroMqCommunication::sendServices(QString const &module, MessagePtr const &msg)
{
    QHash<QString,ZeroMqBasicClient *>::const_iterator itr=clients.find(module);
    if(itr!=clients.constEnd())
    {
#ifdef DEBUG
        qDebug() << " send all registered services";
#endif
        QString moduleName(config.getModule().x);
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        quint8 i=REGISTERSERVICES;
        stream << i << moduleName;
        msg->serialize(stream);
        senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
    }
}

void ZeroMqCommunication::publish(MessagePtr const &msg)
{
    // The name of the publisher is the name of the process
    QHash<QString,ZeroMqBasicClient *>::const_iterator itr=clients.find(moduleName);
    if(itr!=clients.constEnd())
    {
        TRACE("CBUS: start serialization publish message of type " << msg->messageType);
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        stream << msg->messageType;
        msg->serialize(stream);
        TRACE("CBUS: end serialization publish message of type " << msg->messageType);
        senderThread.addToQueue(ZeroMqObjectPtr(new ZeroMqObject(data,itr.value())));
    }
}

void ZeroMqCommunication::subscribe(quint16 messageType)
{
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    stream << messageType;
    subscriberServer.subscribe(data);
}

