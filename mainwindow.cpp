#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turret.h"
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
        //disablebuttons();
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




//function to analyze an image. may be moved into a thread at a later time
void MainWindow::analyzeimage()
{
    CvCapture* capture=0;
    capture = cvCaptureFromCAM(0);
    if(!capture)
    {
        QMessageBox::critical(this,"Error","Unable to connect to camera");
        return;
    }
    IplImage* frame=0;
    QImage* tmpimage;
    while(true)
    {
        QApplication::processEvents();
        frame=cvQueryFrame(capture);
        if(!frame)
            break;
        IplImage* imgYellowThresh=GetThresholdedImage(frame);
        tmpimage=IplImage2QImage(frame);
        QPixmap tmppix;
        tmppix=QPixmap::fromImage(*tmpimage);
        ui->videostreamviewer->setPixmap(tmppix);
        int c=cvWaitKey(10);
        if(c!=-1)
        {
            break;
        }
    }


}





QImage*  MainWindow::IplImage2QImage(IplImage *iplImg)
 {
 int h = iplImg->height;
 int w = iplImg->width;
 int channels = iplImg->nChannels;
 QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
 char *data = iplImg->imageData;

for (int y = 0; y < h; y++, data += iplImg->widthStep)
 {
 for (int x = 0; x < w; x++)
 {
 char r, g, b, a = 0;
 if (channels == 1)
 {
 r = data[x * channels];
 g = data[x * channels];
 b = data[x * channels];
 }
 else if (channels == 3 || channels == 4)
 {
 r = data[x * channels + 2];
 g = data[x * channels + 1];
 b = data[x * channels];
 }

if (channels == 4)
 {
 a = data[x * channels + 3];
 qimg->setPixel(x, y, qRgba(r, g, b, a));
 }
 else
 {
 qimg->setPixel(x, y, qRgb(r, g, b));
 }
 }
 }
 return qimg;

}





IplImage* MainWindow::GetThresholdedImage(IplImage* img)
{
    IplImage* imgHSV=cvCreateImage(cvGetSize(img),8,3);
    cvCvtColor(img,imgHSV,CV_BGR2HSV);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img),8,1);
    cvInRangeS(imgHSV,cvScalar(20,100,100),cvScalar(30,255,255),imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
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
