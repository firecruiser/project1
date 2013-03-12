#include "turret.h"
#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <qDebug>





//creates the turret object and sets the access pointer to NULL
Turret::Turret(QWidget* parent) : QWidget(parent)
{
handle=NULL;
QSettings* targetsettings=new QSettings("targetsettings.ini",QSettings::IniFormat);
targetsettings->beginGroup("turret");
xmovespeed=targetsettings->value("xspeed",53.8).toDouble();
ymovespeed=targetsettings->value("yspeed",32).toDouble();
xoffset=targetsettings->value("xoff",0).toDouble();
yoffset=targetsettings->value("yoff",0).toDouble();
targetsettings->endGroup();
delete targetsettings;
}


//obtains connection to turret and returns whether or not the turret is connected
int Turret::init()
{
    //initialize the turret and the buffer to pass to the turret
    int turretfound=0;
    if(handle!=NULL)
    {
        hid_close(handle);
    }


    handle=hid_open(0x2123,0x1010,NULL);
    // if the turret is not found an error is returned
    if(handle==NULL)
    {
        turretfound=-1;
        return turretfound;
    }
    hid_set_nonblocking(handle,1);


    for(int i=0;i<9;i++)
    {
        buf[i]=0x0;
    }
    buf[1]=0x02;
    missilecount=0;
    connect(&movetimer,SIGNAL(timeout()),this,SLOT(emitstop()));
    return turretfound;
}




//sends signal to turret to move and sends stop signal after duration passes
//this may be changed to just accept the command and have the main loop send the stop command
int Turret::move(MissileCmd cmd,double duration)
{
    if(sendmsg(cmd)<0)
    {
        return -1;
    }
    if(duration!=0)
    {
        timethismove=duration;
        timemoved.start();
        currentmovedirection=cmd;
        movetimer.start(duration*1000);
    }
    if(cmd==FIRE)
    {
        missilecount=missilecount-1;
    }
    return 0;
}




//actually sends the message to the turret
int Turret::sendmsg(MissileCmd cmd)
{
buf[2]=cmd;
return hid_write(handle,buf,9);
}




//returns the current number of missiles remaining
int Turret::currentmissilecount()
{
    return missilecount;
}



//resets the missile count to a fully loaded 4 missiles
void Turret::resetmissilecount()
{
    missilecount=4;
}


//sets the turrets angle to the defined angle uses the angles given and incorporates offsets and movespeed of the turret
void Turret::setAngle(double x,double y)
{
    shootingcanceled=false;
    y+=yoffset;
    y=(y<-4)?-4:y;
    y=(y>30)?30:y;
    x+=xoffset;
    double movedistx=x-beta;
    double movedisty=y-betav;
    double movextime=fabs(movedistx/xmovespeed);
    double moveytime=fabs(movedisty/ymovespeed);
    MissileCmd command;
    command=(movedistx>0)?RIGHT:LEFT;
    if(movedistx!=0)
    {
        inmotion=true;
        move(command,movextime);
        while(inmotion)
        {
            qApp->processEvents();
        }
        if(shootingcanceled)
        {
            return;
        }
    }
    command=(movedisty>0)?UP:DOWN;
    if(movedisty!=0)
    {
        inmotion=true;
        move(command,moveytime);
        while(inmotion)
        {
            qApp->processEvents();
        }
        if(shootingcanceled)
        {
            return;
        }
    }
    beta=x;
    betav=y;
    inmotion=true;
    move(FIRE,4.5);
    while(inmotion)
    {
        qApp->processEvents();
    }
}


// initializes the turret to the bottom left position
void Turret::initAngle()
{
    shootingcanceled=false;
    inmotion=true;
    if(shootingcanceled)
    {
        return;
    }
    move(LEFT,6);
    while(inmotion)
    {
        qApp->processEvents();
    }
    inmotion=true;
    if(shootingcanceled)
    {
        return;
    }
    move(DOWN,1.5);
    while(inmotion)
    {
        qApp->processEvents();
    }
    beta=-150;
    betav=-4;
}

QVector<double> Turret::getparameters()
{
    QVector<double> tmpdouble;
    tmpdouble<<xmovespeed<<ymovespeed<<xoffset<<yoffset;
    return tmpdouble;
}

void Turret::setparameters(double xmove,double ymove,double offx,double offy)
{
    xmovespeed=xmove;
    ymovespeed=ymove;
    xoffset=offx;
    yoffset=offy;

}

void Turret::emitstop()
{
    sendmsg(STOP);
    inmotion=false;
    movetimer.stop();
}

void Turret::pauseshooting()
{
    emitstop();
    double timeyet=timethismove-timemoved.elapsed();
    QMessageBox::information(this,"Paused","Shooting Paused! \n Ok to Resume");
    emit(resume());
    inmotion=true;
    move(currentmovedirection,timeyet);
    while(inmotion)
    {
        qApp->processEvents();
    }
    inmotion=true;
    if(shootingcanceled)
    {
        return;
    }


}

void Turret::stopshooting()
{
    shootingcanceled=true;
    sendmsg(STOP);
    inmotion=false;
    movetimer.stop();

}


//closes connection to the turret and destroys the object
Turret::~Turret()
{
    hid_close(handle);
}

