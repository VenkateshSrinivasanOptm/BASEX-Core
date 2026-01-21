#ifndef MSGMANAGER_H
#define MSGMANAGER_H

#include <QObject>
#include <QDebug>
#include "appmanmsgif.h"

class HALLogger;
class QProcess;

#define MESSAGE_UNKNOWN_TYPE 'u'
#define MESSAGE_SUBSCRIBE 's'
#define MESSAGE_PUBLISH 'p'
#define MESSAGE_TERMINATE 't'
#define MESSAGE_NOTIFY 'n'
#define MESSAGE_ASK_RESTART 'a'
#define MESSAGE_ASK_RESTART_RESPONSE 'r'
#define MESSAGE_UI_CHANGE_OF_POWER 'c'
#define MESSAGE_HEARTBEAT 'h'

#define STDIO_MESSAGE_SIZE 200
typedef struct
{
    char msg[STDIO_MESSAGE_SIZE];
} stdioMessage;

class MsgManager : public QObject
{
    Q_OBJECT

public:
    MsgManager();
    MsgManager(char id, int p1);
    MsgManager(stdioMessage msg);
    virtual ~MsgManager();
    int msgId(void);
    StateChange msgNotificationState(void);
    stdioMessage serialize(void);
  //  char *toStr(void);
    static void sendPublished(const char* appname,const char* appver);
    static void sendCommunicationEstablished(void);
    static void sendUiPowerChangeNotification(UiPowerNotification state);
    static void sendUiRestartResponse(bool response);
    void receiveMessage(void);
    void receiveMessage(QProcess *process);

    //Interface used exclusively by the Application Manager
    static bool getPublished(QProcess *process,QByteArray &appNameAndVer);
    static void sendEstablishCommunication(QProcess *process, char powerState);
    static bool getCommunicationEstablished(QProcess *process);
    static void sendHeartbeat(QProcess *process);
    static void returnHeartbeat();
    bool getUiRestartResponse();
    UiPowerNotification getUiPowerNotificationState();
    bool sendMessage(QProcess *process);
#ifdef TEST_BUILD
    static void testSendMessageToTest(QByteArray msg);
#endif

private:
    char msgIdentifier;
    int p1;
    stdioMessage msg;
};

#endif // MSGMANAGER_H
