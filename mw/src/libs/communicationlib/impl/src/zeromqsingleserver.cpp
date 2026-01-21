#include "zeromqsingleserver.h"
#include"zeromqincommingmessage.h"
#include<QDebug>

ZeroMqSingleServer::ZeroMqSingleServer(QString const &_address, zmq::context_t &_ctx,
                                       ZeroMqIncommingMessage &_messageHandler):
    messageHandler(_messageHandler), address(_address), ctx(_ctx)
{
}

void ZeroMqSingleServer::run()
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
    // zmq::message_t reply(0);
    //memset (reply.data (), 0, reply.size ());

    zmq::message_t msg;
    while(1) {
        try{
            s.recv(&msg);
        }catch(zmq::error_t t)
        {
            qDebug() << __FUNCTION__ << ":" << __LINE__ << "error receiving message : "
                     << t.what() << "for address " << address;
            continue;
        }
        QByteArray data= QByteArray::fromRawData(reinterpret_cast<char *>(msg.data()),msg.size());
        messageHandler.handleZmqIncommingMessage(data);
    }
}
