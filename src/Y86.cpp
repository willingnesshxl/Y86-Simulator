#include "Y86.h"
#include "ui_Y86.h"
#include <QtGui>
#include<iostream>
#include<string>
#include<fstream>
#include<time.h>
#include<list>
#include <QApplication>
#include "magnifierwidget.h"
using namespace std;
Q_Y86::Q_Y86()
{


    Ui::Y86::setupUi(this);
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(open()));
    connect(Step, SIGNAL(clicked()), this, SLOT(nextstep()));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(Run, SIGNAL(clicked()), this, SLOT(run()));
    connect(Reset, SIGNAL(clicked()), this, SLOT(reset()));
    connect(Stop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(actionAbout_me, SIGNAL(triggered()), this, SLOT(about()));
    connect(actionAbout_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionOpen_Magnifier, SIGNAL(triggered()), this, SLOT(openMagnifier()));
    connect(actionClose_Magnifier_2, SIGNAL(triggered()), this, SLOT(closeMagnifer()));
    Stop->setDisabled(true);
    showRegister();
    showMemory();
    showCC();
    Run->setDisabled(true);
    Step->setDisabled(true);
    Reset->setDisabled(true);
    actionSave->setDisabled(true);
    flag = 0;

}
void Q_Y86::writeFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("Y86"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (unsigned int i = 0; i < Result.size() - 1; ++i)
    {
        out << "Circle_" << i + 1 << "\r\n" << "--------------------" << "\r\n" << "FETCH:" << "\r\n";
        out << "        F_predPC = " << Result[i].F_predPC << "\r\n" << "\r\n";
        out << "DECODE:" << "\r\n";
        out << "        D_icode  = " << Result[i].D_icode << "\r\n";
        out << "        D_ifun   = " << Result[i].D_ifun << "\r\n";
        out << "        D_rA     = " << Result[i].D_rA << "\r\n";
        out << "        D_rB     = " << Result[i].D_rB << "\r\n";
        out << "        D_valC   = " << Result[i].D_valC << "\r\n";
        out << "        D_valP   = " << Result[i].D_valP << "\r\n" << "\r\n";
        out << "EXECUTE:" << "\r\n";
        out << "        E_icode  = " << Result[i].E_icode << "\r\n";
        out << "        E_ifun   = " << Result[i].E_ifun << "\r\n";
        out << "        E_valC   = " << Result[i].E_valC << "\r\n";
        out << "        E_valA   = " << Result[i].E_valA << "\r\n";
        out << "        E_valB   = " << Result[i].E_valB << "\r\n";
        out << "        E_dstE   = " << Result[i].E_dstE << "\r\n";
        out << "        E_dstM   = " << Result[i].E_dstM << "\r\n";
        out << "        E_srcA   = " << Result[i].E_srcA << "\r\n";
        out << "        E_srcB   = " << Result[i].E_srcB << "\r\n" << "\r\n";
        out << "MEMORY:" << "\r\n";
        out << "        M_icode  = " << Result[i].M_icode << "\r\n";
        out << "        M_Bch  = " << Result[i].M_Bch << "\r\n";
        out << "        M_valE  = " << Result[i].M_valE << "\r\n";
        out << "        M_valA  = " << Result[i].M_valA << "\r\n";
        out << "        M_dstE   = " << Result[i].M_dstE << "\r\n";
        out << "        M_dstM   = " << Result[i].M_dstM << "\r\n" << "\r\n";
        out << "WRITE BACK:" << "\r\n";
        out << "        W_icode  = " << Result[i].W_icode << "\r\n";
        out << "        W_valE  = " << Result[i].W_valE << "\r\n";
        out << "        W_valM  = " << Result[i].W_valM << "\r\n";
        out << "        W_dstE   = " << Result[i].W_dstE << "\r\n";
        out << "        W_dstM   = " << Result[i].W_dstM << "\r\n" << "\r\n";





    }
    QApplication::restoreOverrideCursor();
    return ;

}


void Q_Y86::saveAs()
{
    QString resultName = QFileDialog::getSaveFileName(this, tr("Save the Result"), ".", tr("txt files(*.txt)"));
    if (resultName.isEmpty())
        return;
    writeFile(resultName);
}

void Q_Y86::open()
{
    fileName = QFileDialog::getOpenFileName(this);
    char *temp = new char[fileName.length()];
    unsigned int j;
    string s = fileName.toStdString();
    for (j = 0; j < s.length(); j++)
        temp[j] = s[j];
    temp[j] = '\0';
    int length = Mem.setMemory(temp);
    if (length != 0x400)
    {
        Run->setDisabled(false);
        Step->setDisabled(false);
        Reset->setDisabled(false);
    }
    R.reset();
    F.reset();
    D.reset();
    E.reset();
    M.reset();
    W.reset();
    f.reset();
    d.reset();
    e.reset();
    m.reset();
    w.reset();
    F.setF_predPC(0x400);
    R.setRegister(4, 0x400);
    R.setRegister(5, 0x400);
    Result.clear();
    Acode.clear();
    setAssemble(length);
    showRegister();
    showMemory();
    showCC();
    updateLabel();

}

void Q_Y86::showRegister()
{
    RegisterView->clear();
    for (int i = 0; i < 8; i++)
    {
        QString temp;
        temp = "0x" + temp.number(R.getRegister(i), 16);
        switch (i)
        {
        case 0:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/eax.png"), temp));
            break;
        case 1:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/ecx.png"), temp));
            break;
        case 2:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/edx.png"), temp));
            break;
        case 3:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/ebx.png"), temp));
            break;
        case 4:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/esp.png"), temp));
            break;
        case 5:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/ebp.png"), temp));
            break;
        case 6:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/esi.png"), temp));
            break;
        case 7:
            RegisterView->addItem(new QListWidgetItem(QIcon(":/images/edi.png"), temp));
            break;
        }
    }
}

void Q_Y86::showMemory()
{
    MemoryView->clear();
    for (int i = 0; i < 0x800; i += 4)
    {
        QString temp1, temp2, temp4;
        unsigned char temp3;
        for (int j = i; j < i + 4; j++)
        {
            if (j == i)
            {
                Mem.Read_Byte_Memory(j, &temp3);
                if (temp3 < 16)
                    temp1 = temp1 + "0" + temp2.number(temp3, 16);
                else
                    temp1 = temp1 + temp2.number(temp3, 16);


            }
            else
            {
                Mem.Read_Byte_Memory(j, &temp3);
                if (temp3 < 16)
                    temp1 = temp1 + " 0" + temp2.number(temp3, 16);
                else
                    temp1 = temp1 + " " + temp2.number(temp3, 16);
            }
        }
        if (i < 16)
            temp4 = "0x00" + temp2.number(i, 16);
        else if (i < 0X100)
            temp4 = "0x0" + temp2.number(i, 16);
        else
            temp4 = "0x" + temp2.number(i, 16);
        temp1 = temp4 + "  " + temp1;
        MemoryView->addItem(temp1);
    }

}

void Q_Y86::showCC()
{
    CCView->clear();
    if (e.gete_CC().getZF() == 0)
        CCView->addItem(new QListWidgetItem(QIcon(":/images/ZF.png"), "0"));
    else
        CCView->addItem(new QListWidgetItem(QIcon(":/images/ZF.png"), "1"));
    if (e.gete_CC().getSF() == 0)
        CCView->addItem(new QListWidgetItem(QIcon(":/images/SF.png"), "0"));
    else
        CCView->addItem(new QListWidgetItem(QIcon(":/images/SF.png"), "1"));
    if (e.gete_CC().getOF() == 0)
        CCView->addItem(new QListWidgetItem(QIcon(":/images/OF.png"), "0"));
    else
        CCView->addItem(new QListWidgetItem(QIcon(":/images/OF.png"), "1"));

}

void Q_Y86::nextstep()
{

    if (flag == 0)
    {
        flag = 1;
    }
    if (W.getW_icode() == 1)
    {
        Run->setDisabled(true);
        Step->setDisabled(true);
        actionSave->setDisabled(false);
        flag = 0;
        widget.close();

    }
    w.update(W, R);
    if (!m.update(M, Mem))
    {
        reset();
        QMessageBox::warning(this, tr("Warning"),
                             tr("Memory Error!"), QMessageBox::Ok);
        return;
    }
    e.update(E);
    d.update(D, R, e, m, E, M, W);
    if (!f.update(Mem, F, M, W, E))
    {
        reset();
        QMessageBox::warning(this, tr("Warning"),
                             tr("error!"), QMessageBox::Ok);
        return;
    }
    P_C.setControl(F, D, E, M, W, d, e);
    if (F.getstall() == 0)
        F.setF_predPC(f.getpredPC());
    if (D.getstall() == 0 && D.getbubble() == 0)
    {
        D.setD_icode(f.getf_icode());
        D.setD_ifun(f.getf_ifun());
        D.setD_rA(f.getf_rA());
        D.setD_rB(f.getf_rB());
        D.setD_valC(f.getf_valC());
        D.setD_valP(f.getf_valP());
        D.setPC(f.getPC());

    }
    if (E.getbubble() == 0)
    {
        E.setE_icode(d.getd_icode());
        E.setE_ifun(d.getd_ifun());
        E.setE_valC(d.getd_valC());
        E.setE_valA(d.getd_valA());
        E.setE_valB(d.getd_valB());
        E.setE_dstE(d.getd_dstE());
        E.setE_dstM(d.getd_dstM());
        E.setE_srcA(d.getd_srcA());
        E.setE_srcB(d.getd_srcB());
        E.setPC(d.getPC());
    }
    M.setM_icode(e.gete_icode());
    M.setM_Cnd(e.gete_Cnd());
    M.setM_valE(e.gete_valE());
    M.setM_valA(e.gete_valA());
    M.setM_dstE(e.gete_dstE());
    M.setM_dstM(e.gete_dstM());
    M.setPC(e.getPC());
    W.setW_icode(m.getm_icode());
    W.setW_valE(m.getm_valE());
    W.setW_valM(m.getm_valM());
    W.setW_dstE(m.getm_dstE());
    W.setW_dstM(m.getm_dstM());
    W.setPC(m.getPC());
    output.setOutput(F, D, E, M, W);
    Result.push_back(output);
    showRegister();
    showMemory();
    showCC();
    updateLabel();
    updateCode();


}

void Q_Y86::updateLabel()
{
    QString temp;
    F_predPC = "0x" + temp.number(F.getF_predPC(), 16);
    D_icode = temp.number(D.getD_icode(), 16);
    D_ifun = temp.number(D.getD_ifun(), 16);
    D_rA = temp.number(D.getD_rA(), 16);
    D_rB = temp.number(D.getD_rB(), 16);
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
    M_Cnd = temp.number(M.getM_Cnd(), 16);
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
    vpredPC->setText(F_predPC);
    vD_icode->setText(D_icode);
    vD_ifun->setText(D_ifun);
    vD_rA->setText(D_rA);
    vD_rB->setText(D_rB);
    vD_valC->setText(D_valC);
    vD_valP->setText(D_valP);
    vE_icode->setText(E_icode);
    vE_ifun->setText(E_ifun);
    vE_valC->setText(E_valC);
    vE_valA->setText(E_valA);
    vE_valB->setText(E_valB);
    vE_dstE->setText(E_dstE);
    vE_dstM->setText(E_dstM);
    vE_srcA->setText(E_srcA);
    vE_srcB->setText(E_srcB);
    vM_icode->setText(M_icode);
    vM_Cnd->setText(M_Cnd);
    vM_valA->setText(M_valA);
    vM_valE->setText(M_valE);
    vM_dstE->setText(M_dstE);
    vM_dstM->setText(M_dstM);
    vW_icode->setText(W_icode);
    vW_valE->setText(W_valE);
    vW_valM->setText(W_valM);
    vW_dstE->setText(W_dstE);
    vW_dstM->setText(W_dstM);
    if (F.getstall() == 1)
        vF_stat->setText("STALL");
    else
        vF_stat->clear();
    if (D.getstall() == 1)
        vD_stat->setText("STALL");
    else if (D.getbubble() == 1)
        vD_stat->setText("BUBBLE");
    else
        vD_stat->clear();

    if (E.getbubble() == 1)
        vE_stat->setText("BUBBLE");
    else
        vE_stat->clear();
}

void Q_Y86::runstep()
{

    if (flag == 0)
    {
        flag = 1;
    }
    if (W.getW_icode() == 1)
    {
        Run->setDisabled(true);
        Step->setDisabled(true);
        Reset->setDisabled(false);
        Stop->setDisabled(true);
        spinBox->setDisabled(false);
        slider->setDisabled(false);
        actionSave->setDisabled(false);
        actionLoad->setDisabled(false);
        disconnect(mTimer, SIGNAL(timeout()), this, SLOT(runstep()));
        mTimer = NULL;
        flag = 0;
        widget.close();
    }
    w.update(W, R);
    if (!m.update(M, Mem))
    {
        Run->setDisabled(false);
        Step->setDisabled(false);
        Reset->setDisabled(false);
        Stop->setDisabled(true);
        spinBox->setDisabled(false);
        slider->setDisabled(false);
        reset();
        disconnect(mTimer, SIGNAL(timeout()), this, SLOT(runstep()));
        mTimer = NULL;
        QMessageBox::warning(this, tr("Warning"),
                             tr("error!"), QMessageBox::Ok);
        return;
    }
    e.update(E);
    d.update(D, R, e, m, E, M, W);
    if (!f.update(Mem, F, M, W, E))
    {
        Run->setDisabled(false);
        Step->setDisabled(false);
        Reset->setDisabled(false);
        Stop->setDisabled(true);
        spinBox->setDisabled(false);
        slider->setDisabled(false);
        reset();
        disconnect(mTimer, SIGNAL(timeout()), this, SLOT(runstep()));
        mTimer = NULL;
        QMessageBox::warning(this, tr("Warning"),
                             tr("error!"), QMessageBox::Ok);
        return;
    }
    P_C.setControl(F, D, E, M, W, d, e);
    if (F.getstall() == 0)
        F.setF_predPC(f.getpredPC());
    if (D.getstall() == 0 && D.getbubble() == 0)
    {
        D.setD_icode(f.getf_icode());
        D.setD_ifun(f.getf_ifun());
        D.setD_rA(f.getf_rA());
        D.setD_rB(f.getf_rB());
        D.setD_valC(f.getf_valC());
        D.setD_valP(f.getf_valP());
        D.setPC(f.getPC());
    }
    if (E.getbubble() == 0)
    {
        E.setE_icode(d.getd_icode());
        E.setE_ifun(d.getd_ifun());
        E.setE_valC(d.getd_valC());
        E.setE_valA(d.getd_valA());
        E.setE_valB(d.getd_valB());
        E.setE_dstE(d.getd_dstE());
        E.setE_dstM(d.getd_dstM());
        E.setE_srcA(d.getd_srcA());
        E.setE_srcB(d.getd_srcB());
        E.setPC(d.getPC());
    }
    M.setM_icode(e.gete_icode());
    M.setM_Cnd(e.gete_Cnd());
    M.setM_valE(e.gete_valE());
    M.setM_valA(e.gete_valA());
    M.setM_dstE(e.gete_dstE());
    M.setM_dstM(e.gete_dstM());
    M.setPC(e.getPC());
    W.setW_icode(m.getm_icode());
    W.setW_valE(m.getm_valE());
    W.setW_valM(m.getm_valM());
    W.setW_dstE(m.getm_dstE());
    W.setW_dstM(m.getm_dstM());
    W.setPC(m.getPC());
    output.setOutput(F, D, E, M, W);
    Result.push_back(output);
    showRegister();
    showMemory();
    showCC();
    updateLabel();
    updateCode();



}

void Q_Y86::run()
{
    Run->setDisabled(true);
    Step->setDisabled(true);
    Reset->setDisabled(true);
    Stop->setDisabled(false);
    actionSave->setDisabled(true);
    actionLoad->setDisabled(true);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(runstep()));
    mTimer->start(2500 / spinBox->text().toInt());
    spinBox->setDisabled(true);
    slider->setDisabled(true);
    return;

}

void Q_Y86::reset()
{
    char *temp = new char[fileName.length()];
    unsigned int j;
    string s = fileName.toStdString();
    for (j = 0; j < s.length(); j++)
        temp[j] = s[j];
    temp[j] = '\0';
    if (Mem.setMemory(temp) != false)
    {
        Run->setDisabled(false);
        Step->setDisabled(false);
        Reset->setDisabled(false);
        spinBox->setDisabled(false);
        slider->setDisabled(false);
        actionSave->setDisabled(true);
        actionLoad->setDisabled(false);
    }
    R.reset();
    F.reset();
    D.reset();
    E.reset();
    M.reset();
    W.reset();
    f.reset();
    d.reset();
    e.reset();
    m.reset();
    w.reset();
    F.setF_predPC(0x400);
    F_predPC = "0x0";
    Result.clear();
    R.setRegister(4, 0x400);
    R.setRegister(5, 0x400);
    showRegister();
    showMemory();
    showCC();
    updateLabel();
    resetCode();
    widget.close();

}

void Q_Y86::stop()
{
    Run->setDisabled(false);
    Step->setDisabled(false);
    Reset->setDisabled(false);
    Stop->setDisabled(true);
    spinBox->setDisabled(false);
    slider->setDisabled(false);
    disconnect(mTimer, SIGNAL(timeout()), this, SLOT(runstep()));
    mTimer = NULL;
    reset();
    return;
}

void Q_Y86::setAssemble(int length)
{
    CodeView->clear();
    unsigned char temp_char;
    QString assemble, address, temp, regA, regB, code;
    jcAddress.clear();

    int pc = 0x400, addr;
    int f_icode, f_ifun, f_rA, f_rB, f_valP, f_valC;
    AssemblyCode Ctemp;
    while (pc < length)
    {
        addr = pc;
        if (!Mem.Read_Byte_Memory(pc, &temp_char))
            return;
        f_ifun = temp_char & 0xf;
        f_icode = (temp_char >> 4) & 0xf;
        pc++;
        int need_regids = 0;
        if (f_icode == IRRMOVL || f_icode == IOPL || f_icode == IPUSHL || f_icode == IPOPL || f_icode == IIRMOVL || f_icode == IRMMOVL || f_icode == IMRMOVL)
            need_regids = 1;
        if (need_regids == 1)
        {
            if (!Mem.Read_Byte_Memory(pc, &temp_char))
                return;
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
            if (!Mem.Read_Word_Memory(pc, &f_valC))
                return;

            pc += 4;

            f_valP = pc;

        }
        else
            f_valC = 0;
        switch (f_rA)
        {
        case 0:
            regA = "%eax";
            break;
        case 1:
            regA = "%ecx";
            break;
        case 2:
            regA = "%edx";
            break;
        case 3:
            regA = "%ebx";
            break;
        case 4:
            regA = "%esp";
            break;
        case 5:
            regA = "%ebp";
            break;
        case 6:
            regA = "%esi";
            break;
        case 7:
            regA = "%edi";
            break;
        default:
            regA = "none";
            break;

        }
        switch (f_rB)
        {
        case 0:
            regB = "%eax";
            break;
        case 1:
            regB = "%ecx";
            break;
        case 2:
            regB = "%edx";
            break;
        case 3:
            regB = "%ebx";
            break;
        case 4:
            regB = "%esp";
            break;
        case 5:
            regB = "%ebp";
            break;
        case 6:
            regB = "%esi";
            break;
        case 7:
            regB = "%edi";
            break;
        default:
            regB = "none";
            break;

        }

        switch (f_icode)
        {
        case 0:
            assemble = "nop";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 1:
            assemble = "halt";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 2:
            assemble = "rrmovl " + regA + " " + regB;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 3:
            assemble = "irmovl " + temp.number(f_valC, 16) + " " + regB;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 4:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "rmmovl " + regA + " " + address + "(" + regB + ")";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 5:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "mrmovl " + address + "(" + regB + ")" + " " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 6:
            switch (f_ifun)
            {
            case 0:
                assemble = "addl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 1:
                assemble = "subl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 2:
                assemble = "andl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 3:
                assemble = "xorl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;


            }
            break;
        case 7:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            switch (f_ifun)
            {
            case 0:
                assemble = "jmp " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 1:
                assemble = "jle " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 2:
                assemble = "jl " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 3:
                assemble = "je " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 4:
                assemble = "jne " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 5:
                assemble = "jge " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 6:
                assemble = "jg " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            }
            jcAddress.push_back(f_valC);
            break;

        case 8:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "call " + address;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            jcAddress.push_back(f_valC);
            break;
        case 9:
            assemble = "ret";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        case 10:
            assemble = "pushl " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        case 11:
            assemble = "popl " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        }
        Ctemp.address = addr;
        Ctemp.assemble = assemble;
        Acode.push_back(Ctemp);



    }
    setjcCode();
    for (unsigned int i = 0; i < Acode.size(); i++)
    {
        code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
        CodeView->addItem(code);
    }
}

void Q_Y86::setjcCode()
{

    unsigned char temp_char;
    QString assemble, address, temp, regA, regB, code;

    int pc, addr;
    int f_icode, f_ifun, f_rA, f_rB, f_valP, f_valC;
    AssemblyCode Ctemp;
    while (jcAddress.size() > 0)
    {
        cout << "xxx" << endl;
        pc = jcAddress[0];
        addr = pc;
        if (!Mem.Read_Byte_Memory(pc, &temp_char))
            return;
        f_ifun = temp_char & 0xf;
        f_icode = (temp_char >> 4) & 0xf;
        pc++;
        int need_regids = 0;
        if (f_icode == IRRMOVL || f_icode == IOPL || f_icode == IPUSHL || f_icode == IPOPL || f_icode == IIRMOVL || f_icode == IRMMOVL || f_icode == IMRMOVL)
            need_regids = 1;
        if (need_regids == 1)
        {
            if (!Mem.Read_Byte_Memory(pc, &temp_char))
                return;
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
            if (!Mem.Read_Word_Memory(pc, &f_valC))
                return;

            pc += 4;
            f_valP = pc;

        }
        else
            f_valC = 0;
        switch (f_rA)
        {
        case 0:
            regA = "%eax";
            break;
        case 1:
            regA = "%ecx";
            break;
        case 2:
            regA = "%edx";
            break;
        case 3:
            regA = "%ebx";
            break;
        case 4:
            regA = "%esp";
            break;
        case 5:
            regA = "%ebp";
            break;
        case 6:
            regA = "%esi";
            break;
        case 7:
            regA = "%edi";
            break;
        default:
            regA = "none";
            break;

        }
        switch (f_rB)
        {
        case 0:
            regB = "%eax";
            break;
        case 1:
            regB = "%ecx";
            break;
        case 2:
            regB = "%edx";
            break;
        case 3:
            regB = "%ebx";
            break;
        case 4:
            regB = "%esp";
            break;
        case 5:
            regB = "%ebp";
            break;
        case 6:
            regB = "%esi";
            break;
        case 7:
            regB = "%edi";
            break;
        default:
            regB = "none";
            break;

        }

        switch (f_icode)
        {
        case 0:
            assemble = "nop";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 1:
            assemble = "halt";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 2:
            assemble = "rrmovl " + regA + " " + regB;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 3:
            assemble = "irmovl " + temp.number(f_valC, 16) + " " + regB;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 4:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "rmmovl " + regA + " " + address + "(" + regB + ")";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 5:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "mrmovl " + address + "(" + regB + ")" + " " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;
        case 6:
            switch (f_ifun)
            {
            case 0:
                assemble = "addl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 1:
                assemble = "subl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 2:
                assemble = "andl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 3:
                assemble = "xorl " + regA + " " + regB;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;


            }
            break;
        case 7:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            switch (f_ifun)
            {
            case 0:
                assemble = "jmp " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 1:
                assemble = "jle " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 2:
                assemble = "jl " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 3:
                assemble = "je " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 4:
                assemble = "jne " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 5:
                assemble = "jge " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            case 6:
                assemble = "jg " + address;
                code = "0x00000" + temp.number(addr, 16) + " " + assemble;
                break;

            }
            jcAddress.push_back(f_valC);
            break;

        case 8:
            if (f_valC < 16)
                address = "0x0000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x100)
                address = "0x000000" + temp.number(f_valC, 16);
            else if (f_valC < 0x800)
                address = "0x00000" + temp.number(f_valC, 16);
            else
                address = temp.number(f_valC, 16);
            assemble = "call " + address;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            jcAddress.push_back(f_valC);
            break;
        case 9:
            assemble = "ret";
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        case 10:
            assemble = "pushl " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        case 11:
            assemble = "popl " + regA;
            code = "0x00000" + temp.number(addr, 16) + " " + assemble;
            break;

        }
        Ctemp.address = jcAddress[0];
        Ctemp.assemble = assemble;
        for (unsigned int i = 0; i < Acode.size() - 1; i++)
            if (jcAddress[0] > Acode[i].address && jcAddress[0] < Acode[i + 1].address)
            {
                Acode.insert(Acode.begin() + i + 1, Ctemp);
                cout << jcAddress[0] << endl;
            }
        if (jcAddress[0] > Acode[Acode.size() - 1].address)
            Acode.push_back(Ctemp);
        jcAddress.erase(jcAddress.begin());


    }
}

void Q_Y86::about()
{
    QMessageBox::about(this, tr("About Y86"),
                       tr("<h2>Y86 1.1</h2>"
                          "<p>Copyright &copy; 2012 WJF."
                          "<p>Y86 is a small application that "
                          "demonstrates 'A Programmer's Perspective of Computer Systems' "
                         ));
}

void Q_Y86::updateCode()
{
    QListWidgetItem *tempItem;
    for (unsigned int i = 0; i < Acode.size(); i++)
    {
        Acode[i].instruction = 0;
    }
    for (unsigned int i = 0; i < Acode.size(); i++)
    {
        if (Acode[i].address == F.getF_predPC())
            Acode[i].instruction = 1;
        if (Acode[i].address == D.getPC())
            Acode[i].instruction = 2;
        if (Acode[i].address == E.getPC())
            Acode[i].instruction = 3;
        if (Acode[i].address == M.getPC())
            Acode[i].instruction = 4;
        if (Acode[i].address == W.getPC())
            Acode[i].instruction = 5;
    }
    CodeView->clear();
    QString temp, code;
    for (unsigned int i = 0; i < Acode.size(); i++)
    {
        switch (Acode[i].instruction)
        {
        case 0:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            CodeView->addItem(new QListWidgetItem(QIcon(":/images/N.png"), code));
            break;
        case 1:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            tempItem = new QListWidgetItem(QIcon(":/images/F.png"), code);
            CodeView->addItem(tempItem);
            CodeView->setCurrentItem(tempItem);
            break;
        case 2:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            CodeView->addItem(new QListWidgetItem(QIcon(":/images/D.png"), code));
            break;
        case 3:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            CodeView->addItem(new QListWidgetItem(QIcon(":/images/E.png"), code));
            break;
        case 4:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            CodeView->addItem(new QListWidgetItem(QIcon(":/images/M.png"), code));
            break;
        case 5:
            code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
            CodeView->addItem(new QListWidgetItem(QIcon(":/images/W.png"), code));
            break;





        }

    }
}

void Q_Y86::resetCode()
{
    CodeView->clear();
    QString temp, code;
    for (unsigned int i = 0; i < Acode.size(); i++)
    {
        Acode[i].instruction = 0;
        code = "0x00000" + temp.number(Acode[i].address, 16) + " " + Acode[i].assemble;
        CodeView->addItem(code);
    }


}

void Q_Y86::openMagnifier()
{
    widget.show();
}

void Q_Y86::closeMagnifer()
{
    widget.close();
}


