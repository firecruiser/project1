#include "readimages.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <NuiApi.h>
#include <NuiSensor.h>
#include <NuiImageCamera.h>
#include <Windows.h>
#include <Ole2.h>


ReadImages::ReadImages()
{
}



ReadImages::~ReadImages()
{

}


int ReadImages::initialize()
{

    if(!initKinect())
    {
        emit error("Unable to find Kinect");
        return -1;
    }
    else
    {
        return 0;
    }

    //enable below code to connect to webcam
    //this is disabled except for testing purposes without a kinect device
    /*
    capture=0;
    capture = cvCaptureFromCAM(0);
    if(!capture)
    {
        emit error("Unable to obtain image from camera");
        qDebug()<<"no camera";

        return -1;
    }
    else
    {
        return 0;
    }
    */

}


bool ReadImages::initKinect() {
    // Get a working kinect sensor
    int numSensors;
    if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
    if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

    // Initialize sensor
    sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
    sensor->NuiImageStreamOpen(
        NUI_IMAGE_TYPE_COLOR,            // Depth camera or rgb camera?
        NUI_IMAGE_RESOLUTION_640x480,    // Image resolution
        0,      // Image stream flags, e.g. near mode
        2,      // Number of frames to buffer
        NULL,   // Event handle
        &rgbStream);
    return sensor;
}




QImage*  ReadImages::IplImage2QImage(IplImage *iplImg)
 {
 int h = iplImg->height;
 int w = iplImg->width;
 int channels = iplImg->nChannels;
 QImage *qimg = new QImage(w, h, QImage::Format_ARGB32);
 data = iplImg->imageData;

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

IplImage* ReadImages::GetThresholdedImage(IplImage* img)
{
    IplImage* imgHSV=cvCreateImage(cvGetSize(img),8,3);
    cvCvtColor(img,imgHSV,CV_BGR2HSV);
    IplImage* imgThreshed = cvCreateImage(cvGetSize(img),8,1);
    cvInRangeS(imgHSV,cvScalar(160,100,100),cvScalar(179,255,255),imgThreshed);
    cvReleaseImage(&imgHSV);
    return imgThreshed;
}

void ReadImages::sendimage()
{
    frame=0;
    tmpimage=NULL;
    if(getColor(rgbStream,frame)!=0)
    {
        return;
    }

    //testing code that uses webcam image capture
    /*
        frame=cvQueryFrame(capture);
        if(!frame)
            return;
        //IplImage* imgThresh=GetThresholdedImage(frame);
        */
        tmpimage=IplImage2QImage(frame);
        tmppix=QPixmap::fromImage(*tmpimage);
        delete tmpimage;
        emit picready(tmppix);
}

int ReadImages::getColor(HANDLE h, IplImage* color)
{
    const NUI_IMAGE_FRAME * pImageFrame = NULL;
    HRESULT hr = NuiImageStreamGetNextFrame(h, 0, &pImageFrame);
    if (FAILED(hr))
    {
        return -1;
    }
    INuiFrameTexture * pTexture = pImageFrame->pFrameTexture;
    NUI_LOCKED_RECT LockedRect;
    pTexture->LockRect(0, &LockedRect, NULL, 0);
    if (LockedRect.Pitch != 0)
    {
        BYTE * pBuffer = (BYTE*) LockedRect.pBits;
        cvSetData(color, pBuffer, LockedRect.Pitch);
    }
    NuiImageStreamReleaseFrame(h, pImageFrame);
    return 0;
}

void ReadImages::endimages()
{
    timer->stop();
    cvReleaseCapture(&capture);
    emit finished();
}

void ReadImages::startimages()
{
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendimage()));
    timer->start(60);
}
