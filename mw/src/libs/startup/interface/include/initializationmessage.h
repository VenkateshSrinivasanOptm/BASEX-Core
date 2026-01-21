#pragma once

#include<message.h>
#include<comlibservices.h>

struct InitializationMessage : public Message
{
        enum Type {
          START,
          CONFIGURED,
          UNCONFIGURED,
          AUTHENTICATION_ERROR,
          NETWORK_ERROR,
          SW_UPDATE,
          INITIALIZATION_FINISHED,
          UNEXPECTED_ERROR,
          CAN_READY,
          DB_READY
        };

        Type type;

        InitializationMessage(Type _type):Message(BLSTARTUP_INTIALIZATION), type(_type){}
        InitializationMessage():Message(BLSTARTUP_INTIALIZATION){}
        virtual ~InitializationMessage(){}

        virtual void serialize(QDataStream &stream) const
        {
                stream << (quint8) type;
        }

        virtual void deserialize(QDataStream &stream)
        {
            quint8 i;
            stream >> i;
            type=(Type)i;
        }
};
