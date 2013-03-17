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
    // required by QDialog to enable signals and slots
    Q_OBJECT
    
public:

    //constructor for calibration window
    /*inputs
     *parent = parent widget pointer
     *turr* = pointer to connected turret object
     *incomingdataint = pointer to integer data stored in mainwindow
     *incomingdatadouble = pointer to double data stored in main window
     *tmpimagein = pointer to image from kinect
     */
    explicit CalibrationWindow(QWidget *parent = 0,Turret* turr = NULL,QVector<int>* incomingdataint=NULL,QVector<double>* incomingdatadouble=NULL,IplImage* tmpimagein=NULL);
    ~CalibrationWindow();
    
private slots:

    //button clicked slots
    //these are called when a button on the gui is clicked
    void on_accept_clicked();

    void on_cancel_clicked();

    void on_saveaccept_clicked();

    void on_highendbutton_clicked();

    void on_lowendbutton_clicked();

    void on_defaultbutton_clicked();


    //updates the thresholded image displayed in the calibration window
    //allows the user to change the color ranges to refine the thresholding point
    void updateImage();

private:


    // constructor to generate user interface created by Qt Designer
    Ui::CalibrationWindow *ui;

    // integer values of the thresholding ranges
    int lowerH1,lowerH2,upperH1,upperH2,lowerS1,lowerS2,upperS1,upperS2,lowerV1,lowerV2,upperV1,upperV2;

    // double values for the turret movement and the location of the turret
    double h,offset,xmovespeed,ymovespeed,xoffset,yoffset;

    // pointer to the connected turret object
    Turret* turr;

    // a vector of ints used to store temporary data
    QVector<int>* ints;

    // a vector of doubles used to store temporary data
    QVector<double>* doubles;

    // pointers to IplImages that hold the color image and the thresholded binary image shows in the calibration window
    IplImage* testingimage;
    IplImage* threshedimage;

    // pointer to a QImage that temporarily holds the image before shown in the gui
    QImage* tmpimage;

    // boolean that determines which radio button in the gui is checked when one of the is clicked
    bool highchecked;

    // function that thresholds an IplImage
    /*
     *takes in two IplImage* one is the input and one is the output
     *The image is thresholded using the currently defined values in the gui
     */
    void threshimage(IplImage*,IplImage*);

    // function that converts an IplImage to a QImage
    /*
     *IplImage* = incoming thesholded IplImage
     *
     *returns
     *
     *QImage* = image that is returned so it may be displayed in the gui
     */
    QImage* IplImage2QImage(IplImage*);

    // a pointer to a temporary data array that allows for easy conversion from IplImage to QImage
    char* data;
};

#endif // CALIBRATIONWINDOW_H
