#include "zeromqsubscriberserver.h"
#include "cbusconfigprivate.h"
#include "zeromqincommingmessage.h"

#include<QDebug>
#include <iostream>

ZeroMqSubscriberServer::ZeroMqSubscriberServer(zmq::context_t &_ctx, ZeroMqIncommingMessage &_messageHandler):
    messageHandler(_messageHandler), ctx(_ctx), socket(NULL)
{
}

void ZeroMqSubscriberServer::run()
{
#ifdef DEBUG
    qDebug() << "in 0mq subscriber server";
#endif
    QList<CBusConfigPrivate::StringTriple> remoteModules=CBusConfigPrivate::instance().getRemoteModules();
    quint8 size=remoteModules.size();
    bool sub=false;

    socket= new zmq::socket_t(ctx, ZMQ_SUB);



    for(quint8 i=0; i<size;++i)
    {
        CBusConfigPrivate::StringTriple m=remoteModules.at(i);
        if(!(m.z.isNull() || m.z.isEmpty()))
        {
#ifdef DEBUG
            qDebug() << "create a 0mq subscribe client for process " << m.x<< " on address " << m.z;
#endif
            sub=true;
            socket->connect(m.z.toLatin1().data());
        }
    }

    if(!sub)
    {
        qDebug() << " we don't have 0mq subscribers";
        delete socket;
        socket=NULL;
        return;
    }

    // subscribe for all published messages
    // TODO find out how we can filter this
    socket->setsockopt(ZMQ_SUBSCRIBE,"",0);

    zmq::message_t msg;

    while(1)
    {
        try {
            socket->recv(&msg);
        }
        catch(zmq::error_t t)
        {
            qDebug() << __FUNCTION__ << ":" << __LINE__ << "error receiving message : "
                     << t.what();
            continue;
        }
        QByteArray data= QByteArray::fromRawData(reinterpret_cast<char *>(msg.data()),msg.size());
        messageHandler.handleZmqIncommingPublishedMessage(data);
    }
}

// This method is not used at the moment
void ZeroMqSubscriberServer::subscribe(QByteArray const &data)
{
    if(socket)
    {
        socket->setsockopt(ZMQ_SUBSCRIBE,data.data(),data.size());
    }
    else
    {
        qDebug() << "no 0mq subscriber running ";
    }
}
