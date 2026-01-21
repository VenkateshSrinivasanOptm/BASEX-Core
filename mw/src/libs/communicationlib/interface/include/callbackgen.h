#ifndef CALLBACKGEN_H
#define CALLBACKGEN_H

#include"callback.h"

/**
  * this class can be used for the defining the callback  and deserializer function for handling
  * a received message
  **/

template<class T> class CallBackGen: public CallBack
{
public:

    /**
      * defines the callback function when a reply message is received
      **/
    typedef bool (T::*CallBackFn)(quint16 requestId, MessagePtr replyMessage);

    /**
      * defines the deserializer function for deserializing the reply message
      **/
    typedef MessagePtr (T::*DeserializeFn)(quint16 type , QDataStream &stream);

    CallBackGen(T *_instance, CallBackFn _callBackFn, DeserializeFn _deserializeFn ):
            instance(_instance), callBackFn(_callBackFn), deserializeFn(_deserializeFn){}

    virtual ~CallBackGen(){}

    /**
      * this method executes the callBackFn from class T
      **/
    virtual bool execute(quint16 requestId, MessagePtr message){
        return (instance->*callBackFn)(requestId,message);}

    /**
      * This method executes the deserializeFn from class T
      **/
    virtual MessagePtr deserialize(quint16 type, QDataStream &stream){
        return (instance->*deserializeFn)(type,stream);}

private:
    T *instance;
    CallBackFn callBackFn;
    DeserializeFn deserializeFn;
};

#endif // CALLBACKGEN_H
