#include "canlib.h"
#include "canparser.h"

Canlib::Canlib()
{
}

Canlib::~Canlib()
{

}

const CanParser& Canlib::getCanParser() const
{
    return m_canParser;
}

