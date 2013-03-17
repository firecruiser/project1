#include "calibrationwindow.h"
#include "ui_calibrationwindow.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>


// constructor for the calibration window which pulls all of the values from the incoming vectors and defines them in local values
CalibrationWindow::CalibrationWindow(QWidget *parent,Turret* turr,QVector<int>* incomingdataint,QVector<double>* incomingdatadouble,IplImage* tmpimagein) :
    QDialog(parent),
    ui(new Ui::CalibrationWindow)
{
    this->turr=turr;
    ints=incomingdataint;
    doubles=incomingdatadouble;
    testingimage=tmpimagein;
    threshedimage=cvCreateImage(cvGetSize(tmpimagein),8,1);
    ui->setupUi(this);
    lowerH1=incomingdataint->at(0);
    lowerS1=incomingdataint->at(1);
    lowerV1=incomingdataint->at(2);
    upperH1=incomingdataint->at(3);
    upperS1=incomingdataint->at(4);
    upperV1=incomingdataint->at(5);
    lowerH2=incomingdataint->at(6);
    lowerS2=incomingdataint->at(7);
    lowerV2=incomingdataint->at(8);
    upperH2=incomingdataint->at(9);
    upperS2=incomingdataint->at(10);
    upperV2=incomingdataint->at(11);
    h=incomingdatadouble->at(0);
    offset=incomingdatadouble->at(1);
    QVector<double> turretdata=turr->getparameters();
    xmovespeed=turretdata.at(0);
    ymovespeed=turretdata.at(1);
    xoffset=turretdata.at(2);
    yoffset=turretdata.at(3);
    ui->xmovespeed->setText(QString::number(xmovespeed));
    ui->ymovespeed->setText(QString::number(ymovespeed));
    ui->xoffset->setText(QString::number(xoffset));
    ui->yoffset->setText(QString::number(yoffset));
    ui->targdist->setText(QString::number(h));
    ui->turroffset->setText(QString::number(offset));
    ui->highendbutton->setChecked(true);
    highchecked=true;
    ui->lowHslider->setValue(lowerH2);
    ui->lowSslider->setValue(lowerS2);
    ui->lowVslider->setValue(lowerV2);
    ui->upperHslider->setValue(upperH2);
    ui->upperSslider->setValue(upperS2);
    ui->upperVslider->setValue(upperV2);
    QDoubleValidator* valid=new QDoubleValidator(this);
    valid->setNotation(QDoubleValidator::StandardNotation);
    ui->xmovespeed->setValidator(valid);
    ui->ymovespeed->setValidator(valid);
    ui->xoffset->setValidator(valid);
    ui->yoffset->setValidator(valid);
    ui->targdist->setValidator(valid);
    ui->turroffset->setValidator(valid);
    connect(ui->lowHslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    connect(ui->lowSslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    connect(ui->lowVslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    connect(ui->upperHslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    connect(ui->upperSslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    connect(ui->upperVslider,SIGNAL(sliderMoved(int)),this,SLOT(updateImage()));
    updateImage();
}

CalibrationWindow::~CalibrationWindow()
{
    delete ui;
}


// saves the current calibration to the file "targetsettings.ini" so the calibration may be loaded next run
void CalibrationWindow::on_saveaccept_clicked()
{
    QSettings* targetsettings=new QSettings(QApplication::applicationDirPath()+"/targetsettings.ini",QSettings::IniFormat);
    targetsettings->beginGroup("target");
    targetsettings->setValue("h",ui->targdist->text().toDouble());
    targetsettings->setValue("offset",ui->turroffset->text().toDouble()); //in feet!!!!!!
    targetsettings->setValue("lowerH1",lowerH1);
    targetsettings->setValue("lowerH2",lowerH2);
    targetsettings->setValue("upperH1",upperH1);
    targetsettings->setValue("upperH2",upperH2);
    targetsettings->setValue("lowerS1",lowerS1);
    targetsettings->setValue("lowerS2",lowerS2);
    targetsettings->setValue("upperS1",upperS1);
    targetsettings->setValue("upperS2",upperS2);
    targetsettings->setValue("lowerV1",lowerV1);
    targetsettings->setValue("lowerV2",lowerV2);
    targetsettings->setValue("upperV1",upperV1);
    targetsettings->setValue("upperV2",upperV2);
    targetsettings->endGroup();
    targetsettings->beginGroup("turret");
    targetsettings->setValue("xspeed",ui->xmovespeed->text().toDouble());
    targetsettings->setValue("yspeed",ui->ymovespeed->text().toDouble());
    targetsettings->setValue("xoff",ui->xoffset->text().toDouble());
    targetsettings->setValue("yoff",ui->yoffset->text().toDouble());
    targetsettings->endGroup();
    delete targetsettings;
    on_accept_clicked();
}

// updates the vectors that point to values held by the mainwindow
// these calls would be dangerous if these values were used constantly
// but considering the values are used when the user needs them
// and that these pointers are always predefined these calls are safe
void CalibrationWindow::on_accept_clicked()
{
    ints->clear();
    *ints<<lowerH1;
    *ints<<lowerS1;
    *ints<<lowerV1;
    *ints<<upperH1;
    *ints<<upperS1;
    *ints<<upperV1;
    *ints<<lowerH2;
    *ints<<lowerS2;
    *ints<<lowerV2;
    *ints<<upperH2;
    *ints<<upperS2;
    *ints<<upperV2;
    doubles->clear();
    *doubles<<ui->targdist->text().toDouble();
    *doubles<<ui->turroffset->text().toDouble();
    turr->setparameters(ui->xmovespeed->text().toDouble(),ui->ymovespeed->text().toDouble(),ui->xoffset->text().toDouble(),ui->yoffset->text().toDouble());
    accept();
}

// closes the window without updating
void CalibrationWindow::on_cancel_clicked()
{
    reject();
}
// changes the sliders to show the values of the first threshold range
void CalibrationWindow::on_highendbutton_clicked()
{
    if(highchecked==false)
    {
    lowerH1=ui->lowHslider->value();
    lowerS1=ui->lowSslider->value();
    lowerV1=ui->lowVslider->value();
    upperH1=ui->upperHslider->value();
    upperS1=ui->upperSslider->value();
    upperV1=ui->upperVslider->value();
    ui->lowHslider->setValue(lowerH2);
    ui->lowSslider->setValue(lowerS2);
    ui->lowVslider->setValue(lowerV2);
    ui->upperHslider->setValue(upperH2);
    ui->upperSslider->setValue(upperS2);
    ui->upperVslider->setValue(upperV2);
    }
    highchecked=true;
}

// changes the sliders in the gui to display the second range of theshold values
void CalibrationWindow::on_lowendbutton_clicked()
{
    if(highchecked==true)
    {
    lowerH2=ui->lowHslider->value();
    lowerS2=ui->lowSslider->value();
    lowerV2=ui->lowVslider->value();
    upperH2=ui->upperHslider->value();
    upperS2=ui->upperSslider->value();
    upperV2=ui->upperVslider->value();
    ui->lowHslider->setValue(lowerH1);
    ui->lowSslider->setValue(lowerS1);
    ui->lowVslider->setValue(lowerV1);
    ui->upperHslider->setValue(upperH1);
    ui->upperSslider->setValue(upperS1);
    ui->upperVslider->setValue(upperV1);
    }
    highchecked=false;
}

// thesholds the image for display by the calibration window so the user may see the results of the changes
void CalibrationWindow::threshimage(IplImage* imagein,IplImage* imageout)
{
    IplImage* imgHSV=cvCreateImage(cvGetSize(imagein),8,3);
    cvCvtColor(imagein,imgHSV,CV_BGR2HSV);
    IplImage* tmp1=cvCreateImage(cvGetSize(imagein),8,1);
    IplImage* tmp2=cvCreateImage(cvGetSize(imagein),8,1);
    cvInRangeS(imgHSV,cvScalar(lowerH1,lowerS1,lowerV1),cvScalar(upperH1,upperS1,upperV1),tmp1);
    cvInRangeS(imgHSV,cvScalar(lowerH2,lowerS2,lowerV2),cvScalar(upperH2,upperS2,upperV2),tmp2);
    cvOr(tmp1,tmp2,imageout);
    cvReleaseImage(&imgHSV);
    cvReleaseImage(&tmp1);
    cvReleaseImage(&tmp2);
}

// updates the image to display the new thresholded image when the user slides a slider
void CalibrationWindow::updateImage()
{
    if(ui->lowendbutton->isChecked())
    {
        lowerH1=ui->lowHslider->value();
        lowerS1=ui->lowSslider->value();
        lowerV1=ui->lowVslider->value();
        upperH1=ui->upperHslider->value();
        upperS1=ui->upperSslider->value();
        upperV1=ui->upperVslider->value();
    }
    else if(ui->highendbutton->isChecked())
    {
        lowerH2=ui->lowHslider->value();
        lowerS2=ui->lowSslider->value();
        lowerV2=ui->lowVslider->value();
        upperH2=ui->upperHslider->value();
        upperS2=ui->upperSslider->value();
        upperV2=ui->upperVslider->value();
    }
    threshimage(testingimage,threshedimage);
    tmpimage=IplImage2QImage(threshedimage);
    QPixmap currentimage=QPixmap::fromImage(*tmpimage).scaledToHeight(200);
    ui->calibrationimage->setPixmap(currentimage);
    delete tmpimage;
}





//convert OpenCV image to QImage
//takes IplImage format that is obtained from kinect
//returns a pointer to a QImage
QImage*  CalibrationWindow::IplImage2QImage(IplImage *iplImg)
 {
 int h = iplImg->height;
 int w = iplImg->width;
 int channels = iplImg->nChannels;
 QImage *qimg = new QImage(w, h, QImage::Format_RGB32);
 if(qimg->isNull())
 {
     qDebug()<<"unable to allocate image";
 }
 data = iplImg->imageData;

for (int y = 0; y < h; y++, data += iplImg->widthStep)
 {
 for (int x = 0; x <w; x++)
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
 qimg->setPixel((w-1)-x, y, qRgba(r, g, b, a));
 }
 else
 {
 qimg->setPixel((w-1)-x, y, qRgb(r, g, b));
 }
 }
 }

 return qimg;
}

// returns values int he calibration window to defaults
void CalibrationWindow::on_defaultbutton_clicked()
{
    ui->targdist->setText("10");
    ui->turroffset->setText("-0.66");
    lowerH1=0;
    lowerH2=150;
    upperH1=20;
    upperH2=180;
    lowerS1=175;
    lowerS2=175;
    upperS1=256;
    upperS2=256;
    lowerV1=81;
    lowerV2=81;
    upperV1=133;
    upperV2=133;
    ui->xmovespeed->setText("53.8");
    ui->ymovespeed->setText("32");
    ui->xoffset->setText("0");
    ui->yoffset->setText("0");
    if(highchecked==false)
    {
        ui->lowHslider->setValue(lowerH1);
        ui->lowSslider->setValue(lowerS1);
        ui->lowVslider->setValue(lowerV1);
        ui->upperHslider->setValue(upperH1);
        ui->upperSslider->setValue(upperS1);
        ui->upperVslider->setValue(upperV1);
    }
    else if(highchecked==true)
    {
        ui->lowHslider->setValue(lowerH2);
        ui->lowSslider->setValue(lowerS2);
        ui->lowVslider->setValue(lowerV2);
        ui->upperHslider->setValue(upperH2);
        ui->upperSslider->setValue(upperS2);
        ui->upperVslider->setValue(upperV2);
    }
    updateImage();
}
