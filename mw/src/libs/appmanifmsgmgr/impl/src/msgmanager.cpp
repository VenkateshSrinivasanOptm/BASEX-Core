#include "msgmanager.h"
#include "msgmanagerimpl.h"
#include <QDebug>
#include <QProcess>

/************************************************************************************
 *  MsgManager
 *
 * Defines a message. A message consists of a messgage identifier and one parameter (p1)
 * The message can be defined as -
 *  (char *) - This is the format as received via the communications channel stdio
 *  Message  - This is a private format of MsgManager that can be used when passing the
 *             message data in signals. Convertion is via serialize() function.
 *  int,int  - Allows direct assignment of single parameter messages.
 *
 ************************************************************************************/
MsgManager::MsgManager() /*: m_pLogger(m_pLogger = getLogger("appmanifmsgmgr"))*/
{
    msgIdentifier=MESSAGE_UNKNOWN_TYPE;
    msg.msg[0]=0;
    p1=0;
}
MsgManager::MsgManager(stdioMessage message)
{
    msg=message;
//    strcpy(msg.msg,message.msg);

    // Extract the message identifier
    msgIdentifier=message.msg[0];
    p1 = message.msg[1] - '0';
}
MsgManager::MsgManager(char id, int param1)
{
    msgIdentifier=id;
    p1 = param1;
    msg.msg[0]=msgIdentifier;
    msg.msg[1]=p1 + '0';
    msg.msg[2]=0;
    //qDebug() << msgIdentifier << "," << p1 << "\n";
}
MsgManager::~MsgManager()
{
}
/************************************************************************************
 * msgId
 *
 * Returns the message identifier of the message.
 *
 ************************************************************************************/
int MsgManager::msgId(void)
{
    return msgIdentifier;
}
/************************************************************************************
 * msgNotificationState
 *
 * For Notification messages this returns the State being notified
 ************************************************************************************/
StateChange MsgManager::msgNotificationState(void)
{
    if (msgIdentifier!=MESSAGE_NOTIFY && msgIdentifier!=MESSAGE_TERMINATE && msgIdentifier!=MESSAGE_SUBSCRIBE)
    {
        return POWER_RESET_STATE_UNKNOWN;
    }
    return (StateChange) p1;
}

/************************************************************************************
 * serialize
 *
 * This function converts a message into type Message for passing in signals etc.
 *
 ************************************************************************************/
stdioMessage MsgManager::serialize(void)
{
    return msg;
}

void MsgManager::receiveMessage()
{
    msgIdentifier=MESSAGE_UNKNOWN_TYPE;

    msg.msg[0]=fgetc(stdin);
    if (msg.msg[0]==MESSAGE_GUARD_CHAR)
    {
        fgets(msg.msg,STDIO_MESSAGE_SIZE,stdin);  //reads upto the next newline or until 200 chars.

        // Extract the message identifier
        int len=strlen(msg.msg);
        if (len>1)
        {
             msgIdentifier=msg.msg[0];
             if (len>1)
                 p1 = msg.msg[1] - '0';

             // remove any appended newline
             if (msg.msg[strlen(msg.msg) - 1]=='\n')
                 msg.msg[strlen(msg.msg) - 1]=0;
        }
    }
    else
    {
        fgets(&msg.msg[1],STDIO_MESSAGE_SIZE - 1,stdin);  //reads upto the next newline or until 200 chars.
      //TODO Madhur qDebug() << "WARN: Skipped unrecognised stdio = " << msg.msg;
    }
}
void MsgManager::receiveMessage(QProcess *process)
{
    QByteArray line=process->readLine();

    // Extract the message identifier
    msgIdentifier=MESSAGE_UNKNOWN_TYPE;
    if (line.count()>=2)
    {
        if (line[0]==MESSAGE_GUARD_CHAR)
        {
            msgIdentifier=line[1];
            p1 = line[2] - '0';
        }
    }
    if (msgIdentifier!=MESSAGE_UNKNOWN_TYPE)
    {
        strncpy(msg.msg,line.remove(0,1),STDIO_MESSAGE_SIZE);

       // remove any appended newline
       if (msg.msg[strlen(msg.msg) - 1]=='\n')
           msg.msg[strlen(msg.msg) - 1]=0;
    }
    else
    {
        qDebug() << "ERROR UNKNOWN Msg Received = " << msg.msg;
    }
}
bool MsgManager::getUiRestartResponse()
{
    if (p1==0)
        return false;
    return true;
}

UiPowerNotification MsgManager::getUiPowerNotificationState()
{
    return (UiPowerNotification) p1;
}

/************************************************************************************
 * sendPublished
 *
 * This function sends a published message to the Applications Manager
 *
 ************************************************************************************/
void MsgManager::sendPublished(const char* appname, const char* appver)
{
    qDebug() << "MsgManager::sendPublished " << appname << appver;
    fputc(MESSAGE_GUARD_CHAR,stdout);
    fputc(MESSAGE_PUBLISH,stdout);
    fputs(appname,stdout);
    fputs(":",stdout);
    fputs(appver,stdout);
    fputs("\n",stdout);
    fflush(stdout);
}
/************************************************************************************
 * getPublished
 *
 * This function receives a published message from the MW. It  is used by the
 * Applications Manager
 *
 ************************************************************************************/
bool MsgManager::getPublished(QProcess *process,QByteArray &appNameAndVer)
{
    int msgId=MESSAGE_UNKNOWN_TYPE;
    appNameAndVer=process->readLine();
    qDebug() << "appNameAndVer " << appNameAndVer;
    appNameAndVer.truncate(appNameAndVer.length()-1);
    if (appNameAndVer.count()>=2)
    {
        if (appNameAndVer[0]==MESSAGE_GUARD_CHAR)
            msgId=appNameAndVer[1];
    }
    else
        return false;
    if (msgId!=MESSAGE_PUBLISH)
        return false;
    appNameAndVer.remove(0,2);
    return true;
}
/************************************************************************************
 * sendCommunicationEstablished
 *
 * This function sends a subscribed message from the MW module to the Applications Manager
 *
 ************************************************************************************/
void MsgManager::sendCommunicationEstablished()
{
   fputc(MESSAGE_GUARD_CHAR,stdout);
   fputc(MESSAGE_SUBSCRIBE,stdout);
   fputs("\n",stdout);
   fflush(stdout);
}
/************************************************************************************
 * getCommunicationEstablished
 *
 * This function receives a subscribed message from the MW module
 *
 ************************************************************************************/
bool MsgManager::getCommunicationEstablished(QProcess *process)
{
   QByteArray msg=process->readLine();
   if (msg.count()>=2)
       if (msg[0]==MESSAGE_GUARD_CHAR)
           if (msg[1]==MESSAGE_SUBSCRIBE)
               return true;

   return false;
}
/************************************************************************************
 * sendEstablishCommunication
 *
 * This function sends a subscribe message from the Applications Manager to the MW module
 *
 ************************************************************************************/
void MsgManager::sendEstablishCommunication(QProcess *process, char powerState)
{
   qDebug() << "MsgManager::sendEstablishCommunication process Id " << process->processId();
   process->putChar(MESSAGE_GUARD_CHAR);
   process->putChar(MESSAGE_SUBSCRIBE);
   process->putChar(powerState);
   process->putChar('\n');
}
void MsgManager::sendUiPowerChangeNotification(UiPowerNotification state)
{
    fputc(MESSAGE_GUARD_CHAR,stdout);
    fputc(MESSAGE_UI_CHANGE_OF_POWER,stdout);
    fputc(state + '0',stdout);
    fputc('\n',stdout);
    fflush(stdout);
}

void MsgManager::sendUiRestartResponse(bool response)
{
    fputc(MESSAGE_GUARD_CHAR,stdout);
    fputc(MESSAGE_ASK_RESTART_RESPONSE,stdout);
    if (response)
        fputc('1',stdout);
    else
        fputc('0',stdout);
    fputc('\n',stdout);
    fflush(stdout);
}

/************************************************************************************
 * sendMessage
 *
 * This function is used by the Applications Manager to send a message to the MW module.
 * This is an alternative to the static calls that uses the class data to determine the
 * type and content of the message to be sent.
 *
 ************************************************************************************/
bool MsgManager::sendMessage(QProcess *process)
{
    if (msgIdentifier==MESSAGE_UNKNOWN_TYPE)
        return false;

    process->putChar(MESSAGE_GUARD_CHAR);
    process->putChar(msgIdentifier);

    //Output a parameter for those message types that require one
    switch(msgIdentifier)
    {
        case MESSAGE_TERMINATE:
        case MESSAGE_NOTIFY:
        case MESSAGE_ASK_RESTART:
        process->putChar(p1 + '0');
    }
    process->putChar('\n');
    return true;
}
void MsgManager::sendHeartbeat(QProcess *process)
{
    qDebug() << "sendHeartbeat " << MESSAGE_GUARD_CHAR << " from process " << process->processId();
    process->putChar(MESSAGE_GUARD_CHAR);
    process->putChar(MESSAGE_HEARTBEAT);
    process->putChar('\n');
    fflush(stdout);
    return;
}

void MsgManager::returnHeartbeat()
{
    fputc(MESSAGE_GUARD_CHAR,stdout);
    fputc(MESSAGE_HEARTBEAT,stdout);
    fputc('\n',stdout);
    fflush(stdout);
    return;
}

#ifdef TEST_BUILD
void MsgManager::testSendMessageToTest(QByteArray msg)
{
    fputc(MESSAGE_GUARD_CHAR,stdout);
    fputs(msg,stdout);
    fputs("\n",stdout);
    fflush(stdout);
}
#endif
