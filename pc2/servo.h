#include <Arduino.h>
#include <Servo.h>

class servo
{
    public:
        servo();
        void moveServo(char in);
    private:
        Servo servoPan = Servo();
        Servo servoTilt = Servo();
        int panCentre = 142;
        int tiltCentre = 80;
        int rotate = 45;
        void moveServoTo(Servo sv,int to);
};
