#include "FM_tx.h"

//Freq = {100,600,1100,1600}
//Delay = {}

FM_tx::FM_tx()
{
  dac.begin(0x64);//A2
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / 4;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / 4;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / 4;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / 4;
  freq[0] = 500;
  freq[1] = 800;
  freq[2] = 1100;
  freq[3] = 1400;
  for (int i = 0; i < 4; i++)
  {
    Delay[i] = (1000000 / freq[i] - 1000000 / defaultFreq) / 4;
  }

}


void FM_tx::Transmit(String data) {
  int counter = 0;

  for (int i = 0; i < data.length(); i++)
  {
    inData[i] = data[i];
    counter++;
  }
  //    Serial.println(inp);
  //    Serial.println(counter);
  for (int i = 0; i < counter ; i++) //send data
  {
    //char preShifted = inData[i];

    for (int k = 7; k > 0; k -= 2) //send 8 bits from LSB tp MSB
    {
      int tmp = inData[i] & 3;
      int useDelay, cyc;
      if (tmp == 0)
      {
        //Serial.println("00");
        cyc = 5;
        useDelay = Delay[0];
      }
      else if (tmp == 1)
      {
        //Serial.println("01");
        cyc = 8;
        useDelay = Delay[1];
      }
      else if (tmp == 2)
      {
        //Serial.println("10");
        cyc = 11;
        useDelay = Delay[2];
      }
      else
      {
        //Serial.println("11");
        cyc = 14;
        useDelay = Delay[3];
      }
      for (int sl = 0; sl < cyc; sl++)
      {
        for (int s = 0; s < 4; s++) //4 sample/cycle
        {
          dac.setVoltage(S_DAC_fsk[s], false);//modify amplitude
          delayMicroseconds(useDelay);
        }
      }
      inData[i] >>= 2;
    }
  }
  dac.setVoltage(0, false);
}
