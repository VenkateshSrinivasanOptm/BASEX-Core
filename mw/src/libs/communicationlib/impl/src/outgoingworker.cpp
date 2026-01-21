#include "outgoingworker.h"
#include "zeromqcommunication.h"
#include "cbusforwardintf.h"
#include <QDebug>

OutGoingWorker::OutGoingWorker(Queue<SendObject const> &_sendQueue, ZeroMqCommunication &_externalComm,
                               CBusForwardIntf &_cbus):
    sendQueue(_sendQueue), externalComm(_externalComm), cbus(_cbus)
{
}


void OutGoingWorker::stopRunning()
{
    running= false;
}

void OutGoingWorker::run()
{
    running = true;
    while(running)
    {
        SendObjectPtr obj(sendQueue.removeFromQueue());
        if(!obj.isNull())cbus.forwardObject(obj);
    }
#ifdef DEBUG
    qDebug() << __FUNCTION__ << ":" << __LINE__ << "Thread stops working";
#endif
}

