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

// constructor
ReadImages::ReadImages()
{
}


// destructor
ReadImages::~ReadImages()
{

}



//stops image collection so the readimages object may be destroyed and the thread stopped
void ReadImages::endimages()
{
    timer->stop();
    emit finished();
}


//after initiation - starts timer and begins sending image signals
void ReadImages::startimages()
{
    frame=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,4);
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendimage()));
    timer->start(30);
}

//gets image from kinect then converts the image into a QImage format to be sent to the gui
void ReadImages::sendimage()
{
    tmpimage=NULL;
    if(getColor(rgbStream,frame)!=0)
    {
        return;
    }

        tmpimage=IplImage2QImage(frame);
        tmppix=QPixmap::fromImage(*tmpimage);
        delete tmpimage;
        emit picready(tmppix);
}




//Initializes the kinect and returns an error if the kinect cannot be found or the image stream cannot be started
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

}

//called by the initiating function and begins the image stream.  will return false if the sensor cannot be obtained
//defines a handle - rgbStream to get images from
//defines sensor to allow access to the kinect
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



//convert OpenCV image to QImage
//takes IplImage format that is obtained from kinect
//returns a pointer to a QImage
QImage*  ReadImages::IplImage2QImage(IplImage *iplImg)
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




//obtains image from kinect and defines the IplImage poiter to the image obtained from the kinect
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
