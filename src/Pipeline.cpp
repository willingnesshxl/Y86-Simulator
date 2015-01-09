#include"Pipeline.h"
#include<iostream>
#include"Memory.h"
#include"Register.h"
#include"ConditionCode.h"
#include"ALU.h"
using namespace std;
void decode::update(r_Decode Decode, Register reg, execute e, memory m, r_Execute E, r_Memory M, r_Write W)
{

    pc = Decode.getPC();
    d_ifun = Decode.getD_ifun();
    d_stat = Decode.getD_stat();
    d_valC = Decode.getD_valC();
    d_icode = Decode.getD_icode();
    //get srcA
    if (d_icode == IRRMOVL || d_icode == IRMMOVL || d_icode == IOPL || d_icode == IPUSHL)
        d_srcA = Decode.getD_rA();
    else if (d_icode == IPOPL || d_icode == IRET)
        d_srcA = RESP;
    else
        d_srcA = RNONE;

    //get srcB
    if (d_icode == IOPL || d_icode == IRMMOVL || d_icode == IMRMOVL)
        d_srcB = Decode.getD_rB();
    else if (d_icode == IPUSHL || d_icode == IPOPL || d_icode == ICALL || d_icode == IRET)
        d_srcB = RESP;
    else
        d_srcB = RNONE;
    //get dstE
    if (d_icode == IRRMOVL || d_icode == IIRMOVL || d_icode == IOPL)
        d_dstE = Decode.getD_rB();
    else if (d_icode == IPUSHL || d_icode == IPOPL || d_icode == ICALL || d_icode == IRET)
        d_dstE = RESP;
    else
        d_dstE = RNONE;
    //get dstM
    if (d_icode == IMRMOVL || d_icode == IPOPL)
        d_dstM = Decode.getD_rA();
    else
        d_dstM = RNONE;


    if (d_icode == ICALL || d_icode == IJXX)
        d_valA = Decode.getD_valP();
    else if (d_srcA == E.getE_dstE() && E.getE_dstE() < 8)
        d_valA = e.gete_valE();
    else if (d_srcA == M.getM_dstM() && M.getM_dstM() < 8)
        d_valA = m.getm_valM();
    else if (d_srcA == M.getM_dstE() && M.getM_dstE() < 8)
        d_valA = M.getM_valE();
    else if (d_srcA == W.getW_dstM() && W.getW_dstM() < 8)
        d_valA = W.getW_valM();
    else if (d_srcA == W.getW_dstE() && W.getW_dstE() < 8)
        d_valA = W.getW_valE();
    else
        d_valA = reg.getRegister(d_srcA);

    if (d_srcB == E.getE_dstE() && E.getE_dstE() < 8)
        d_valB = e.gete_valE();
    else if (d_srcB == M.getM_dstM() && M.getM_dstM() < 8)
        d_valB = m.getm_valM();
    else if (d_srcB == M.getM_dstE() && M.getM_dstE() < 8)
        d_valB = M.getM_valE();
    else if (d_srcB == W.getW_dstM() && W.getW_dstM() < 8)
        d_valB = W.getW_valM();
    else if (d_srcB == W.getW_dstE() && W.getW_dstE() < 8)
        d_valB = W.getW_valE();
    else
        d_valB = reg.getRegister(d_srcB);

}

bool fetch::update(codeMemory memory, r_Fetch Fetch, r_Memory M, r_Write W, r_Execute E)
{

    if (E.getE_icode() != IHALT && M.getM_icode() != IHALT && W.getW_icode() != IHALT)
    {
        unsigned char temp_char;

        int pc;
        if (M.getM_icode() == IJXX && M.getM_Cnd() == 0)
            pc = M.getM_valA();
        else if (W.getW_icode() == IRET)
            pc = W.getW_valM();
        else
            pc = Fetch.getF_predPC();
        f_pc = pc;
        if (!memory.Read_Byte_Memory(pc, &temp_char))
            return false;
        f_ifun = temp_char & 0xf;
        f_icode = (temp_char >> 4) & 0xf;

        pc++;
        int need_regids = 0;
        if (f_icode == IRRMOVL || f_icode == IOPL || f_icode == IPUSHL || f_icode == IPOPL || f_icode == IIRMOVL || f_icode == IRMMOVL || f_icode == IMRMOVL)
            need_regids = 1;
        if (need_regids == 1)
        {
            if (!memory.Read_Byte_Memory(pc, &temp_char))
                return false;
            f_rB = temp_char & 0xf;
            f_rA = (temp_char >> 4) & 0xf;
            pc++;
        }
        else
        {
            f_rA = RNONE;
            f_rB = RNONE;
        }
        int need_valC = 0;
        if (f_icode == IIRMOVL || f_icode == IRMMOVL || f_icode == IMRMOVL || f_icode == IJXX || f_icode == ICALL)
            need_valC = 1;
        if (need_valC == 1)
        {
            if (!memory.Read_Word_Memory(pc, &f_valC))
                return false;
            pc += 4;
            f_valP = pc;

        }
        else
            f_valC = 0;
        if (f_icode == IJXX || f_icode == ICALL)
            predPC = f_valC;
        else
        {
            predPC = pc;
        }
        return true;
    }
    else
    {
        reset();
        return true;
    }

}
void execute::update(r_Execute Execute)
{

    pc = Execute.getPC();
    e_stat = Execute.getE_stat();
    e_icode = Execute.getE_icode();
    e_dstE = Execute.getE_dstE();
    e_dstM = Execute.getE_dstM();
    e_valA = Execute.getE_valA();
    alu.setaluA(Execute.getE_valC(), Execute.getE_valA(), e_icode);
    alu.setvalB(Execute.getE_valB(), e_icode);
    alu.setALUfun(e_icode, Execute.getE_ifun());
    e_valE = alu.calculatevalE();
    if (e_icode == IOPL)
    {

        // Set Condition Code Here

        cc.setZF(e_valE == 0 ? 1 : 0);
        cc.setSF(e_valE >= 0 ? 0 : 1);
        cc.setOF(0);
        if (Execute.getE_ifun() == 0)
        {
            if (alu.getaluB() > 0 && alu.getaluA() > 0 && e_valE < 0)
                cc.setOF(1);
            if (alu.getaluB() < 0 && alu.getaluA() < 0 && e_valE > 0)
                cc.setOF(1);
        } else if (Execute.getE_ifun() == 1)
        {
            if (alu.getaluB() > 0 && alu.getaluA() < 0 && e_valE < 0)
                cc.setOF(1);
            if (alu.getaluB() < 0 && alu.getaluA() > 0 && e_valE > 0)
                cc.setOF(1);
        }
    }
    e_Cnd = 0;
    if (e_icode == IJXX)
    {
        switch (Execute.getE_ifun())
        {
        case 0:
            e_Cnd = 1;
            break;
        case 1:
            if (cc.getZF() == 1 || cc.getSF() == 1)
                e_Cnd = 1;
            break;
        case 2:
            if (cc.getSF() == 1)
                e_Cnd = 1;
            break;
        case 3:
            if (cc.getZF() == 1)
                e_Cnd = 1;
            break;
        case 4:
            if (cc.getZF() != 1)
                e_Cnd = 1;
            break;
        case 5:
            if (cc.getSF() == 0)
                e_Cnd = 1;
            break;
        case 6:
            if (cc.getSF() == 0 && cc.getZF() == 0)
                e_Cnd = 1;
            break;
        default:
            e_Cnd = 0;
            break;
        }
    }


}

bool memory::update(r_Memory Memory, codeMemory &memory)
{
    pc = Memory.getPC();
    m_stat = Memory.getM_stat();
    m_dstE = Memory.getM_dstE();
    m_dstM = Memory.getM_dstM();
    m_icode = Memory.getM_icode();
    m_valE = Memory.getM_valE();
    m_Addr = 0;
    switch (m_icode)
    {
    case IRMMOVL:
    case IPUSHL:
    case ICALL:
    case IMRMOVL:
        m_Addr = Memory.getM_valE();
        break;
    case IPOPL:
    case IRET:
        m_Addr = Memory.getM_valA();
        break;
    default: break;

    }
    switch (m_icode)
    {
    case IMRMOVL:
    case IPOPL:
    case IRET:
        if (!memory.Read_Word_Memory(m_Addr, &m_valM))
        {   return false;
            break;
        }
    case IRMMOVL:
    case IPUSHL:
    case ICALL:
        if (!memory.Write_Word_Memory(Memory.getM_valA(), m_Addr))
        {   return false;
            break;
        }
    default: break;
    }
    return true;
}

void write::update(r_Write Write, Register &reg)
{
    w_valE = Write.getW_valE();
    w_valM = Write.getW_valM();
    reg.setRegister(Write.getW_dstE(), w_valE);
    reg.setRegister(Write.getW_dstM(), w_valM);
}

void Pipecontrol::setControl(r_Fetch &F, r_Decode &D, r_Execute &E, r_Memory &M, r_Write &W, decode &d, execute &e)
{
    if ((E.getE_icode() == IMRMOVL || E.getE_icode() == IPOPL) && (E.getE_dstM() == d.getd_srcA() || E.getE_dstM() == d.getd_srcB()) ||
            (D.getD_icode() == IRET || E.getE_icode() == IRET || M.getM_icode() == IRET))
        F.setstall(1);
    else
        F.setstall(0);
    if ((E.getE_icode() == IMRMOVL || E.getE_icode() == IPOPL) && (E.getE_dstM() == d.getd_srcA() || E.getE_dstM() == d.getd_srcB()))
        D.setstall(1);
    else
        D.setstall(0);
    if ((E.getE_icode() == IJXX && (!e.gete_Cnd())) || (D.getD_icode() == IRET || E.getE_icode() == IRET || M.getM_icode() == IRET))
    {
        D.setbubble(1);
        D.setD_icode(0);
        D.setD_ifun(0);
        D.setD_rA(8);
        D.setD_rB(8);
        D.setD_valC(0);
        D.setD_valP(0);
        D.setPC(0);
    }
    else
        D.setbubble(0);
    if ((E.getE_icode() == IJXX && (!e.gete_Cnd())) ||
            (E.getE_icode() == IMRMOVL || E.getE_icode() == IPOPL) && (E.getE_dstM() == d.getd_srcA() || E.getE_dstM() == d.getd_srcB()))
    {
        E.setbubble(1);
        E.setE_valA(0);
        E.setE_valB(0);
        E.setE_valC(0);
        E.setE_icode(0);
        E.setE_ifun(0);
        E.setE_srcA(8);
        E.setE_srcB(8);
        E.setE_dstE(8);
        E.setE_dstM(8);
        E.setPC(0);
    }
    else
        E.setbubble(0);

}

r_Fetch::r_Fetch()
{
    F_predPC = 0;
    stall = 0;
}
int r_Fetch::getF_predPC()
{
    return F_predPC;
}
void r_Fetch::setF_predPC(int newPC)
{
    F_predPC = newPC;
}
void r_Fetch::setstall(int s)
{
    stall = s;
}
int r_Fetch::getstall()
{
    return stall;
}
void r_Fetch::reset()
{
    F_predPC = 0;
    stall = 0;
}

fetch::fetch()
{
    f_pc = 0;
    f_icode = 0;
    f_ifun = 0;
    f_rA = 0;
    f_rB = 0;
    f_valC = 0;
    f_valP = 0;
    predPC = 0;

}
void fetch::reset()
{
    f_pc = 0;
    f_icode = 0;
    f_ifun = 0;
    f_rA = 0;
    f_rB = 0;
    f_valC = 0;
    f_valP = 0;
    predPC = 0;

}

int fetch::getf_icode()
{
    return f_icode;
}
int fetch::getf_ifun()
{
    return f_ifun;
}
int fetch::getf_rA()
{
    return f_rA;
}
int fetch::getf_rB()
{
    return f_rB;
}
int fetch::getf_valC()
{
    return f_valC;
}
int fetch::getf_valP()
{
    return f_valP;
}
int fetch::getpredPC()
{
    return predPC;
}
int fetch::getPC()
{
    return f_pc;
}

r_Decode::r_Decode()
{
    D_stat = 0;
    D_icode = 0;
    D_ifun = 0;
    D_rA = 0;
    D_rB = 0;
    D_valC = 0;
    D_valP = 0;
    stall = 0;
    bubble = 0;
    pc = 0;
}
void r_Decode::reset()
{
    D_stat = 0;
    D_icode = 0;
    D_ifun = 0;
    D_rA = 0;
    D_rB = 0;
    D_valC = 0;
    D_valP = 0;
    stall = 0;
    bubble = 0;
    pc = 0;
}


void r_Decode::setD_stat(int stat)
{
    D_stat = stat;
}
void r_Decode::setD_icode(int icode)
{
    D_icode = icode;
}
void r_Decode::setD_ifun(int ifun)
{
    D_ifun = ifun;
}
void r_Decode::setD_rA(int rA)
{
    D_rA = rA;
}
void r_Decode::setD_rB(int rB)
{
    D_rB = rB;
}
void r_Decode::setD_valC(int valC)
{
    D_valC = valC;
}
void r_Decode::setD_valP(int valP)
{
    D_valP = valP;
}
void r_Decode::setPC(int addr)
{
    pc = addr;
}

int r_Decode::getPC()
{
    return pc;
}

int r_Decode::getD_stat()
{
    return D_stat;
}
int r_Decode::getD_icode()
{
    return D_icode;
}
int r_Decode::getD_ifun()
{
    return D_ifun;
}
int r_Decode::getD_rA()
{
    return D_rA;
}
int r_Decode::getD_rB()
{
    return D_rB;
}
int r_Decode::getD_valP()
{
    return D_valP;
}
int r_Decode::getD_valC()
{
    return D_valC;
}
void r_Decode::setstall(int s)
{
    stall = s;
}
int r_Decode::getstall()
{
    return stall;
}
void r_Decode::setbubble(int b)
{
    bubble = b;
}
int r_Decode::getbubble()
{
    return bubble;
}

decode::decode()
{
    d_stat = 0;
    d_icode = 0;
    d_ifun = 0;
    d_valC = 0;
    d_valA = 0;
    d_valB = 0;
    d_dstE = 8;
    d_dstM = 8;
    d_srcA = 8;
    d_srcB = 8;
    stall = 0;
    pc = 0;
}
void decode::reset()
{
    d_stat = 0;
    d_icode = 0;
    d_ifun = 0;
    d_valC = 0;
    d_valA = 0;
    d_valB = 0;
    d_dstE = 8;
    d_dstM = 8;
    d_srcA = 8;
    d_srcB = 8;
    stall = 0;
    pc = 0;

}

int decode::getd_stat()
{
    return d_stat;
}
int decode::getd_icode()
{
    return d_icode;
}
int decode::getd_ifun()
{
    return d_ifun;
}
int decode::getd_valA()
{
    return d_valA;
}
int decode::getd_valB()
{
    return d_valB;
}
int decode::getd_valC()
{
    return d_valC;
}
int decode::getd_dstE()
{
    return d_dstE;
}
int decode::getd_dstM()
{
    return d_dstM;
}
int decode::getd_srcA()
{
    return d_srcA;
}
int decode::getd_srcB()
{
    return d_srcB;
}
void decode::setstall(int s)
{
    stall = s;
}
int decode::getstall()
{
    return stall;
}


int decode::getPC()
{
    return pc;
}

r_Execute::r_Execute()
{
    E_stat = 0;
    E_icode = 0;
    E_ifun = 0;
    E_valC = 0;
    E_valA = 0;
    E_valB = 0;
    E_dstE = 8;
    E_dstM = 8;
    E_srcA = 8;
    E_srcB = 8;
    bubble = 0;
    pc = 0;

}
void r_Execute::reset()
{
    E_stat = 0;
    E_icode = 0;
    E_ifun = 0;
    E_valC = 0;
    E_valA = 0;
    E_valB = 0;
    E_dstE = 8;
    E_dstM = 8;
    E_srcA = 8;
    E_srcB = 8;
    bubble = 0;
    pc = 0;

}

int r_Execute::getE_stat()
{
    return E_stat;
}
int r_Execute::getE_icode()
{
    return E_icode;
}
int r_Execute::getE_ifun()
{
    return E_ifun;
}
int r_Execute::getE_valA()
{
    return E_valA;
}
int r_Execute::getE_valB()
{
    return E_valB;
}
int r_Execute::getE_valC()
{
    return E_valC;
}
int r_Execute::getE_dstE()
{
    return E_dstE;
}
int r_Execute::getE_dstM()
{
    return E_dstM;
}
int r_Execute::getE_srcA()
{
    return E_srcA;
}
int r_Execute::getd_srcB()
{
    return E_srcB;
}

int r_Execute::getPC()
{
    return pc;
}

void r_Execute::setPC(int addr)
{
    pc = addr;
}


void r_Execute::setE_stat(int stat)
{
    E_stat = stat;
}
void r_Execute::setE_icode(int icode)
{
    E_icode = icode;
}
void r_Execute::setE_ifun(int ifun)
{
    E_ifun = ifun;
}
void r_Execute::setE_valA(int valA)
{
    E_valA = valA;
}
void r_Execute::setE_valB(int valB)
{
    E_valB = valB;
}
void r_Execute::setE_valC(int valC)
{
    E_valC = valC;
}
void r_Execute::setE_dstE(int dstE)
{
    E_dstE = dstE;
}
void r_Execute::setE_dstM(int dstM)
{
    E_dstM = dstM;
}
void r_Execute::setE_srcA(int srcA)
{
    E_srcA = srcA;
}
void r_Execute::setE_srcB(int srcB)
{
    E_srcB = srcB;
}
void r_Execute::setbubble(int b)
{
    bubble = b;
}
int r_Execute::getbubble()
{
    return bubble;
}

execute::execute()
{

    e_stat = 0;
    e_icode = 0;
    e_Cnd = 0;
    e_valE = 0;
    e_valA = 0;
    e_dstE = 8;
    e_dstM = 8;
    pc = 0;
}
void execute::reset()
{
    e_stat = 0;
    e_icode = 0;
    e_Cnd = 0;
    e_valE = 0;
    e_valA = 0;
    e_dstE = 8;
    e_dstM = 8;
    cc.reset();
    pc = 0;
}

int execute::gete_stat()
{
    return e_stat;
}
int execute::gete_icode()
{
    return e_icode;
}

int execute::gete_valA()
{
    return e_valA;
}

int execute::gete_valE()
{
    return e_valE;
}
int execute::gete_dstE()
{
    return e_dstE;
}
int execute::gete_dstM()
{
    return e_dstM;
}
int execute::gete_Cnd()
{
    return e_Cnd;
}
int execute::getPC()
{
    return pc;
}
CC execute::gete_CC()
{
    return cc;
}

r_Memory::r_Memory()
{
    M_stat = 0;
    M_icode = 0;
    M_Cnd = 0;
    M_valE = 0;
    M_valA = 0;
    M_dstE = 8;
    M_dstM = 8;
    pc = 0;
}

void r_Memory::reset()
{
    M_stat = 0;
    M_icode = 0;
    M_Cnd = 0;
    M_valE = 0;
    M_valA = 0;
    M_dstE = 8;
    M_dstM = 8;
    pc = 0;
}

int r_Memory::getM_stat()
{
    return M_stat;
}
int r_Memory::getM_icode()
{
    return M_icode;
}

int r_Memory::getM_valA()
{
    return M_valA;
}

int r_Memory::getM_valE()
{
    return M_valE;
}
int r_Memory::getM_dstE()
{
    return M_dstE;
}
int r_Memory::getM_dstM()
{
    return M_dstM;
}
int r_Memory::getM_Cnd()
{
    return M_Cnd;
}
int r_Memory::getPC()
{
    return pc;
}

void r_Memory::setPC(int addr)
{
    pc = addr;
}

void r_Memory::setM_stat(int stat)
{
    M_stat = stat;
}
void r_Memory::setM_icode(int icode)
{
    M_icode = icode;
}

void r_Memory::setM_valA(int valA)
{
    M_valA = valA;
}

void r_Memory::setM_valE(int valE)
{
    M_valE = valE;
}
void r_Memory::setM_dstE(int dstE)
{
    M_dstE = dstE;
}
void r_Memory::setM_dstM(int dstM)
{
    M_dstM = dstM;
}
void r_Memory::setM_Cnd(int Cnd)
{
    M_Cnd = Cnd;
}

memory::memory()
{
    m_Addr = 0;
    m_valE = 0;
    m_stat = 0;
    m_icode = 0;
    m_dstE = 8;
    m_dstM = 8;
    m_valM = 0;
    pc = 0;
}

void memory::reset()
{
    m_Addr = 0;
    m_valE = 0;
    m_stat = 0;
    m_icode = 0;
    m_dstE = 8;
    m_dstM = 8;
    m_valM = 0;
    pc = 0;
}

int memory::getm_stat()
{
    return m_stat;
}
int memory::getm_icode()
{
    return m_icode;
}
int memory::getm_valM()
{
    return m_valM;
}

int memory::getm_valE()
{
    return m_valE;
}
int memory::getm_dstE()
{
    return m_dstE;
}
int memory::getm_dstM()
{
    return m_dstM;
}
int memory::getPC()
{
    return pc;
}

r_Write::r_Write()
{
    W_stat = 0;
    W_icode = 0;
    W_valE = 0;
    W_valM = 0;
    W_dstE = 8;
    W_dstM = 8;
    pc = 0;

}
void r_Write::reset()
{
    W_stat = 0;
    W_icode = 0;
    W_valE = 0;
    W_valM = 0;
    W_dstE = 8;
    W_dstM = 8;
    pc = 0;
}

int r_Write::getW_stat()
{
    return W_stat;
}
int r_Write::getW_icode()
{
    return W_icode;
}

int r_Write::getW_valM()
{
    return W_valM;
}

int r_Write::getW_valE()
{
    return W_valE;
}
int r_Write::getW_dstE()
{
    return W_dstE;
}
int r_Write::getW_dstM()
{
    return W_dstM;
}

int r_Write::getPC()
{
    return pc;
}
void r_Write::setPC(int addr)
{
    pc = addr;
}


void r_Write::setW_stat(int stat)
{
    W_stat = stat;
}
void r_Write::setW_icode(int icode)
{
    W_icode = icode;
}

void r_Write::setW_valM(int valM)
{
    W_valM = valM;
}

void r_Write::setW_valE(int valE)
{
    W_valE = valE;
}
void r_Write::setW_dstE(int dstE)
{
    W_dstE = dstE;
}
void r_Write::setW_dstM(int dstM)
{
    W_dstM = dstM;
}

write::write()
{
    w_stat = 0;
    w_valE = 0;
    w_valM = 0;
}
void write::reset()
{
    w_stat = 0;
    w_valE = 0;
    w_valM = 0;

}
