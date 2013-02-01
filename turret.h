#ifndef TURRET_H
#define TURRET_H
#define NOMINMAX
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "hidapi/hidapi/hidapi.h"

using namespace std;

typedef enum
{
    FIRE = 0x10, LEFT = 0x4, RIGHT = 0x8, UP = 0x2, DOWN = 0x1, STOP = 0x0
} MissileCmd;

class Turret
{

public:
    Turret();
    ~Turret();
    int move(MissileCmd direction,double duration);
    int currentmissilecount();
    void resetmissilecount();
    int init();
private:
    int sendmsg(MissileCmd);
    int missilecount;
    hid_device* handle;
    unsigned char buf[9];
};

#endif // TURRET_H
