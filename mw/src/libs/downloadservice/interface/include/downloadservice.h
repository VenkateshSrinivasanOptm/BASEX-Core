#ifndef DBSERVICE_H
#define DBSERVICE_H
#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>
#include <initializationmessage.h>
#include "tcpclient.h"

#define MWNAMESTR "downloadservice"
#define MSVERSTR "1.0"

class DownloadCBusComm;

class DownloadService : QObject {
  Q_OBJECT
public:
  static DownloadService &getInstance();

  /* initialisation method for startup sequence */
  void initialise(void);
  TcpClient* client();

public slots:
  void processStart(QSharedPointer<InitializationMessage const>);

private:
  DownloadService();
  virtual ~DownloadService();
  Q_DISABLE_COPY(DownloadService)
  QSharedPointer<DownloadCBusComm> m_CBusComm;
  TcpClient mClient;
};

#endif // DBSERVICE_H
