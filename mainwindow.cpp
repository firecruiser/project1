#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turret.h"
#include "readimages.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/MainWindowIcon.png"));
    turr=new Turret();
    if(turr->init()!=0)
    {
        QMessageBox::critical(this,"Error","Unable to connect to turret! Check if turret is connected and try again!");
        disablebuttons();
    }
    ui->firebutton->setEnabled(false);
    ui->firebutton->setStyleSheet("Background: blue");
    thread=new QThread;
    readimg=new ReadImages();
    connect(readimg,SIGNAL(error(QString)),this,SLOT(errorstring(QString)));
    if(readimg->initialize()==0)
    {
    readimg->moveToThread(thread);
    connect(thread,SIGNAL(started()),readimg,SLOT(startimages()));
    connect(readimg,SIGNAL(finished()),thread,SLOT(quit()));
    connect(readimg,SIGNAL(finished()),readimg,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(readimg,SIGNAL(picready(QPixmap)),this,SLOT(updatepic(QPixmap)));
    connect(this,SIGNAL(stopVideo()),readimg,SLOT(endimages()));
    thread->start();
    }
    else
    {
        ui->videostreamviewer->setText("No Camera Attatched \n Please connect a Camera and attempt to Reconnect!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit stopVideo();
    while(thread->isRunning())
    {
        this->hide();
        QApplication::processEvents();
    }
    event->accept();
}




void MainWindow::senderror()
{
    QMessageBox::critical(this,"Error","Unable to send to turret");
}




void MainWindow::enablebuttons()
{
    ui->controlbox->setEnabled(true);
    ui->shotbox->setEnabled(true);
    ui->autobox->setEnabled(true);
    ui->calibratebutton->setEnabled(true);
}





void MainWindow::disablebuttons()
{
    ui->controlbox->setEnabled(false);
    ui->shotbox->setEnabled(false);
    ui->autobox->setEnabled(false);
    ui->calibratebutton->setEnabled(false);
}






void MainWindow::on_upbutton_pressed()
{
    if(turr->move(UP,0)!=0)
    {
        senderror();
    }

}

void MainWindow::on_upbutton_released()
{
    if(turr->move(STOP,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_rightbutton_pressed()
{
    if(turr->move(RIGHT,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_rightbutton_released()
{
    if(turr->move(STOP,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_leftbutton_pressed()
{
    if(turr->move(LEFT,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_leftbutton_released()
{
    if(turr->move(STOP,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_downbutton_pressed()
{
    if(turr->move(DOWN,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_downbutton_released()
{
    if(turr->move(STOP,0)!=0)
    {
        senderror();
    }
}

void MainWindow::on_firebutton_clicked()
{
    if(turr->move(FIRE,4.5)!=0)
    {
        senderror();
    }
    ui->shotcountdisplay->display(turr->currentmissilecount());
    if(turr->currentmissilecount()==0)
    {
        ui->firebutton->setEnabled(false);
        ui->firebutton->setStyleSheet("Background: blue");
        //QMessageBox::information(this,"Reload","Please press reload to continue firing!");
    }
}

void MainWindow::on_reloadbutton_clicked()
{
    on_actionReload_triggered();
}

void MainWindow::on_startstopbutton_clicked()
{

}

void MainWindow::on_calibratebutton_clicked()
{
    on_actionCalibrate_triggered();
}












void MainWindow::on_actionReload_triggered()
{
    QMessageBox::information(this,"Reload","Reload launcher and press ok when finished");
    turr->resetmissilecount();
    ui->shotcountdisplay->display(turr->currentmissilecount());
    ui->firebutton->setEnabled(true);
    ui->firebutton->setStyleSheet("Background: red");
}

void MainWindow::on_actionCalibrate_triggered()
{

}


void MainWindow::on_actionConnect_to_Turret_triggered()
{
    if(turr->init()!=0)
    {
        QMessageBox::critical(this,"Error","Unable to connect to turret! Check if turret is connected and try again!");
        disablebuttons();
    }
    else
    {
        QMessageBox::information(this,"Success","Connection to turret established");
        enablebuttons();
    }
}

void MainWindow::on_actionConnect_to_Camera_triggered()
{
    if(readimg->initialize()==0)
    {
    readimg->moveToThread(thread);
    connect(thread,SIGNAL(started()),readimg,SLOT(startimages()));
    connect(readimg,SIGNAL(finished()),thread,SLOT(quit()));
    connect(readimg,SIGNAL(finished()),readimg,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(readimg,SIGNAL(picready(QPixmap)),this,SLOT(updatepic(QPixmap)));
    connect(this,SIGNAL(stopVideo()),readimg,SLOT(endimages()));
    thread->start();
    }
    else
    {
        ui->videostreamviewer->setText("No Camera Attatched \n Please connect a Camera and attempt to Reconnect!");
    }
}

void MainWindow::errorstring(QString err)
{
    QMessageBox::critical(this,"Error",err);
}

void MainWindow::updatepic(QPixmap newpicture)
{
    ui->videostreamviewer->setPixmap(newpicture);
}
