#ifndef COMMANDMONITOR_H
#define COMMANDMONITOR_H

#include <QThread>
#include <QByteArray>
#ifdef TEST_BUILD
#include "appmantest.h"
#endif

class CommandMonitor : public QThread
{
    Q_OBJECT

public:
    CommandMonitor();
    virtual ~CommandMonitor();
    void run(void);
signals:
#ifdef TEST_BUILD
    void testInstructionMsg(QByteArray line);
#endif
};

#endif // COMMANDMONITOR_H
