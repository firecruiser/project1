#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turret.h"
#include "readimages.h"
#include "calibrationwindow.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "BlobResult.h"
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/MainWindowIcon.png"));
    pi=3.14159;
    alpha=57;
    alphav=43;

    QSettings* targetsettings=new QSettings("targetsettings.ini",QSettings::IniFormat);
    targetsettings->beginGroup("target");
    h=targetsettings->value("h",10).toDouble();
    offset=targetsettings->value("offset",-.66).toDouble(); //in feet!!!!!!
    lowerH1=targetsettings->value("lowerH1",0).toInt();
    lowerH2=targetsettings->value("lowerH2",150).toInt();
    upperH1=targetsettings->value("upperH1",20).toInt();
    upperH2=targetsettings->value("upperH2",180).toInt();
    lowerS1=targetsettings->value("lowerS1",175).toInt();
    lowerS2=targetsettings->value("lowerS2",175).toInt();
    upperS1=targetsettings->value("upperS1",256).toInt();
    upperS2=targetsettings->value("upperS2",256).toInt();
    lowerV1=targetsettings->value("lowerV1",81).toInt();
    lowerV2=targetsettings->value("lowerV2",81).toInt();
    upperV1=targetsettings->value("upperV1",133).toInt();
    upperV2=targetsettings->value("upperV2",133).toInt();
    targetsettings->endGroup();
    delete targetsettings;





    turr=new Turret();
    on_actionConnect_to_Turret_triggered();
    ui->stopButton->hide();
    ui->pauseButton->hide();
    ui->firebutton->setEnabled(false);
    ui->firebutton->setStyleSheet("Background: blue");
    thread=new QThread;
    on_actionConnect_to_Camera_triggered();
    connect(&timeshooting,SIGNAL(timeout()),this,SLOT(updaterunningtime()));

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
    ui->timeBox->setEnabled(true);
}





void MainWindow::disablebuttons()
{
    ui->controlbox->setEnabled(false);
    ui->shotbox->setEnabled(false);
    ui->autobox->setEnabled(false);
    ui->timeBox->setEnabled(false);
}


void MainWindow::setupautobuttons()
{
    ui->controlbox->setEnabled(false);
    ui->reloadbutton->setEnabled(false);
    ui->startstopbutton->hide();
    ui->stopButton->show();
    ui->pauseButton->show();

}

void MainWindow::restoredefaultbuttons()
{
    ui->controlbox->setEnabled(true);
    ui->reloadbutton->setEnabled(true);
    ui->startstopbutton->show();
    ui->stopButton->hide();
    ui->pauseButton->hide();

}


void MainWindow::on_stopButton_clicked()
{
    emit(stopshooting());
    shootingstopped=true;
    restoredefaultbuttons();
}

void MainWindow::on_pauseButton_clicked()
{
    timeshooting.stop();
    emit(pauseshooting());

}

void MainWindow::shootingresumed()
{
    timeshooting.start(100);
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
    if(turr->currentmissilecount()<=0)
    {
        ui->firebutton->setEnabled(false);
        ui->firebutton->setStyleSheet("Background: blue");
    }
}

void MainWindow::on_reloadbutton_clicked()
{
    on_actionReload_triggered();
}

void MainWindow::on_startstopbutton_clicked()
{
    shootingstopped=false;
    QImage* currimage=getQImage();
    n=currimage->width();
    k=currimage->height();
    IplImage* curriplimage=Qimage2IplImage(&currimage->convertToFormat(QImage::Format_RGB32));
    IplImage* threshedimage=threshimage(curriplimage);
    CBlobResult blobs;
    CBlob* currentblob;
    blobs=CBlobResult(threshedimage,NULL,0);
    blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 150 );
    int j=blobs.GetNumBlobs();
    if(j==0)
    {
        QMessageBox::information(this,"No Targets","No Targets Found!");
        cvReleaseImage(&threshedimage);
        cvReleaseImage(&curriplimage);
        return;
    }
    CBlobGetXCenter XCenter;
    CBlobGetYCenter YCenter;
    for(int i=0;i<blobs.GetNumBlobs();i++)
    {
      tmptargetcenter=new targetcenter;
      currentblob=blobs.GetBlob(i);
      tmptargetcenter->x=XCenter(*currentblob);
      tmptargetcenter->y=YCenter(*currentblob);
      getangles(tmptargetcenter);
      targets.append(tmptargetcenter);
    }
    checkformissiles();
    ui->targetcountdisplay->display(targets.size());
    setupautobuttons();
    qApp->processEvents();
    ui->timeNumber->display(0);
    timeshooting.start(100);
    turr->initAngle();
    if(shootingstopped)
    {
        timeshooting.stop();
        targets.clear();
        return;
    }
    foreach(targetcenter* target,targets)
    {
        checkformissiles();
        qApp->processEvents();
        turr->setAngle(target->beta,target->betav);
        ui->shotcountdisplay->display(turr->currentmissilecount());
        if(shootingstopped)
        {
            timeshooting.stop();
            targets.clear();
            delete target;
            return;
        }
        ui->targetcountdisplay->display(ui->targetcountdisplay->value()-1);
        qApp->processEvents();
        delete target;
    }
    targets.clear();
    cvReleaseImage(&threshedimage);
    cvReleaseImage(&curriplimage);
    restoredefaultbuttons();
    timeshooting.stop();
    if(turr->currentmissilecount()<=0)
    {
        ui->firebutton->setEnabled(false);
        ui->firebutton->setStyleSheet("Background: blue");
    }


}

void MainWindow::getangles(targetcenter* target)
{
    viewwidth=2*h*tan((alpha/2)*pi/180);
    viewheight=2*h*tan((alphav/2)*pi/180);
    x_actual=target->x/n*viewwidth;
    y_actual=target->y/k*viewheight;
    x_location_launcher=-(viewwidth/2+offset-x_actual);
    y_location_launcher=viewheight/2-y_actual;
    beta=atan(x_location_launcher/h);
    betav=atan(y_location_launcher/h);
    beta=beta*180/pi;
    betav=betav*180/pi;
    target->beta=beta;
    target->betav=betav;


}



IplImage* MainWindow::threshimage(IplImage* img)
{

    IplImage* imgHSV=cvCreateImage(cvGetSize(img),8,3);
    cvCvtColor(img,imgHSV,CV_BGR2HSV);
    IplImage* tmp1=cvCreateImage(cvGetSize(img),8,1);
    IplImage* tmp2=cvCreateImage(cvGetSize(img),8,1);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img),8,1);
    cvInRangeS(imgHSV,cvScalar(lowerH1,lowerS1,lowerV1),cvScalar(upperH1,upperS1,upperV1),tmp1);
    cvInRangeS(imgHSV,cvScalar(lowerH2,lowerS2,lowerV2),cvScalar(upperH2,upperS2,upperV2),tmp2);
    cvOr(tmp1,tmp2,imgThreshed);
    cvReleaseImage(&imgHSV);
    cvReleaseImage(&tmp1);
    cvReleaseImage(&tmp2);
    return imgThreshed;
}



QImage* MainWindow::getQImage()
{
    const QPixmap *tmppix=ui->videostreamviewer->pixmap();
    QPixmap actualpix=*tmppix;
    QImage* image=&actualpix.toImage();
    return image;
}

IplImage* MainWindow::Qimage2IplImage(QImage* imagein)
{
    IplImage *imgHeader = cvCreateImageHeader( cvSize(imagein->width(), imagein->height()), IPL_DEPTH_8U, 4);
     imgHeader->imageData = (char*) imagein->bits();

    uchar* newdata = (uchar*) malloc(sizeof(uchar) * imagein->byteCount());
     memcpy(newdata, imagein->bits(), imagein->byteCount());
     imgHeader->imageData = (char*) newdata;
     return imgHeader;

}







void MainWindow::on_actionReload_triggered()
{
    QMessageBox::information(this,"Reload","Reload launcher and press ok when finished");
    turr->resetmissilecount();
    ui->shotcountdisplay->display(turr->currentmissilecount());
    ui->firebutton->setEnabled(true);
    ui->firebutton->setStyleSheet("Background: red");
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
        //QMessageBox::information(this,"Success","Connection to turret established");
        enablebuttons();
        connect(this,SIGNAL(pauseshooting()),turr,SLOT(pauseshooting()));
        connect(this,SIGNAL(stopshooting()),turr,SLOT(stopshooting()));
        connect(turr,SIGNAL(resume()),this,SLOT(shootingresumed()));
    }
}

void MainWindow::on_actionConnect_to_Camera_triggered()
{
    if(thread->isRunning())
    {
        QMessageBox::information(this,"Connected","Already connected to Kinect");
        return;
    }
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

void MainWindow::errorstring(QString err)
{
    QMessageBox::critical(this,"Error",err);
}

void MainWindow::updatepic(QPixmap newpicture)
{
    ui->videostreamviewer->setPixmap(newpicture);
}

void MainWindow::checkformissiles()
{
    if(turr->currentmissilecount()<=0)
    {
        timeshooting.stop();
        QMessageBox::critical(this,"Reload","Please Reload Turret \n Press Ok to Continue");
        turr->resetmissilecount();
        ui->firebutton->setEnabled(true);
        ui->firebutton->setStyleSheet("Background: red");
        timeshooting.start(100);
    }
    ui->shotcountdisplay->display(turr->currentmissilecount());
}

void MainWindow::on_actionCalibrate_triggered()
{
    QVector<int> tmpvectorint;
    QVector<double> tmpvectordouble;
    tmpvectorint<<lowerH1<<lowerS1<<lowerV1<<upperH1<<upperS1<<upperV1<<lowerH2<<lowerS2<<lowerV2<<upperH2<<upperS2<<upperV2;
    tmpvectordouble<<h<<offset;
    CalibrationWindow* cal=new CalibrationWindow(this,turr,&tmpvectorint,&tmpvectordouble,Qimage2IplImage(&getQImage()->convertToFormat(QImage::Format_RGB32)));
    cal->setModal(true);
    if(cal->exec()==QDialog::Accepted)
    {
        lowerH1=tmpvectorint.at(0);
        lowerS1=tmpvectorint.at(1);
        lowerV1=tmpvectorint.at(2);
        upperH1=tmpvectorint.at(3);
        upperS1=tmpvectorint.at(4);
        upperV1=tmpvectorint.at(5);
        lowerH2=tmpvectorint.at(6);
        lowerS2=tmpvectorint.at(7);
        lowerV2=tmpvectorint.at(8);
        upperH2=tmpvectorint.at(9);
        upperS2=tmpvectorint.at(10);
        upperV2=tmpvectorint.at(11);
        h=tmpvectordouble.at(0);
        offset=tmpvectordouble.at(1);
    }
    delete cal;
}

void MainWindow::updaterunningtime()
{
    ui->timeNumber->display(ui->timeNumber->value()+0.1);
}
