#include"ALU.h"

ALU::ALU()
{
    aluA = 0;
    aluB = 0;
    valE = 0;
    ALUfun = 0;;
}
void ALU::setaluA(int valC, int valA, int icode)
{
    switch (icode)
    {
    case IRRMOVL:
    case IOPL:
        aluA = valA;
        break;
    case IIRMOVL:
    case IRMMOVL:
    case IMRMOVL:
        aluA = valC;
        break;
    case ICALL:
    case IPUSHL:
        aluA = -4;
        break;
    case IRET:
    case IPOPL:
        aluA = 4;
        break;
    default:
        aluA = 0;
    }

}
void ALU::setvalB(int valB, int icode)
{
    switch (icode)
    {
    case IRMMOVL:
    case IMRMOVL:
    case IOPL:
    case ICALL:
    case IPUSHL:
    case IRET:
    case IPOPL:
        aluB = valB;
        break;
    default:
        aluB = 0;
    }
}
void ALU::setALUfun(int icode, int ifun)
{
    if (icode == IOPL)
    {
        ALUfun = ifun;
    }
    else
        ALUfun = 0;

}
int ALU::calculatevalE()
{
    switch (ALUfun)
    {
    case 1:
        valE = aluB - aluA;
        break;
    case 2:
        valE = aluA & aluB;
        break;
    case 3:
        valE = aluA ^ aluB;
        break;
    case 0:
        valE = aluA + aluB;
        break;
    default:
        valE = 0;
        break;

    }
    return valE;
}
int ALU::getaluA()
{
    return aluA;
}
int ALU::getaluB()
{
    return aluB;
}
int ALU::getvalE()
{
    return valE;
}
