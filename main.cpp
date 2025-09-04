#include "mainwindow.h"

#include <QApplication>
#include <QRandomGenerator>
#include <QDateTime>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 程序启动时调用一次
    //QRandomGenerator::global()->seed(QDateTime::currentMSecsSinceEpoch());
    MainWindow w;
    w.show();
    return a.exec();
}
