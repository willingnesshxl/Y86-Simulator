#include"ConditionCode.h"

CC::CC()
{
    ZF = 0;
    SF = 0;
    OF = 0;

}
void CC::reset()
{
    ZF = 0;
    SF = 0;
    OF = 0;
}

void CC::setZF(int zf)
{
    ZF = zf;
}
void CC::setSF(int sf)
{
    SF = sf;
}
void CC::setOF(int of)
{
    OF = of;
}
int CC::getZF()
{
    return ZF;
}
int CC::getSF()
{
    return SF;
}
int CC::getOF()
{
    return OF;
}

