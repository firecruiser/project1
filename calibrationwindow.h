#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QDialog>
#include "turret.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

namespace Ui {
class CalibrationWindow;
}

class CalibrationWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationWindow(QWidget *parent = 0,Turret* turr = NULL,QVector<int>* incomingdataint=NULL,QVector<double>* incomingdatadouble=NULL,IplImage* tmpimagein=NULL);
    ~CalibrationWindow();
    
private slots:
    void on_accept_clicked();

    void on_cancel_clicked();

    void on_saveaccept_clicked();

    void on_highendbutton_clicked();

    void on_lowendbutton_clicked();

    void updateImage();

    void on_defaultbutton_clicked();

private:
    Ui::CalibrationWindow *ui;
    int lowerH1,lowerH2,upperH1,upperH2,lowerS1,lowerS2,upperS1,upperS2,lowerV1,lowerV2,upperV1,upperV2;
    double h,offset,xmovespeed,ymovespeed,xoffset,yoffset;
    Turret* turr;
    QVector<int>* ints;
    QVector<double>* doubles;
    IplImage* testingimage;
    IplImage* threshedimage;
    QImage* tmpimage;
    QTimer updatetime;
    bool highchecked;
    void threshimage(IplImage*,IplImage*);
    QImage* IplImage2QImage(IplImage*);
    char* data;
};

#endif // CALIBRATIONWINDOW_H
