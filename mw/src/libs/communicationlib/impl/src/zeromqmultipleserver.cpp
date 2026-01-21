#include "zeromqmultipleserver.h"
#include "zeromqincommingmessage.h"
#include "zeromqworker.h"

#include <QDebug>

ZeroMqMultipleServer::ZeroMqMultipleServer(quint8 _nbrOfWorkers, QString  const &_address,
                                           zmq::context_t &_ctx, ZeroMqIncommingMessage &_messageHandler):
    address(_address), ctx(_ctx)
{
    init(_nbrOfWorkers, _messageHandler);
}

void ZeroMqMultipleServer::run()
{

    zmq::socket_t s(ctx, ZMQ_PULL);
#ifdef DEBUG
    qDebug() << "0mq listen on address " << address;
#endif

    try {
        s.bind(address.toLatin1().data());
    }
    catch(zmq::error_t t)
    {
        qDebug() << __FUNCTION__ << ":" << __LINE__ << "error bind 0mq address "
                 <<  address << " "
                 << QString(t.what())
                 << " thread stops running ";
        return;
    }

    // define a empty reply message
    //  zmq::message_t reply(0);

    // zmq::message_t msg;


    while(1) {
#ifdef DEBUG
        qDebug() << " listen on incoming messages ";
#endif
        zmq::message_t *msg=new zmq::message_t();
        try{
            s.recv(msg);
        }catch(zmq::error_t t)
        {
            qDebug() << __FUNCTION__ << ":" << __LINE__ << "error receiving message : "
                     << t.what() << "for address " << address;
            continue;
        }
        queue.addToQueue(QSharedPointer<zmq::message_t>(msg));
    }

}

void ZeroMqMultipleServer::init(quint8 nbr,ZeroMqIncommingMessage &messageHandler)
{
    for(quint8 i=0; i<nbr; ++i)
    {
        QThread *thread=new ZeroMqWorker(queue, messageHandler);
        thread->start();
        workerThread.append(QSharedPointer<QThread>(thread));
    }
}
