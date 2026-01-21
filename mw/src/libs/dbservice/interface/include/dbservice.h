#ifndef DBSERVICE_H
#define DBSERVICE_H
#include "callback.h"
#include "deserializer.h"
#include "messagehandler.h"
#include "subscriberIntf.h"
#include <QObject>

#define TESTING
#define MWNAMESTR "dbservice"
#define MSVERSTR "1.0"

class DbCBusComm;

class Dbservice : QObject {
  Q_OBJECT
public:
  static Dbservice &getInstance();

  /* initialisation method for startup sequence */
  void initialise(void);
public slots:

private:
  Dbservice();
  virtual ~Dbservice();
  Q_DISABLE_COPY(Dbservice);
  QSharedPointer<DbCBusComm> m_CBusComm;
};

#endif // DBSERVICE_H
