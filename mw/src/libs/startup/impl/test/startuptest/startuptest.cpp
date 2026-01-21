#include "startuptest.h"
#include "request.h"
#include "request2.h"

StartupTest::StartupTest()
{
}

void StartupTest::test()
{
   Request *request;
   request = new Request();
   request->test();
   delete request;
}

void StartupTest::test2()
{
   Request2 *request;
   request = new Request2();
   request->test();
   delete request;
}

void StartupTest::cleanupTestCase()
{
    //exit(0);
}

QTEST_MAIN(StartupTest)
