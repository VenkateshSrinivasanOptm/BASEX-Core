#ifndef ZEROMQWORKER_H
#define ZEROMQWORKER_H

#include<QThread>
#include "queue.h"
#include "zmq.hpp"

class ZeroMqIncommingMessage;
/**
  * The ZeroMqWorker handles an incoming request. It is used in with the ZeroMqMultipleServer class
  * The number of workers are defined in the config file. If the nbrOfWorkers=1, the ZeroMqSingleServer class
  * is used for the incoming request.
  **/

class ZeroMqWorker:public QThread
{
public:
    ZeroMqWorker(Queue< zmq::message_t > &queue,ZeroMqIncommingMessage &messageHandler);
    void run();

private:
    Queue< zmq::message_t > &queue;
    ZeroMqIncommingMessage &messageHandler;
};

#endif // ZEROMQWORKER_H
