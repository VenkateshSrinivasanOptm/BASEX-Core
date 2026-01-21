#ifndef WIFISIGNALSTRENGTHMESSAGE_H
#define WIFISIGNALSTRENGTHMESSAGE_H

#include <comlibservices.h>
#include <message.h>

struct WifiSignalStrengthMessage : public Message
{
    int m_signalStrength;

    WifiSignalStrengthMessage() : Message(WifiSignalStrengthMessageMacro)
    {
    }

    WifiSignalStrengthMessage(const int &signalStrength):Message(WifiSignalStrengthMessageMacro)
    {
        m_signalStrength = signalStrength;
    }

    virtual ~WifiSignalStrengthMessage() {}

    virtual void serialize(QDataStream &stream) const { stream << m_signalStrength; }

    virtual void deserialize(QDataStream &stream) { stream >> m_signalStrength; }
};


#endif // WIFISIGNALSTRENGTHMESSAGE_H
