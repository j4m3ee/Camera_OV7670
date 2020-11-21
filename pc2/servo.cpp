#include "servo.h"

servo::servo()
{
    servoPan.attach(7);
    servoTilt.attach(8);
}

void servo::moveServo(char in)
{
    if(in == 'C'){
      moveServoTo(servoPan,panCentre);
      moveServoTo(servoTilt,tiltCentre);
    }else if(in == 'L'){ //left rotate
      moveServoTo(servoPan,panCentre+rotate);
      moveServoTo(servoTilt,tiltCentre-12);
    }else if(in == 'R'){ //right rotate
      moveServoTo(servoPan,panCentre-rotate);
      moveServoTo(servoTilt,tiltCentre+5);
    }
}

void servo::moveServoTo(Servo sv, int to)
{
    int from = sv.read();
    if (from < to)
    {
        for (int i = from; i < to; i++)
        {
            sv.write(i);
            delay(5);
        }
    }
    else
    {
        for (int i = from; i > to; i--)
        {
            sv.write(i);
            delay(10);
        }
    }
}
