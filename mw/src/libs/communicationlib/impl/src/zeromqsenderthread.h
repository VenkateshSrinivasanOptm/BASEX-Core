#ifndef ZEROMQSENDERTHREAD_H
#define ZEROMQSENDERTHREAD_H

#include "zeromqbasicclient.h"
#include <zmq.hpp>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QSemaphore>
#include <QHash>
#include <QSharedPointer>


struct ZeroMqObject {
    QByteArray data;
    ZeroMqBasicClient *client;
    ZeroMqObject(QByteArray const &_data, ZeroMqBasicClient *_client) :
            data(_data), client(_client) {}
    virtual ~ZeroMqObject(){}
};

typedef QSharedPointer<ZeroMqObject> ZeroMqObjectPtr;

class ZeroMqSenderThread:public QThread
{

public:
    ZeroMqSenderThread(zmq::context_t &_ctx,QHash<QString,ZeroMqBasicClient *> &_clients);
    void run();
    void addToQueue(ZeroMqObjectPtr const &obj);

private:
    zmq::context_t &ctx;
    void removeFromQueue();
    QQueue<ZeroMqObjectPtr> queue;
    QMutex queueMutex;
    QSemaphore queueSem;
    QHash<QString,ZeroMqBasicClient *> &clients;


};

#endif // ZEROMQSENDERTHREAD_H
