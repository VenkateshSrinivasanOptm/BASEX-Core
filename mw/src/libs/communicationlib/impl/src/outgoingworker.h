#ifndef OUTGOINGWORKER_H
#define OUTGOINGWORKER_H

#include<QThread>
#include"queue.h"
#include "sendobject.h"

class ZeroMqCommunication;
class CBusForwardIntf;

class OutGoingWorker: public QThread
{
public:
    OutGoingWorker(Queue<SendObject const> &sendQueue, ZeroMqCommunication &externalComm, CBusForwardIntf &cbus);
    void run();
    void stopRunning();

private:
    Queue<SendObject const> &sendQueue;
    ZeroMqCommunication &externalComm;
    CBusForwardIntf &cbus;
    volatile bool running;
};

#endif // OUTGOINGWORKER_H
