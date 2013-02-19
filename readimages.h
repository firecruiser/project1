#ifndef READIMAGES_H
#define READIMAGES_H

#include <QObject>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <QPixmap>
#include <QTimer>
#include <windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

class ReadImages : public QObject
{
    Q_OBJECT
public:
ReadImages();
~ReadImages();
    
signals:
void finished();
void error(QString err);
void picready(QPixmap map);

public slots:
int initialize();
void sendimage();
void endimages();
void startimages();

private:
    QImage* IplImage2QImage(IplImage *iplImg);
     IplImage* GetThresholdedImage(IplImage* img);
     bool initKinect();
     int getColor(HANDLE,IplImage*);
    QTimer* timer;
    CvCapture* capture;

    INuiSensor* sensor;
    HANDLE rgbStream;
    char* data;
    IplImage* frame;
    QImage* tmpimage;
    QPixmap tmppix;
};

#endif // READIMAGES_H
