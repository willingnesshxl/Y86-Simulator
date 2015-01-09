#ifndef OUTPUT_H
#define OUTPUT_H
#include <QtGui>
#include<iostream>
#include<string>
#include<fstream>
#include<time.h>
#include"Pipeline.h"
using namespace std;
class Output
{
public:
    QString F_predPC,
            D_icode,
            D_ifun,
            D_rA,
            D_rB,
            D_valC,
            D_valP,
            E_icode,
            E_ifun,
            E_valC,
            E_valA,
            E_valB,
            E_dstE,
            E_dstM,
            E_srcA,
            E_srcB,
            M_icode,
            M_Bch,
            M_valE,
            M_valA,
            M_dstE,
            M_dstM,
            W_icode,
            W_valE,
            W_valM,
            W_dstE,
            W_dstM;
    void setOutput(r_Fetch F, r_Decode D, r_Execute E, r_Memory M, r_Write W)
    {
        QString temp;
        F_predPC = "0x00000" + temp.number(F.getF_predPC(), 16);
        D_icode = temp.number(D.getD_icode(), 16);
        D_ifun = temp.number(D.getD_ifun(), 16);
        D_rA = temp.number(D.getD_rA());
        D_rB = temp.number(D.getD_rB());
        if (D.getD_valC() < 16)
            D_valC = "0x0000000" + temp.number(D.getD_valC(), 16);
        else if (D.getD_valC() < 0x100)
            D_valC = "0x000000" + temp.number(D.getD_valC(), 16);
        else
            D_valC = "0x00000" + temp.number(D.getD_valC(), 16);
        if (D.getD_valP() < 16)
            D_valP = "0x0000000" + temp.number(D.getD_valP(), 16);
        else if (D.getD_valP() < 0x100)
            D_valP = "0x000000" + temp.number(D.getD_valP(), 16);
        else
            D_valP = "0x00000" + temp.number(D.getD_valP(), 16);
        E_icode = temp.number(E.getE_icode(), 16);
        E_ifun = temp.number(E.getE_ifun(), 16);
        if (E.getE_valC() < 16)
            E_valC = "0x0000000" + temp.number(E.getE_valC(), 16);
        else if (E.getE_valC() < 0x100)
            E_valC = "0x000000" + temp.number(E.getE_valC(), 16);
        else
            E_valC = "0x00000" + temp.number(E.getE_valC(), 16);
        if (E.getE_valA() < 16)
            E_valA = "0x0000000" + temp.number(E.getE_valA(), 16);
        else if (E.getE_valA() < 0x100)
            E_valA = "0x000000" + temp.number(E.getE_valA(), 16);
        else
            E_valA = "0x00000" + temp.number(E.getE_valA(), 16);

        if (E.getE_valB() < 16)
            E_valB = "0x0000000" + temp.number(E.getE_valB(), 16);
        else if (E.getE_valB() < 0x100)
            E_valB = "0x000000" + temp.number(E.getE_valB(), 16);
        else
            E_valB = "0x00000" + temp.number(E.getE_valB(), 16);
        E_dstE = temp.number(E.getE_dstE(), 16);
        E_dstM = temp.number(E.getE_dstM(), 16);
        E_srcA = temp.number(E.getE_srcA(), 16);
        E_srcB = temp.number(E.getd_srcB(), 16);
        M_icode = temp.number(M.getM_icode(), 16);
        M_Bch = temp.number(M.getM_Cnd(), 16);
        if (M.getM_valE() < 16)
            M_valE = "0x0000000" + temp.number(M.getM_valE(), 16);
        else if (M.getM_valE() < 0x100)
            M_valE = "0x000000" + temp.number(M.getM_valE(), 16);
        else
            M_valE = "0x00000" + temp.number(M.getM_valE(), 16);
        if (M.getM_valA() < 16)
            M_valA = "0x0000000" + temp.number(M.getM_valA(), 16);
        else if (M.getM_valA() < 0x100)
            M_valA = "0x000000" + temp.number(M.getM_valA(), 16);
        else
            M_valA = "0x00000" + temp.number(M.getM_valA(), 16);
        M_dstE = temp.number(M.getM_dstE(), 16);
        M_dstM = temp.number(M.getM_dstM(), 16);
        W_icode = temp.number(W.getW_icode(), 16);
        if (W.getW_valE() < 16)
            W_valE = "0x0000000" + temp.number(W.getW_valE(), 16);
        else if (W.getW_valE() < 0x100)
            W_valE = "0x000000" + temp.number(W.getW_valE(), 16);
        else
            W_valE = "0x00000" + temp.number(W.getW_valE(), 16);
        if (W.getW_valM() < 16)
            W_valM = "0x0000000" + temp.number(W.getW_valM(), 16);
        else if (W.getW_valM() < 0x100)
            W_valM = "0x000000" + temp.number(W.getW_valM(), 16);
        else
            W_valM = "0x00000" + temp.number(W.getW_valM(), 16);
        W_dstE = temp.number(W.getW_dstE(), 16);
        W_dstM = temp.number(W.getW_dstM(), 16);

    }
};

#endif // OUTPUT_H
