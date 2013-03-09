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
    //constructor and destructor
ReadImages();
~ReadImages();
    
signals:

// tells the thread that the process has ended
void finished();

// sends error that occured
void error(QString err);

// tells the gui an image is ready
void picready(QPixmap map);

public slots:

// sets up connection to kinect
int initialize();

// sends an image to the gui
void sendimage();

// allows the gui to terminate the image stream
void endimages();

// allows the gui to begin the image stream
void startimages();

private:

    // used to convert image from Ipl format to QImage format that the gui can use
    QImage* IplImage2QImage(IplImage *iplImg);

    //initialized the connection to the kinect
     bool initKinect();

     // gets a color image from the kinect and stores as Ipl image
     int getColor(HANDLE,IplImage*);

     // timer to call getColor function
    QTimer* timer;

    // the kinect sensor
    INuiSensor* sensor;

    // link to the color image stream
    HANDLE rgbStream;

    // data to be used in conversion to QImage
    char* data;

    // pointer to image from the getColor function
    IplImage* frame;

    // QImage created during conversion
    QImage *tmpimage;

    // pixmap sent to gui to be displayed. The gui needs this format to display the image
    QPixmap tmppix;
};

#endif // READIMAGES_H
