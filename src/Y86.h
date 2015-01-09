#ifndef Y86_H
#define Y86_H
#include"Pipeline.h"
#include<QMainWindow>
#include <ui_Y86.h>
#include <Output.h>
#include<vector>
#include "magnifierwidget.h"
#include <QApplication>
using namespace namespace_wslibs::namespace_wsbaswidgets;


class Q_Y86 : public QMainWindow, public Ui::Y86
{
    Q_OBJECT


public:

    Q_Y86();
public slots:

    void open();
    void nextstep();
    void run();
    void runstep();
    void reset();
    void stop();
    void saveAs();
    void about();
    void openMagnifier();
    void closeMagnifer();


private :
    r_Fetch F;
    fetch f;
    r_Decode D;
    decode d;
    r_Execute E;
    execute e;
    r_Memory M;
    memory m;
    r_Write W;
    write w;
    codeMemory Mem;
    Pipecontrol P_C;
    Register R;
    QString F_predPC, D_stat, D_icode, D_ifun, D_rA, D_rB, D_valC, D_valP, E_stat, E_icode, E_ifun, E_valC,
            E_valA, E_valB, E_dstE, E_dstM, E_srcA, E_srcB, M_stat, M_icode, M_Cnd, M_valE, M_valA, M_dstE, M_dstM,
            W_stat, W_icode, W_valE, W_valM, W_dstE, W_dstM;
    QTimer *mTimer;
    QString fileName;
    Output output;
    vector<Output> Result;
    MagnifierWidget widget;
    int flag;
    void showMemory();
    void showRegister();
    void showCC();
    void updateLabel();
    void setAssemble(int length);
    void writeFile(const QString &fileName);


    struct AssemblyCode
    {
        int address;
        QString assemble;
        int instruction;

    };
    vector<AssemblyCode> Acode;
    void updateCode();
    void resetCode();
    vector<int> jcAddress;

    void setjcCode();



};














#endif // Y86_H
