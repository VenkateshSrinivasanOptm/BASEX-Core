#ifndef MWCONFIG_H
#define MWCONFIG_H
#include <QString>
#include <QList>
#include <QByteArray>
#include "brokerdocument.h"
#include "mwserviceinfo.h"
#include <QSemaphore>

class HALLogger;

class MwConfig
{
public:
    static void instantiate(void);
    static MwConfig * getInstance(void);
    void acquire(void);
    int getPackageCount(void);
    int getStabilityTimeout(void);
    int getAMStartTimeout(void);
    int getPackageTerminateTimeout(void);
    int getUIrestartRequestTimeout();
    int getHeartbeatPeriod();
    QByteArray getPackagePath(int idx);
    QByteArray getPackageId(int idx);
    QByteArray getGdbScriptFile();
    QList<ServicesList> mwServiceList;
    QList<QByteArray> mwServNameList;
    qint16 minEPGhrs;
    QSemaphore gdbDumpInProgress;
private:
    MwConfig();
    QString createConfigFileFromResource(const QString &resourcePath, const QString &fileName);
    int amStartupTimeout;
    int uiRestartRequestTimeout;
    int packageTerminateTimeout;
    QList<QByteArray> packageId;
    QList<QByteArray> packagePath;
    QByteArray gdbScriptFile;
    int packageCount;
    static MwConfig *pInstance;
    int heartbeatPeriod;
    int stabilityTimeout;
};

#endif // MWCONFIG_H
