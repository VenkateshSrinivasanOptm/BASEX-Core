#ifndef CALLBACK_H
#define CALLBACK_H

#include "message.h"


class CallBack
{
public:
    virtual ~CallBack(){}
    virtual bool execute(quint16 requestId, MessagePtr message)=0;
    virtual MessagePtr deserialize(quint16 type, QDataStream &stream)=0;
};



#endif // CALLBACK_H
