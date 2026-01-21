#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include<QThread>
#include<QList>
#include <QSharedPointer>
#include<zmq.hpp>
#include "queue.h"


class ZeroMqIncommingMessage;
/**
  * this class starts a multiple 0MQ server using the req/resp socket
  * the messages are stored in a queue
  */

class ZeroMqMultipleServer: public QThread
{
public:
    ZeroMqMultipleServer(quint8 nbrOfWorkers, QString const &address,
                         zmq::context_t &ctx, ZeroMqIncommingMessage &messageHandler);
    void run();

private:
    QString address;
    zmq::context_t &ctx;
    Queue<zmq::message_t> queue;
    QList< QSharedPointer<QThread> > workerThread;
    void init(quint8 nbrOfWorkers, ZeroMqIncommingMessage &messageHandler);

};

#endif // ZEROMQSERVER_H
