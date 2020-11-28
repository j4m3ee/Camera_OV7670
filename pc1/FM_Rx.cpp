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
}



String FM_Rx::Receive(){
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
        Serial.println(char(data));
        output_data  += char(data);
        Serial.println(output_data);
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
  return output_data;
}

void FM_Rx::Clear(){
  output_data = "";
}
