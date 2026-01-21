#ifndef CALLER_H
#define CALLER_H

#include <deserializer.h>
#include <messagehandler.h>

struct Callee
{
    Deserializer *d;
    MessageHandler *h;
    Callee(Deserializer *_d, MessageHandler *_h): d(_d), h(_h) {}
    Callee() {}
};

#endif // CALLER_H
