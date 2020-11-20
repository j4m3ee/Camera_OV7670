//FM-Tx
#include<Wire.h>
#include<Adafruit_MCP4725.h>
#include<Adafruit_ADS1015.h>

#define defaultFreq 1700
#define f0 100
#define f1 600
#define f2 1100
#define f3 1650
int delay0, delay1, delay2, delay3;

const uint16_t S_DAC[4] = {0, 1000, 2000, 1000};  // 10 bits input
Adafruit_MCP4725 dac;

char inData[30];

//FM-Rx
#include <Wire.h>
#include <TEA5767Radio.h>

TEA5767Radio radio = TEA5767Radio();

#ifndef cbi
#define cbi(sfr, bit)(_SFR_BYTE(sfr)&=~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)(_SFR_BYTE(sfr)|=_BV(bit))
#endif

int prev = 0;
int count = 0;

uint16_t baud_check = 0;
uint16_t data = 0;
uint16_t bit_check = -1;

bool check_amp = false;
bool check_baud = false;

uint32_t baud_begin = 0;

//edit this number
#define r_slope 300


//Servo
#include <Servo.h>
Servo hServo;
Servo vServo;

void leftMove(){
  hServo.write(0);
}

void rightMove(){
  hServo.write(90);
}

void centreMove(){
  hServo.write(90);
}

void fmRecive(){
  int tmp = analogRead(A2);

  if ( tmp > r_slope and prev < r_slope and !check_amp ) // check amplitude
  {
    //    Serial.println(tmp);
    check_amp = true; // is first max amplitude in that baud
    if ( !check_baud )
    {
      baud_begin = micros();
      bit_check++;
    }
  }

  if (tmp < r_slope and check_baud) {
    if (micros() - baud_begin > 9900 ) // full baud
    {
      uint16_t last = (int(ceil(float(count - 1) / 5)) & 3) << (bit_check * 2);;  // shift data
      data |= last;                                                 // add two new bits in data
      baud_check++;
      //      Serial.print("COUNT\t\t");
//      Serial.println(count);
      if (baud_check == 4) // 8 bits
      {
        Serial.print((char)data);
        data = 0;
        baud_check = 0;
        bit_check = -1;
      }
      check_baud = false;
      count = 0;
    }
  }

  if (micros() - baud_begin > 40000 ) {
    data = 0;
    baud_check = 0;
    bit_check = -1;
    check_amp = false;
    check_baud = false;
    count = 0;
  }

  if (tmp > r_slope and check_amp) {
    count++;
    check_baud = true;
    check_amp = false;
  }
  prev = tmp;
}

void fmTransmit(){
  if (Serial.available() > 0)
  {
    int counter = 0;
    String inp = Serial.readString();
    inp += "\n";
    for(int i=0;i<inp.length();i++)
    {
      inData[i] = inp[i];
      counter++;  
    }
//    Serial.println(inp);
//    Serial.println(counter);
    for (int s=0;s<4;s++) //4 sample/cycle
        {
            dac.setVoltage(1000, false);//modify amplitude
            delayMicroseconds(delay0);
        }
    for (int i=0;i<counter-1;i++) //send data
    {
      //char preShifted = inData[i];
      
      for (int k = 7; k > 0; k -= 2) //send 8 bits from LSB tp MSB
      {
        int tmp = inData[i] & 3;
        int useDelay,cyc;  
        if(tmp == 0)
        {
          //Serial.println("00");
          cyc = 1;
          useDelay = delay0;   
        }
        else if(tmp == 1)
        {
          //Serial.println("01");
          cyc = 6;
          useDelay = delay1;  
        }
        else if(tmp == 2)
        {
          //Serial.println("10");
          cyc = 11;
          useDelay = delay2; 
        }
        else
        {
          //Serial.println("11");
          cyc = 16;
          useDelay = delay3;  
        }
        
        for (int sl=0;sl<cyc;sl++)
        {
          for (int s=0;s<4;s++) //4 sample/cycle
          {
            dac.setVoltage(S_DAC[s], false);//modify amplitude
            delayMicroseconds(useDelay);
          }
        }
        inData[i]>>=2;
      }
      for (int s=0;s<4;s++) //4 sample/cycle
        {
            dac.setVoltage(1000, false);//modify amplitude
            delayMicroseconds(delay0);
        }
    }
    dac.setVoltage(0, false);
    Serial.flush();
  }
}

void setup() {
  //Servo
  hServo.attach(9);
  vServo.attach(10);

  //FM-Rx
  sbi(ADCSRA, ADPS2); // this for increase analogRead speed
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  Serial.begin(115200);
  Serial.flush();

  Wire.begin();
  radio.setFrequency(87.3);

  //FM-Tx
  dac.begin(0x62);//A2
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
  Serial.begin(115200);
  Serial.flush();
}

void loop() {
  leftMove();
  delay(1000);
  centreMove();
  delay(1000);
  rightMove();
  delay(1000);
}
