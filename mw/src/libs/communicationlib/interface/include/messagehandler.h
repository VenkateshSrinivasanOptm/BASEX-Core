#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include<message.h>

class MessageHandler
{
public:
    virtual ~MessageHandler(){}

    /**
      * This method is called when a noreply message is received .
      * Since this method is called in the thread of the communication library
      * this method should return immediately
      **/
    virtual bool messageHandler(MessagePtr message)=0;

    /**
      * This method is called when a reply has to send to the callee or
      * when a reply is arrived.
      * Since this method is called in the thread of the communication library
      * this method should return immediately
      **/
    virtual bool messageHandler(quint16 requestId, MessagePtr message)=0;
};

#endif // MESSAGEHANDLER_H
