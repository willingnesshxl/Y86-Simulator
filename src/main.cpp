#include <QApplication>
#include<QMainWindow>
#include<QtGui>
#include <ui_Y86.h>
#include<Y86.h>
#include<time.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_Y86 *y86 = new  Q_Y86 ;
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/fuckoffbitch.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Hi~~~~"), topRight, Qt::white);
    clock_t goal, wait;
    wait = (clock_t)3 * CLK_TCK;
    goal = wait + clock();
    while (goal > clock());
    y86->show();
    splash->finish(y86);
    delete splash;
    return app.exec();
}
