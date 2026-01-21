#include "eventwatcher.h"
#include "msgmanager.h"

EventWatcher::EventWatcher()
{
    stop=false;
}
EventWatcher::~EventWatcher()
{
}

// Re-implementation of QThread::run()
void EventWatcher::run()
{
    MsgManager msg;

    while (stop==false)
    {
        msg.receiveMessage();
        if (msg.msgId()!=MESSAGE_UNKNOWN_TYPE)
        {
            if (msg.msgId()==MESSAGE_TERMINATE)
                stop=true;
            emit eventFired(msg.serialize());
        }
        else
        {
            stdioMessage message=msg.serialize();
            // qDebug() << "ERROR - Unrecognized stdio message " << msg.msgId() << message.msg;
        }
    }
}


