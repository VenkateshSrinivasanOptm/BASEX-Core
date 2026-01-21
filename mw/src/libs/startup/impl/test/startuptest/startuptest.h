#ifndef STARTUPTEST_H
#define STARTUPTEST_H

#include<QTest>

class StartupTest: public QObject
{
    Q_OBJECT

public:
    StartupTest();

private slots:
    void test();
    void test2();
    void cleanupTestCase();

};

#endif // STARTUPTEST_H
