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
    Q_OBJECT

public:

    //constructor and destructor
    Turret(QWidget* parent=0);
    ~Turret();

    //moves the turret for a specified amount of time
    int move(MissileCmd direction,double duration);

    // missile count functions
    int currentmissilecount();
    void resetmissilecount();

    //connects to turret
    int init();

    //sets the turret to a specific angle
    void setAngle(double,double);

    //initializes the turret to a known angle
    void initAngle();

    QVector<double> getparameters();

    void setparameters(double,double,double,double);

        hid_device* handle;

public slots:

    void pauseshooting();

    void stopshooting();

signals:

    void resume();

private slots:

    void emitstop();

private:

    //sends the command to the turret
    int sendmsg(MissileCmd);

    //current number of missiles on the launcher
    int missilecount;


    //the array to send to the launcher
    unsigned char buf[9];

    //the launcher current angles
    double beta;
    double betav;

    double xmovespeed;
    double ymovespeed;
    double xoffset;
    double yoffset;

    QTimer movetimer;
    bool inmotion;
    bool shootingcanceled;




    double timethismove;
    QTime timemoved;
    MissileCmd currentmovedirection;
};

#endif // TURRET_H
