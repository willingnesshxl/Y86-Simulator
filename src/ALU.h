#ifndef ALU_H
#define ALU_H
#define INOP    0x0
#define IHALT   0x1
#define IRRMOVL 0x2
#define IIRMOVL 0x3
#define IRMMOVL 0x4
#define IMRMOVL 0x5
#define IOPL    0x6
#define IJXX    0x7
#define ICALL   0x8
#define IRET    0x9
#define IPUSHL  0xA
#define IPOPL   0xB

class ALU
{
private:
    int aluA;
    int aluB;
    int valE;
    int ALUfun;
public:
    ALU();

    void setaluA(int valC, int valA, int icode);

    void setvalB(int valB, int icode);

    void setALUfun(int icode, int ifun);

    int calculatevalE();

    int getaluA();

    int getaluB();

    int getvalE();

};

#endif
