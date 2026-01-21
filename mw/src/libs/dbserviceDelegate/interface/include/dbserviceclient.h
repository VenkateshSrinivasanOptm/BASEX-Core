#ifndef DBSERVICECLIENT_H
#define DBSERVICECLIENT_H

#include <QObject>
#include <cbusinterface.h>
#include <subscriberIntf.h>

class DbServiceClient : public QObject, public SubscriberIntf {
  Q_OBJECT
public:
  DbServiceClient();
  virtual ~DbServiceClient(){};
  MessagePtr deserialize(quint16 messageType, QDataStream &data);
  bool callback(quint16 requestId, MessagePtr reply);
  MessagePtr subscriberDeserialize(quint16 messageType,
                                   QDataStream &stream) override;
  bool subscriberMessageHandler(MessagePtr message) override;

  void writeConfigData(const QString &data);

private:
  CBusInterface &cbus;
};

#endif // DBSERVICECLIENT_H
