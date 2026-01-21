#ifndef ZEROMQINCOMINGMESSAGE_H
#define ZEROMQINCOMINGMESSAGE_H

#include<QByteArray>
#include"cbus.h"

static void  handleZmqIncomingMessage(QByteArray const &data, CBus const &cbus){
    //TODO how can I see that is a replymessage
    QString serviceName; // TODO get serviceName
    QString type;
    MessagePtr message=cbus.deserialize(serviceName,type,data);
    // TODO put in queue ?
    // release message ?
}

#endif // ZEROMQINCOMINGMESSAGE_H
