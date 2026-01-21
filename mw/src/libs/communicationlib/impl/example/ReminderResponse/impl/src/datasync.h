#ifndef DATASYNC_H
#define DATASYNC_H

#include<QObject>

#include <cbusinterface.h>
#include <deserializer.h>
#include <messagehandler.h>

class DataSync: public QObject, public MessageHandler,
    public Deserializer
{
    Q_OBJECT
public:
    DataSync();
    virtual MessagePtr deserialize(quint16 messageType, QDataStream &stream);

    /**
      * this method is not used in our example
      */
    virtual bool messageHandler(MessagePtr message) { return false; }
    virtual bool messageHandler(quint16 requestId, MessagePtr message);

signals:
    void setReminder(quint16 requestId,quint16 channelId,quint16 programId);

private slots:
    void reminder(quint16 requestId,quint16 channelId,quint16 programId);

private:
     CBusInterface &cbus;
};

#endif // DATASYNC_H
