#ifndef PROGRAMINFOQUERIER_H
#define PROGRAMINFOQUERIER_H

#include<qglobal.h>

class ProgramInfoQuerier
{
public:
    ProgramInfoQuerier();
    bool enoughRemainingEPGData(quint16 i);
};

#endif // PROGRAMINFOQUERIER_H
