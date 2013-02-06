#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turret.h"
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
}

MainWindow::~MainWindow()
{
    delete ui;
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





void MainWindow::analyzeimage()
{
    QString currentcolor=ui->colorchanger->currentText();
    int hmin,hmax,vmin=100,vmax=255,smin=100,smax=255;
    if(currentcolor=="Red")
    {
        hmin=-10;
        hmax=10;
    }
    else if(currentcolor=="Green")
    {
        hmin=110;
        hmax=130;
    }
    else if(currentcolor=="Blue")
    {
        hmin=230;
        hmax=250;
    }




    QString loadfile=QFileDialog::getOpenFileName(this,"","","Image Files (*.jpg *.bmp *.png)");
    QImage image(loadfile);
    image=image.convertToFormat(QImage::Format_Indexed8);
    for (int i=0;i<image.colorCount();i++)
    {
        QRgb rgb=image.color(i);
            QColor color(rgb);
            int h,s,v;
            color.getHsv(&h,&s,&v);
            if(h>=hmin&&h<=hmax&&s<=smax&&s>=smin&&v<=vmax&&v>=vmin)
            {
                s=255;
                v=0;
            }
            else
            {
                s=0;
                v=255;
            }
            color.setHsv(h,s,v);
            image.setColor(i,color.rgb());
    }
    QPixmap pixmapper;
    pixmapper.convertFromImage(image);
    ui->videostreamviewer->setPixmap(pixmapper);
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
    if(turr->move(FIRE,4)!=0)
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
    analyzeimage();
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
