#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "turret.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "readimages.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    // required for signals and slots
    Q_OBJECT
    
public:

    // constructor for main gui window
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:

    // stops the video feed and ends the thread
    void stopVideo();

    // signal emitted that is connected to the turret to stop the shooting process
    void stopshooting();

    // signal emitted that is connected to the turret to pause the shooting process
    void pauseshooting();
protected:

    // function that catches the close event of the gui to properly end the thread
    // also hides the gui but waits on the thread to finish before actually closing
    void closeEvent(QCloseEvent *);

private slots:

    // a list of slots called when buttons are used
    void on_upbutton_pressed();

    void on_upbutton_released();

    void on_rightbutton_pressed();

    void on_rightbutton_released();

    void on_leftbutton_pressed();

    void on_leftbutton_released();

    void on_downbutton_pressed();

    void on_downbutton_released();

    void on_firebutton_clicked();

    void on_reloadbutton_clicked();

    void on_startstopbutton_clicked();

    void on_stopButton_clicked();

    void on_pauseButton_clicked();

    // end button slots

    // a list of actions that are available in the top menu

    void on_actionReload_triggered();

    void on_actionConnect_to_Turret_triggered();

    void on_actionConnect_to_Camera_triggered();

    void on_actionCalibrate_triggered();

    void on_actionHelp_triggered();

    //end action slots

    // slot called by the image stream to update the gui image

    void updatepic(QPixmap);

    // slot called by the image stream to let the user know an error has occured
    void errorstring(QString);

    // slot called by a timer timeout to update the timer in the gui
    void updaterunningtime();

    // lets the gui know that the user clicked the ok in the pause window and the turret resumed shooting
    void shootingresumed();

private:

    // the user interface created by QtCreator
    Ui::MainWindow *ui;

    // pointer to turret object
    Turret *turr;

    // thread the image stream is placed into
    QThread* thread;

    // pointer to image stream object
    ReadImages* readimg;

    // function called if turret movement fails
    void senderror();

    // enables all gui buttons
    void enablebuttons();

    // disables all gui buttons
    void disablebuttons();

    // check to ensure turret still has a missile
    void checkformissiles();

    // sets up buttons after the start button is clicked to ensure the user does not click other buttons
    void setupautobuttons();

    // restores buttons to the standard state after no more targets or the user clicks stop
    void restoredefaultbuttons();

    // function to obtain the current image displayed by the gui
    QImage* getQImage();

    // converts a QImage object to an Ipl object
    IplImage* Qimage2IplImage(QImage*);

    // thresholds the image into binary
    IplImage* threshimage(IplImage*);

    // variables used to determine target locations
    double viewwidth,viewheight,beta,betav,x_actual,y_actual,offset,h,x_location_launcher,y_location_launcher;
    double pi;
    int n,k,alpha,alphav;

    // structure to define the center and angles of the target
struct targetcenter
{
    double x;
    double y;
    double beta;
    double betav;
};


// function to fill in targetcenter structure
void getangles(targetcenter*);

// vector of targetcenters
QVector<targetcenter*> targets;

// temporary pointer to a target center.  used before the targetcenter is stored in the vector
targetcenter* tmptargetcenter;

// variables to define threshold range

int lowerH1,lowerH2,upperH1,upperH2,lowerS1,lowerS2,upperS1,upperS2,lowerV1,lowerV2,upperV1,upperV2;

// boolean to notify the targeting process that the user clicked stop
bool shootingstopped;

// timer to update the display for the amount of time used to shoot targets
QTimer timeshooting;
};

#endif // MAINWINDOW_H
