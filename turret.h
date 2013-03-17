#ifndef TURRET_H
#define TURRET_H
#define NOMINMAX
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "hidapi/hidapi/hidapi.h"
#include <QtCore>
#include <QWidget>

using namespace std;


// and enum containing the launchers commands. Used to make code more readable
typedef enum
{
    FIRE = 0x10, LEFT = 0x4, RIGHT = 0x8, UP = 0x2, DOWN = 0x1, STOP = 0x0
} MissileCmd;

class Turret : public QWidget
{
    // required to connect signals and slots
    Q_OBJECT

public:

    //constructor and destructor
    /*
     * *parent = pointer to parent widget
     *sets up the default values for the turret and initializes the handle of the turret to NULL
     */
    Turret(QWidget* parent=0);
    ~Turret();

    //moves the turret for a specified amount of time (duration) in a specific direction (direction)
    int move(MissileCmd direction,double duration);

    // missile count functions
    // returns current count
    int currentmissilecount();

    // resets the cound to 4
    void resetmissilecount();

    //connects to turret
    // if unable to connect will return an error value for int
    int init();

    //sets the turret to a specific angle (x,y)
    void setAngle(double,double);

    //initializes the turret to a known angle (bottom left corner of movement)
    void initAngle();

    // returns a vector of parameters for the turret
    // used to allow editing of turret parameters
    QVector<double> getparameters();

    // sets the turrets parameter to the new values (xmovespeed,ymovespeed,xoffset,yoffset)
    void setparameters(double,double,double,double);

    // pointer to the device
        hid_device* handle;

public slots:

        // slots that are connected to the main window

        // used to pause the shooting routine allowing the routine to be resumed
    void pauseshooting();

        // used to stop the routine. unable to resume must restart
    void stopshooting();

signals:

        // signal called by the pauseshooting slot to allow the main window to know shooting was resumed
    void resume();

private slots:

        // slot called by the turret when the timer expires.
        // stops the turrets current movement so the next move can be started
    void emitstop();

private:

    //sends the command to the turret
    // returns an integer indicating success or failure
    int sendmsg(MissileCmd);

    //current number of missiles on the launcher
    int missilecount;


    //the array to send to the launcher
    unsigned char buf[9];

    //the launcher current angles
    double beta;
    double betav;

    // values defining the physical launchers movement speeds and offsets due to the relation of the turret and the camera
    // as well as to adjust for missile flight angles
    double xmovespeed;
    double ymovespeed;
    double xoffset;
    double yoffset;

    // timer that runs for a specified time
    // when time expires the timer causes the turret to stop moving
    // a sleep function could be used here but the gui would become inactive
    QTimer movetimer;

    // the timer controls this boolean to let the move function know when a single move is complete
    // required to keep the gui active
    bool inmotion;

    // boolean to let the move functions know to cancel movement
    bool shootingcanceled;

    // the amount of time the turret needs to move
    double timethismove;

    // function used to obtain the amount of time the turret has moved thus far
    QTime timemoved;

    // the curren direction the turret is moving
    MissileCmd currentmovedirection;
};

#endif // TURRET_H
