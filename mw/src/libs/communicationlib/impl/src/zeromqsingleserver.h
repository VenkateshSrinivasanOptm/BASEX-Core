#ifndef ZEROMQSINGLESERVER_H
#define ZEROMQSINGLESERVER_H

#include<QThread>
#include <zmq.hpp>

class ZeroMqIncommingMessage;
/**
  * this class handles the incomming request. It used the 0MQ REQ/REP socket. This class is
  * used if the NbrOfWorkers=1 in the config file. If the nbrOfWorker>1 thn the ZeroMqMultipleServer class
  * is used.
  **/

class ZeroMqSingleServer: public QThread
{
public:
    ZeroMqSingleServer(QString const &_address, zmq::context_t &_ctx, ZeroMqIncommingMessage &_messageHandler);

    void run();

private:
    ZeroMqIncommingMessage &messageHandler;
    QString address;
    zmq::context_t &ctx;
};

#endif // ZEROMQSINGLESERVER_H
