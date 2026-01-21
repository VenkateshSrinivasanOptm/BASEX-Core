#ifndef DBSERVICEDELEGATE_H
#define DBSERVICEDELEGATE_H
#include <QObject>

class DbServiceClient;

class DbserviceDelegate : public QObject {

public:
  /**
   * This method will return a reference to a dbservice delegate Object.
   */
  static DbserviceDelegate &getInstance();

  /** Download service request to write config data to file
   */
public slots:

  void writeConfigData(const QString &data);
signals:

  /** Signal which send back the reponse if config data is written */
  void writeConfigDataResponse(bool written);

private:
  DbserviceDelegate();
  virtual ~DbserviceDelegate();
  Q_DISABLE_COPY(DbserviceDelegate);
  const QScopedPointer<DbServiceClient> dbServiceClient;
};

#endif // DBSERVICEDELEGATE_H
