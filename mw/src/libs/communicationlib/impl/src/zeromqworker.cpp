#include "zeromqworker.h"
#include "zeromqincommingmessage.h"


ZeroMqWorker::ZeroMqWorker(Queue<zmq::message_t> &_queue,ZeroMqIncommingMessage &_messageHandler):
        queue(_queue), messageHandler(_messageHandler)
{
}


void ZeroMqWorker::run()
{
    while(1)
    {
        QSharedPointer<zmq::message_t> msg(queue.removeFromQueue());
        if(!msg.isNull())
        {
            QByteArray data= QByteArray::fromRawData(reinterpret_cast<char *>(msg->data()), msg->size());
            messageHandler.handleZmqIncommingMessage(data);
            msg.clear();
        }
    }
}
