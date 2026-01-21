#ifndef APPMANIF_H
#define APPMANIF_H

#include <QDebug>
#include <QTimer>
#include "appmanmsgif.h"

#define TEST_STARTUP_TIMEOUT_MS 500
#define TEST_HEARTBEAT_PERIOD_MS 10000

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

    // The following functions are only provided for testing purposes
    void testStateChange(StateChange state);
    void testUiReqRestart(RestartLevel restartLevel);

signals:
    void establishCommunication(void);
    void notifyStateChange(StateChange state);
    void notifyException(ErrorLevel errLevel);
    void requestSwRestart(RestartLevel restartLevel);
    void heartbeatNotification(void);

private slots:
    void testStartupTimedout(void);
    void beatTheHeart();

private:
    typedef enum
    {
        TEST_STARTUP_AWAIT_PUBLISH,
        TEST_STARTUP_AWAIT_SUBSCRIBED,
        TEST_STARTUP_SUBSCRIBED
    } TestStartupPhase;

    AppManIf();
    virtual ~AppManIf();
    TestStartupPhase testStartupPhase;
    bool initialised;
    QString appName;
    QString appVersion;
    RestartLevel reqRestartLevel;
    QTimer heartbeatTimer;
    bool heartbeatOutstanding;
};

#endif // APPMANIF_H
