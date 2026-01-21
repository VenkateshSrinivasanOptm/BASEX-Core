#include "dbserviceDelegate.h"
#include "dbserviceclient.h"
#include <QDebug>

DbserviceDelegate::DbserviceDelegate() : dbServiceClient(new DbServiceClient) {}

DbserviceDelegate::~DbserviceDelegate() {}

DbserviceDelegate &DbserviceDelegate::getInstance() {
  static DbserviceDelegate instance;
  return instance;
}

void DbserviceDelegate::writeConfigData(const QString &data) {
  dbServiceClient->writeConfigData(data);
}
