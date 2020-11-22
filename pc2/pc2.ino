#include "FM_rx.h"
#include "FM_tx.h"
#include "servo.h"

FM_rx *receiver;
FM_tx *transmitter;
servo *director;

uint8_t buff[20];

void setup()
{
  Serial.begin(115200);
  Serial.flush();

  director = new servo();
  receiver = new FM_rx(97.5);
  transmitter = new FM_tx();

//  output = createWriter
}

uint8_t in[3] = {'L','C','R'};

void loop()
{
    for(int i=0;i<3;i++)
    {
      director -> moveServo(in[i]);
      Serial.write(in[i]);
      while(!(Serial.available())
    }
}
