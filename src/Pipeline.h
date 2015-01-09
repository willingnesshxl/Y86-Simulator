#ifndef PIPE_H
#define PIPE_H
#include<iostream>
#include"Memory.h"
#include"Register.h"
#include"ConditionCode.h"
#include"ALU.h"
using namespace std;
#define MEM_SIZE 0x800
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


#define REAX 0
#define RECX 1
#define REDX 2
#define REBX 3
#define RESP 4
#define REBP 5
#define RESI 6
#define REDI 7
#define RNONE 8
class execute;
class memory;
class r_Memory;
class r_Write;
class r_Execute;
class r_Fetch
{
private:
    int F_predPC;
    int stall;
public:
    r_Fetch();

    int getF_predPC();

    void setF_predPC(int newPC);

    void setstall(int s);

    int getstall();

    void reset();


};
class fetch
{
private:
    int f_stat;
    int f_pc;
    int f_icode;
    int f_ifun;
    int f_rA;
    int f_rB;
    int f_valC;
    int f_valP;
    int predPC;
public:
    fetch();

    void reset();

    bool update(codeMemory, r_Fetch , r_Memory , r_Write, r_Execute );

    int getf_icode();

    int getf_ifun();

    int getf_rA();

    int getf_rB();

    int getf_valC();

    int getf_valP();

    int getpredPC();

    int getPC();



};

class r_Decode
{
private:
    int D_stat;
    int D_icode;
    int D_ifun;
    int D_rA;
    int D_rB;
    int D_valC;
    int D_valP;
    int stall;
    int bubble;
    int pc;
public:
    r_Decode();

    void reset();

    void setD_stat(int stat);

    void setD_icode(int icode);

    void setD_ifun(int ifun);

    void setD_rA(int rA);

    void setD_rB(int rB);

    void setD_valC(int valC);

    void setD_valP(int valP);

    void setPC(int addr);

    int getPC();

    int getD_stat();

    int getD_icode();

    int getD_ifun();

    int getD_rA();

    int getD_rB();

    int getD_valP();

    int getD_valC();

    void setstall(int s);

    int getstall();

    void setbubble(int b);

    int getbubble();

};

class decode
{
private:
private:
    int d_stat;
    int d_icode;
    int d_ifun;
    int d_valC;
    int d_valA;
    int d_valB;
    int d_dstE;
    int d_dstM;
    int d_srcA;
    int d_srcB;
    int stall;
    int pc;
public:
    decode();
    void reset();

    void update(r_Decode, Register, execute , memory , r_Execute , r_Memory, r_Write);

    int getd_stat();

    int getd_icode();

    int getd_ifun();

    int getd_valA();

    int getd_valB();

    int getd_valC();

    int getd_dstE();

    int getd_dstM();

    int getd_srcA();

    int getd_srcB();

    void setstall(int s);

    int getstall();

    int getPC();

};

class r_Execute
{
private:
    int E_stat;
    int E_icode;
    int E_ifun;
    int E_valC;
    int E_valA;
    int E_valB;
    int E_dstE;
    int E_dstM;
    int E_srcA;
    int E_srcB;
    int bubble;
    int pc;
public:
    r_Execute();

    void reset();

    int getE_stat();

    int getE_icode();

    int getE_ifun();

    int getE_valA();

    int getE_valB();

    int getE_valC();

    int getE_dstE();

    int getE_dstM();

    int getE_srcA();

    int getd_srcB();

    int getPC();

    void setPC(int addr);

    void setE_stat(int stat);

    void setE_icode(int icode);

    void setE_ifun(int ifun);

    void setE_valA(int valA);

    void setE_valB(int valB);

    void setE_valC(int valC);

    void setE_dstE(int dstE);

    void setE_dstM(int dstM);

    void setE_srcA(int srcA);

    void setE_srcB(int srcB);

    void setbubble(int b);

    int getbubble();


};

class execute
{
private:
    int e_stat;
    int e_icode;
    int e_Cnd;
    int e_valE;
    int e_valA;
    int e_dstE;
    int e_dstM;
    CC cc;
    ALU alu;
    int pc;
public:
    execute();

    void reset();

    void update(r_Execute Execute);

    int gete_stat();

    int gete_icode();

    int gete_valA();

    int gete_valE();

    int gete_dstE();

    int gete_dstM();

    int gete_Cnd();

    int getPC();

    CC gete_CC();

};

class r_Memory
{
private:
    int M_stat;
    int M_icode;
    int M_Cnd;
    int M_valE;
    int M_valA;
    int M_dstE;
    int M_dstM;
    int pc;
public:
    r_Memory();

    void reset();

    int getM_stat();

    int getM_icode();

    int getM_valA();

    int getM_valE();

    int getM_dstE();

    int getM_dstM();

    int getM_Cnd();

    int getPC();

    void setPC(int addr);

    void setM_stat(int stat);

    void setM_icode(int icode);

    void setM_valA(int valA);

    void setM_valE(int valE);

    void setM_dstE(int dstE);

    void setM_dstM(int dstM);

    void setM_Cnd(int Cnd);

};

class memory
{
private:
    int m_Addr;
    int m_valE;
    int m_stat;
    int m_icode;
    int m_dstE;
    int m_dstM;
    int m_valM;
    int pc;
public:
    memory();

    void reset();

    bool update(r_Memory Memory, codeMemory &memory);

    int getm_stat();

    int getm_icode();

    int getm_valM();

    int getm_valE();

    int getm_dstE();

    int getm_dstM();

    int getPC();


};

class r_Write
{
private:
    int W_stat;
    int W_icode;
    int W_valE;
    int W_valM;
    int W_dstE;
    int W_dstM;
    int pc;
public:
    r_Write();

    void reset();

    int getW_stat();

    int getW_icode();

    int getW_valM();

    int getW_valE();

    int getW_dstE();

    int getW_dstM();

    int getPC();

    void setPC(int addr);

    void setW_stat(int stat);

    void setW_icode(int icode);

    void setW_valM(int valM);

    void setW_valE(int valE);

    void setW_dstE(int dstE);

    void setW_dstM(int dstM);

};

class write
{
private:
    int w_stat;
    int w_valE;
    int w_valM;
public:
    write();

    void reset();

    void update(r_Write Write, Register &reg);

};



class Pipecontrol
{
public:
    void setControl(r_Fetch &F, r_Decode &D, r_Execute &E, r_Memory &M, r_Write &W, decode &d, execute &e);

};
#endif
