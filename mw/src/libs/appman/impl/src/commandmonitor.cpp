#include "commandmonitor.h"
#include "msgmanager.h"
#include <QByteArray>
#include <QDebug>

CommandMonitor::CommandMonitor()
{
}

CommandMonitor::~CommandMonitor()
{
}
void CommandMonitor::run()
{
    bool stop=false;
    MsgManager msg;

    while (stop==false)
    {
        msg.receiveMessage();
#ifdef TEST_BUILD
        stdioMessage msgStr;
        msgStr=msg.serialize();
        if (strncmp(msgStr.msg,TEST_APPMAN_MSG,strlen(TEST_APPMAN_MSG))==0)
        {
            emit testInstructionMsg(msgStr.msg);
        }
        else
#endif
        {
            /* No one asked us to provide a facility to allow the SDK to terminate the MW and UI. This code will work.should the request be made.
             * However, it achieves shutdown by pretenting that the UI sent a Power Off notification.
            if (msg.msgId()==MESSAGE_TERMINATE)
            {
                stop=true;
                uiPowerNotification(UI_NOTIFY_POWER_OFF);
            }
            */
            if (msg.msgId()==MESSAGE_HEARTBEAT)
                MsgManager::returnHeartbeat();
        }
    }
}
