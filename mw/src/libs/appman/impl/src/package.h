#ifndef PACKAGE_H
#define PACKAGE_H

#include <QProcess>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include "mwconfig.h"
#include "appmanif.h"

typedef enum { PACKAGE_CREATED,
           PACKAGE_STARTED,
           PACKAGE_PUBLISHED,
           PACKAGE_SUBSCRIBED,
           PACKAGE_DYING,
           PACKAGE_DEAD
       } PACKAGE_STATE;

class Package : public QObject
{

    Q_OBJECT

public:
    Package();
    virtual ~Package();
    void start(QByteArray processId, QString processPath);
    void restart();
    void sendSubscribeMessage(char powerState);
    void notifyStateChange(StateChange state);
    bool sendHeartbeat(void);
    QByteArray getPackageId(void);
    PACKAGE_STATE getProcessState(void);
    QString getPackagePath(void);
#ifdef TEST_BUILD
    void testNotifyInstructionMsgReceived(QByteArray msg);
#endif

signals:
    void published(void);
    void subscribed(void);
    void shutdown(ErrorLevel level,QByteArray);
    void swRestartResponse(bool restart);
    void uiPowerChangeNotification(UiPowerNotification state);

private slots:
    void startupTimedout(void);
    void started(void);
    void error(QProcess::ProcessError err);
    void handleMessageNotification(void);
    void stderrNotification(void);
    void finished ( int exitCode, QProcess::ExitStatus exitStatus );
//TODO    void childPostMortem();
//TODO        void postMortemFinished ( int exitCode, QProcess::ExitStatus exitStatus );
    void postMortemNotification(void);

private:
    void childDied(ErrorLevel errLevel);

    MwConfig *mwConfig;
    QProcess *process;
    QProcess *postMotemProcess;
    QByteArray postMortemDebug;
    QByteArray packageId; //Id taken from the config file of Appname
    QByteArray packageNameVer; // Name & Version received from the started process
    QString packagePath;
    qint64 processId;
    QStringList arguments;
    PACKAGE_STATE processState;
    bool outstandingHeartbeatResponse;
};

#endif // PACKAGE_H
