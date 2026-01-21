#ifndef DTV_H
#define DTV_H

#include<QObject>

#include <cbusinterface.h>

class Dtv:public QObject
{
    Q_OBJECT

public:
    Dtv();
    MessagePtr deserialize(quint16 messageType, QDataStream &stream);
    bool callback(quint16 requestId, MessagePtr reply);
    void setReminder(quint16 programId, quint16 channelId);

private:
    CBusInterface &cbus;

};
#endif // DTV_H
