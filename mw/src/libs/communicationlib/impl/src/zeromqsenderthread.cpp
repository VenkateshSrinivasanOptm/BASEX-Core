#include "zeromqsenderthread.h"
#include "cbusconfigprivate.h"
#include "zeromqclient.h"
#include "zeromqpublishserver.h"

#include <QDebug>

ZeroMqSenderThread::ZeroMqSenderThread(zmq::context_t &_ctx, QHash<QString,ZeroMqBasicClient *> &_clients):
    ctx(_ctx), clients(_clients)
{
}


void ZeroMqSenderThread::run()
{
    // startup the connection to other modules

    QList<CBusConfigPrivate::StringTriple> remoteModules=CBusConfigPrivate::instance().getRemoteModules();
    quint8 size=remoteModules.size();
    for(quint8 i=0; i<size;++i)
    {
        CBusConfigPrivate::StringTriple m=remoteModules.at(i);
        if(!(m.y.isNull() || m.y.isEmpty())) {
            qDebug() << "create a 0mq client for process " << m.x << "on address " << m.y;
            clients.insert(m.x, new ZeroMqClient(ctx,m.y));
        }
    }

    // startup the publisher server
    CBusConfigPrivate::StringTriple module=CBusConfigPrivate::instance().getModule();
    if(!(module.z.isNull() || module.z.isEmpty()))
    {
        qDebug() << "create a 0Mq publisher for own process " << module.x << " on address" << module.z;
        clients.insert(module.x,new ZeroMqPublishServer(module.z,ctx));
    }

    if(clients.size()==0) {
        qDebug() << " we have no clients running ";
        return;
    }

    while(1)
    {
#ifdef DEBUG
        qDebug() << " REMOVE FROM QUEUE ";
#endif
        removeFromQueue();
#ifdef DEBUG
        qDebug() << " END REMOVE FROM QUEUE";
#endif
    }
}

void ZeroMqSenderThread::addToQueue(ZeroMqObjectPtr const &obj)
{
    queueMutex.lock();
#ifdef DEBUG
    qDebug() << " add to sender Queue";
#endif
    queue.enqueue(obj);
    queueSem.release();
    queueMutex.unlock();
#ifdef DEBUG
    qDebug() << "end " << __FUNCTION__;
#endif
}

void ZeroMqSenderThread::removeFromQueue()
{
    queueSem.acquire();
    queueMutex.lock();
#ifdef DEBUG
    qDebug() << " remove  from sender Queue";
#endif
    ZeroMqObjectPtr obj(queue.dequeue());
    queueMutex.unlock();
#ifdef DEBUG
    qDebug() << " send message to external process ";
#endif
    obj->client->send(obj->data);
#ifdef DEBUG
    qDebug() << "end " << __FUNCTION__;
#endif
}
