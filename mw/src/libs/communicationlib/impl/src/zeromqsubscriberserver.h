#ifndef ZEROMQSUBSCRIBERSERVER_H
#define ZEROMQSUBSCRIBERSERVER_H

#include<QThread>
#include<zmq.hpp>
#include<QStringList>

class ZeroMqIncommingMessage;
/**
  * this class listen to messages that a published
  **/

class ZeroMqSubscriberServer:public QThread
{
public:
    ZeroMqSubscriberServer(zmq::context_t &_ctx, ZeroMqIncommingMessage &_messageHandler);
    void run();
    void subscribe(QByteArray const &data);

private:
    ZeroMqIncommingMessage &messageHandler;
    zmq::context_t &ctx;
    zmq::socket_t *socket;


};

#endif // ZEROMQSUBSCRIBERSERVER_H
