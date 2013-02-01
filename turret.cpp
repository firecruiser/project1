#include "turret.h"
#include <QtGui>
#include <qDebug>






Turret::Turret()
{
handle=NULL;
}

int Turret::init()
{
    //initialize the turret and the buffer to pass to the turret
    int turretfound=0;
    if(handle!=NULL)
    {
        hid_close(handle);
    }


    handle=hid_open(0x2123,0x1010,NULL);
    if(handle==NULL)
    {
        turretfound=-1;
        qDebug()<<"no device found";
        return turretfound;
    }
    hid_set_nonblocking(handle,1);


    for(int i=0;i<9;i++)
    {
        buf[i]=0x0;
    }
    buf[1]=0x02;
    missilecount=0;
    return turretfound;
}



int Turret::move(MissileCmd cmd,double duration)
{
    int success=0;
    if(sendmsg(cmd)<0)
    {
        success=-1;
        qDebug()<<"message failed";
        return success;
    }
    if(duration!=0)
    {
        Sleep(duration*1000);
        sendmsg(STOP);
    }
    if(cmd==FIRE)
    {
        missilecount=missilecount-1;
    }
    return success;
}





int Turret::sendmsg(MissileCmd cmd)
{
int success;
buf[2]=cmd;
success=hid_write(handle,buf,9);
return success;
}




































int Turret::currentmissilecount()
{
    return missilecount;
}




void Turret::resetmissilecount()
{
    missilecount=4;
}




Turret::~Turret()
{
    hid_close(handle);
}

