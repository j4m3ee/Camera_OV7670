#include "FM_Rx.h"
#ifndef cbi
#define cbi(sfr, bit)(_SFR_BYTE(sfr)&=~_BV(bit)) 
#endif
#ifndef sbi
#define sbi(sfr, bit)(_SFR_BYTE(sfr)|=_BV(bit))
#endif

//edit this number
#define r_slope 200
FM_Rx::FM_Rx()
{
      sbi(ADCSRA, ADPS2); // this for increase analogRead speed
      cbi(ADCSRA,ADPS1);
      cbi(ADCSRA,ADPS0);
//    sbi(ADCSRA, ADPS2); // this for increase analogRead speed
//    cbi(ADCSRA, ADPS1);
//    cbi(ADCSRA, ADPS0);
//
//    Wire.begin();
    //radio.setFrequency(freq);
}

//uint8_t FM_Rx::receiveFrame(int timeout)
//{
//    unsigned long start = millis();
//    uint8_t aData[40];
//    int cIdx = 0;
//
//    while (millis() - start < timeout)
//    {
//        uint16_t tmp = analogRead(A2);
//        if (tmp - r_slope and prev < r_slope and !check_amp)
//        {
//            check_amp = true;
//            if (!check_baud)
//            {
//                baud_begin = micros();
//                bit_check++;
//            }
//        }
//
//        if (tmp < r_slope and check_baud)
//        {
//            if (micros() - baud_begin > baudTime)
//            {
//                uint16_t last = (int(ceil(float(count - 1) / 5)) & 3) << (bit_check * 2);
//                ;
//                data |= last;
//                baud_check++;
//                if (baud_check == 4)
//                {
//                    //return char(data);
//                    aData[cIdx] = data;
//                    cIdx++;
//                    data = 0;
//                    baud_check = 0;
//                    bit_check = -1;
//                }
//                check_baud = false;
//                count = 0;
//            }
//        }
//        if (micros() - baud_begin > 40000)
//        {
//            data = 0;
//            baud_check = 0;
//            bit_check = -1;
//            check_amp = false;
//            check_baud = false;
//            count = 0;
//        }
//
//        if (tmp > r_slope and check_amp)
//        {
//            count++;
//            check_baud = true;
//            check_amp = false;
//        }
//        prev = tmp;
//    }
//    return aData;
//}

char FM_Rx::Receive(){
  int tmp = analogRead(A3);
  
  if ( tmp > r_slope and prev < r_slope and !check_amp ) // check amplitude
  {
    check_amp = true; // is first max amplitude in that baud
    if ( !check_baud )
    {
      baud_begin = micros();
      bit_check++;
    }
  }

  if(tmp < r_slope and check_baud) {
    if (micros() - baud_begin > 9900 ) // full baud
    {
      uint16_t last = (((count - 5) / 3) & 3) << (bit_check * 2);;  // shift data
      data |= last;                                                 // add two new bits in data
      baud_check++;
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
  
  if(tmp > r_slope and check_amp) {
    count++;
    check_baud = true;
    check_amp = false;
  }
  prev = tmp;
  return char(data);
}