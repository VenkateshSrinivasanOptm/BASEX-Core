#include <QMetaType>
#include <QTimer>
#include "appmanif.h"


AppManIf::AppManIf()
{
    initialised=false;
    testStartupPhase=TEST_STARTUP_AWAIT_PUBLISH;
    QTimer::singleShot(TEST_STARTUP_TIMEOUT_MS, this, SLOT(testStartupTimedout()));
    reqRestartLevel=RESTART_UNDEFINED;
    heartbeatOutstanding=false;
    connect(&heartbeatTimer, SIGNAL(timeout()), this, SLOT(beatTheHeart()));
}

AppManIf::~AppManIf()
{
}

AppManIf *AppManIf::getInstance() {
    static AppManIf instance;
    return &instance;
}

/****************************************************************************
 * sendHeartbeatResponse
 *
 * Provides you an interface from your test software so that you can respond
 * to a heartbeat request.
 ****************************************************************************/
void AppManIf::sendHeartbeatResponse()
{
    heartbeatOutstanding=false;
    qDebug() << "Pong, Heartbeat song\n";
}

/****************************************************************************
 * testStateChange
 *
 * Provides you an interface from your test software so that you can generate
 * state notifications to your applications code.
 ****************************************************************************/
void AppManIf::testStateChange(StateChange state)
{
    emit notifyStateChange(state);
}

/****************************************************************************
 * testUiReqRestart
 *
 * Provides you an interface from your test software so that you can generate
 * a request to restart notification. This notification is only used by the UI.
 ****************************************************************************/
void AppManIf::testUiReqRestart(RestartLevel restartLevel)
{
    emit requestSwRestart(restartLevel);
    reqRestartLevel=restartLevel;
}

/****************************************************************************
 * swRestartResponse
 *
 * This message is sent by the UI to notify the UI whether it is ok to restart.
 * This test version of the method checks that the response was after a valid
 * request. If so then it notifies back the restart request that would be
 * recieved by the real MW when restarting.
 ****************************************************************************/
void AppManIf::swRestartResponse(bool response)
{
    if (reqRestartLevel==RESTART_UNDEFINED)
    {
        qDebug() << "ERROR: No restart request was sent prior to this response\n";
        emit notifyException(STB_LEVEL_ERR);
    }
    if (response)
    {
        switch (reqRestartLevel)
        {
        case RESTART_UI_ONLY:
        case RESTART_MW_AND_UI:
        case RESTART_STB:
            emit notifyStateChange(RESTART);
            break;
        case RESTART_MW_ONLY:
            emit notifyStateChange(RESTART_OTHER);
            break;
        default:
            break;
        }
    }
    reqRestartLevel=RESTART_UNDEFINED;
}



/****************************************************************************
 * notifyPublished
 *
 * This should be called by your applications code in order to notify that it
 * has completed publication of its interfaces. If your application is using
 * an asynchronious notification of the request to establish communication then
 * this notification will be sent provided that you already called the setup.
 ****************************************************************************/
void AppManIf::notifyPublished(const char *lappName, const char *lappVersion)
{
    //qDebug() << "notifyPublished\n";

    //Published Event Notification
    appName=lappName;
    appVersion=lappVersion;
    if (initialised)
    {
        if (testStartupPhase==TEST_STARTUP_AWAIT_PUBLISH)
        {
            testStartupPhase=TEST_STARTUP_AWAIT_SUBSCRIBED;
        }
        else
        {
            qDebug() << "Startup Error - Published notification received at phase " << testStartupPhase << "\n";
            emit notifyException(STB_LEVEL_ERR);
        }

        emit establishCommunication();
    }
    else
    {
        qDebug() << "Startup Error - You forgot to call initialise\n";
        emit notifyException(STB_LEVEL_ERR);
    }
}

/****************************************************************************
 * initialise
 *
 * For those of you that wish to receive async notification of the request to
 * establish communication then you need to configure this by calling this
 * setup method anytime prior to callig notifyPublished.
 ****************************************************************************/
void AppManIf::initialise()
{
    //qDebug() << "initialised\n";
    initialised=true;
}

/****************************************************************************
 * communicationEstablished
 *
 * You need to call this to notify that you have completed the subscription
 * phase of startup. If you do not call it within a timed period then this test
 * version of the API will force your code to terminate so that you can find
 * and fix your mistake.
 ****************************************************************************/
void AppManIf::communicationEstablished()
{
    //qDebug() << "communicationEstablished\n";
    if (testStartupPhase==TEST_STARTUP_AWAIT_SUBSCRIBED)
    {
        testStartupPhase=TEST_STARTUP_SUBSCRIBED;

        // Start heartbeat tests
        heartbeatTimer.start(TEST_HEARTBEAT_PERIOD_MS);
        return;
    }
    else
    {
        qDebug() << "Startup Error - Subscribed notification received at phase " << testStartupPhase << "\n";
        emit notifyException(STB_LEVEL_ERR);
    }
}

/****************************************************************************
 * uiPowerChangeNotification
 *
 * Sent by the UI to notify the MW of a power change. This test version
 * responds back with a notification that the MW is now notifying everyone of
 * this new power level.
 ****************************************************************************/
void AppManIf::uiPowerChangeNotification(UiPowerNotification state)
{
    StateChange stateChange;

    // Notify back the change to mimic that the AppMan changed the power. Note:
    // UI cannot notify power On as this is only achieved by HAL activation
    switch (state)
    {
    case  UI_NOTIFY_POWER_ON:
        stateChange=POWER_ON;
        break;
    case  UI_NOTIFY_POWER_OFF:
        stateChange=POWER_OFF;
        break;
    case UI_NOTIFY_POWER_DEEP_SLEEP:
        stateChange=POWER_DEEP_SLEEP;
        break;
    case UI_NOTIFY_POWER_STANDBY:
        stateChange=POWER_STANDBY;
        break;
    }

    emit notifyStateChange(stateChange);
}

/****************************************************************************
 * testStartupTimedout
 *
 * For testing, this function will terminate your application in the event that
 * you do not complete a correct startup sequence within a timeout period as
 * specified via TEST_STARTUP_TIMEOUT_MS.
 ****************************************************************************/
void AppManIf::testStartupTimedout()
{
    //qDebug() << "testStartupTimerSignal\n";
    if (testStartupPhase!=TEST_STARTUP_SUBSCRIBED)
    {
        qDebug() << "Startup Error - Timed out\n";
        emit notifyException(MW_UI_LEVEL_ERR);
    }
}

/****************************************************************************
 * beatTheHeart
 *
 * For testing, this function generate a heartbeat to test that your applications
 * code responds to it. It is called from a timer interrupt.
 ****************************************************************************/
void AppManIf::beatTheHeart()
{
    if (heartbeatOutstanding)
    {
        // The previous heartbeat was not answered yet
        qDebug() << "ERROR: Your process failed to respond to the heartbeat\n";
       emit notifyException(MW_UI_LEVEL_ERR);
    }
    else
    {
        // Send the heartbeat notification to your applications code
        qDebug() << "Ping...";
        heartbeatOutstanding=true;
        emit heartbeatNotification();
    }
}
