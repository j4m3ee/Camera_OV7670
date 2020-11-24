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

  //  uint16_t S_DAC[num_S_Dac] = {0, 1000, 2000, 1000};
  //  uint16_t cycle[num_cycle] = {1, 6, 11, 16};
  //
  //
  //  for (int i = 0; i < num_Freq; i++)
  //  {
  //    freq[i] = 100 + (i * f_diff);
  //  }
  //  for (int i = 0; i < num_Delay; i++)
  //  {
  //    Delay[i] = (1000000 / freq[i] - 1000000 / defaultFreq) / 4;
  //  }
  //
  ////    freq[0] = 100;
  ////    freq[1] = 600;
  ////    freq[2] = 1100;
  ////    freq[3] = 1600;
  ////    Delay[0] = (1000000 / freq[0] - 1000000 / defaultFreq) / 4;
  ////    Delay[1] = (1000000 / freq[1] - 1000000 / defaultFreq) / 4;
  ////    Delay[2] = (1000000 / freq[2] - 1000000 / defaultFreq) / 4;
  ////    Delay[3] = (1000000 / freq[3] - 1000000 / defaultFreq) / 4;
  //
}

//void FM_tx::sentFrame(String data)
//{
//  //sentHead();
//  for (uint16_t i = 0; i < data.length(); i++)
//  {
//    transmit(data[i]);
//  }
//  //sentHead();
//}
//
//char FM_tx::packFrame(String data)
//{
//  uint16_t len = data.length();
//  //Flag : ~ [1 bytes]
//  //Frame seq : 0/1 [1 bit]
//  return 'a';
//}
//
//void FM_tx::transmit(char data)
//{
//  int tmp;
//  for (int i = 0; i < 4; i++)
//  {
//    tmp = data & 3;
//
//    //
//    int useDelay,cyc;
//        if(tmp == 0)
//        {
//          //Serial.println("00");
//          cyc = 1;
//          useDelay = Delay[0];
//        }
//        else if(tmp == 1)
//        {
//          //Serial.println("01");
//          cyc = 6;
//          useDelay = Delay[1];
//        }
//        else if(tmp == 2)
//        {
//          //Serial.println("10");
//          cyc = 11;
//          useDelay = Delay[2];
//        }
//        else
//        {
//          //Serial.println("11");
//          cyc = 16;
//          useDelay = Delay[3];
//        }
//        Serial.write('A');
//        for (int sl=0;sl<cyc;sl++)
//        {
//          for (int s=0;s<4;s++)
//          {
//            dac.setVoltage(S_DAC[s], false);//modify amplitude
//            delayMicroseconds(useDelay);
//          }
//        }
//
//    //
//
//    //dacSent(cycle[tmp], Delay[tmp]);
//    data >>= 2;
//  }
//}
//
//void FM_tx::dacSent(uint16_t cyc, int dur)
//{
//  for (uint16_t c = 0; c < cyc; c++)
//  {
//    for (uint16_t i = 0; i < 4; i++)
//    {
//
//      dac.setVoltage(S_DAC[i], false);
//      delayMicroseconds(dur);
//    }
//  }
//}
//
//void FM_tx::setVoltage(int vol){
//  dac.setVoltage(vol, false);
//}
//
//void FM_tx::sentHead(){
//  for (int s=0;s<4;s++) //4 sample/cycle
//        {
//            dac.setVoltage(1000, false);//modify amplitude
//            delayMicroseconds(Delay[0]);
//        }
//}
//
//void FM_tx::testDac(){
//  for(int i=0;i<10;i++){
//    dac.setVoltage(0, false);
//  delay(100);
//  dac.setVoltage(1000, false);
//  delay(100);
//  dac.setVoltage(2000, false);
//  delay(100);
//  dac.setVoltage(1000, false);
//  delay(100);
//  }
//
//}
//
//void FM_tx::FullTransmit(String data){
//    char inData[50];
//    int counter = 0;
//      for(int i=0;i<data.length();i++)
//    {
//      inData[i] = data[i];
//      counter++;
//    }
//    for (int s=0;s<4;s++) //4 sample/cycle
//        {
//            dac.setVoltage(1000, false);//modify amplitude
//            delayMicroseconds(Delay[0]);
//        }
//    for (int i=0;i<counter-1;i++) //send data
//    {
//
//      for (int k = 7; k > 0; k -= 2) //send 8 bits from LSB tp MSB
//      {
//        int tmp = inData[i] & 3;
//        int useDelay,cyc;
//        if(tmp == 0)
//        {
//
//          cyc = 1;
//          useDelay = Delay[0];
//        }
//        else if(tmp == 1)
//        {
//
//          cyc = 6;
//          useDelay = Delay[1];
//        }
//        else if(tmp == 2)
//        {
//
//          cyc = 11;
//          useDelay = Delay[2];
//        }
//        else
//        {
//
//          cyc = 16;
//          useDelay = Delay[3];
//        }
//
//        for (int sl=0;sl<cyc;sl++)
//        {
//          for (int s=0;s<4;s++)
//          {
//            dac.setVoltage(S_DAC[s], false);//modify amplitude
//            delayMicroseconds(useDelay);
//          }
//        }
//        inData[i]>>=2;
//      }
//      for (int s=0;s<4;s++) //4 sample/cycle
//        {
//            dac.setVoltage(1000, false);//modify amplitude
//            delayMicroseconds(Delay[0]);
//        }
//    }
//    dac.setVoltage(0, false);

//}

void FM_tx::Transmit(String data) {
  int counter = 0;
  for (int i = 0; i < data.length(); i++)
  {
    inData[i] = data[i];

    counter++;
  }
  //    Serial.println(inp);
  //    Serial.println(counter);
//  for (int s = 0; s < 4; s++) //4 sample/cycle
//  {
//
//    dac.setVoltage(1000, false);//modify amplitude
//    delayMicroseconds(delay0);
//  }
  for (int i = 0; i < counter; i++) //send data
  {
    //char preShifted = inData[i];
    //Serial.write("A");
    for (int k = 7; k > 0; k -= 2) //send 8 bits from LSB tp MSB
    {
      int tmp = inData[i] & 3;
      int useDelay, cyc;
      if (tmp == 0)
      {
        //Serial.println("00");
        cyc = 1;
        useDelay = delay0;
      }
      else if (tmp == 1)
      {
        //Serial.println("01");
        cyc = 6;
        useDelay = delay1;
      }
      else if (tmp == 2)
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

      for (int sl = 0; sl < cyc; sl++)
      {
        for (int s = 0; s < 4; s++)
        {
          dac.setVoltage(S_DAC[s], false);//modify amplitude
          delayMicroseconds(useDelay);
        }
      }
      inData[i] >>= 2;
    }

  }
//  for (int s = 0; s < 4; s++) //4 sample/cycle
//  {
//    dac.setVoltage(1000, false);//modify amplitude
//    delayMicroseconds(delay0);
//  }
  dac.setVoltage(0, false);

}
