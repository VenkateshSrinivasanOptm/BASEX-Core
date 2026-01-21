#ifndef APPMANIF_H
#define APPMANIF_H

#include <QObject>
#include "msgmanager.h"
#include "appmanmsgif.h"

class EventWatcher;
class MsgManager;
class HALLogger;

class AppManIf  : public QObject
{
    Q_OBJECT

public:
    static AppManIf* getInstance();
    void notifyPublished(const char * appName,const char * appVersion);
    void communicationEstablished(void);
    void initialise(void);
    void swRestartResponse(bool restart);
    void uiPowerChangeNotification(UiPowerNotification state);
    void sendHeartbeatResponse(void);
    static StateChange getCurrentPowerState();

signals:
    void establishCommunication(void);
    void notifyStateChange(StateChange state);
    void notifyException(ErrorLevel);
    void heartbeatNotification(void);

private slots:
    void handleMessage(stdioMessage msg);
    void handleMsgException(ErrorLevel errLev);

private:
#ifdef TEST_BUILD
    bool testNotificationMessage(QByteArray msg);
    bool stopHeartbeat;
    bool hang_on_terminate;
    bool hang_on_subscribed;
    bool permit_restart;
#endif
    AppManIf();
    virtual ~AppManIf();
    QString appName;
    QString appVersion;
    EventWatcher *event;
    HALLogger*  m_pLogger;
};

#endif // APPMANIF_H
