#include <Servo.h>
Servo servoPan;
Servo servoTilt;

#define panCentre 142
#define tiltCentre 80
#define rotate 45
void setup()
{
  Serial.begin(115200);
  servoPan.attach(7);
  servoTilt.attach(8);
  
  servoTilt.write(tiltCentre); //default 90
  servoPan.write(panCentre); //default 47
}

void moveServoTo(Servo sv,int to){
  int from = sv.read();
  if(from < to){
    for(int i=from;i<to;i++){
      sv.write(i);
      delay(5);
    }
  }
  else{
    for(int i=from;i>to;i--){
      sv.write(i);
      delay(10);
    }
  }
}

char last = 'C';
void loop()
{
  if (Serial.available()) {
    char in = Serial.read();
    if(in == 'C'){
      moveServoTo(servoPan,panCentre);
      moveServoTo(servoTilt,tiltCentre-5);
    }else if(in == 'L'){ //left rotate
      moveServoTo(servoPan,panCentre+rotate);
      moveServoTo(servoTilt,tiltCentre-10);
    }else if(in == 'R'){ //right rotate
      moveServoTo(servoPan,panCentre-rotate);
      moveServoTo(servoTilt,tiltCentre+2);
    }
  }
}
