#ifndef CBUSFORWARDINTF_H
#define CBUSFORWARDINTF_H

#include "sendobject.h"

class CBusForwardIntf
{
   public:
        CBusForwardIntf(){}
        virtual ~CBusForwardIntf(){}
        virtual void forwardObject(SendObjectPtr const &obj)=0;
};

#endif // CBUSFORWARDINTF_H
