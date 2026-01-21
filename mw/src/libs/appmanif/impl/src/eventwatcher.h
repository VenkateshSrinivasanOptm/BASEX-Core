#ifndef EVENTWATCHER_H
#define EVENTWATCHER_H

#include <QThread>
#include <QDebug>
#include "msgmanager.h"

class EventWatcher : public QThread
{
   Q_OBJECT

public:
    EventWatcher();
    ~EventWatcher();
     void run(void);
signals:
    void eventFired(const stdioMessage msg);
private:
    bool stop;
};

#endif // EVENTWATCHER_H
