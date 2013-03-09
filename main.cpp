#include "mainwindow.h"
#include <QApplication>
// just sets up the gui and starts interface loop
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
