#include "FM_Tx.h"

FM_Tx::FM_Tx()
{
  dac.begin(0x62);
  uint16_t S_DAC[num_S_Dac] = {0, 1000, 2000, 1000};
  uint16_t cycle[num_cycle] = {1, 6, 11, 16};
  for (int i = 0; i < num_Freq; i++)
  {
    freq[i] = 100 + (i * f_diff);
  }
  for (int i = 0; i < num_Delay; i++)
  {
    Delay[i] = (1000000 / freq[i] - 1000000 / defaultFreq) / 4;
  }
}

void FM_Tx::sentFrame(char data[])
{
  uint16_t len = strlen(data);
  for (uint16_t i = 0; i < len; i++)
  {
    transmit(data[i]);
  }
}

char FM_Tx::packFrame(char data[])
{
  uint16_t len = strlen(data);
  //Flag : ~ [1 bytes]
  //Frame seq : 0/1 [1 bit]
  return 'a';
}

void FM_Tx::transmit(char data)
{
  int tmp;
  for (int i = 7; i > 0; i -= 2)
  {
    tmp = data & 3;
    dacSent(cycle[tmp], Delay[tmp]);
    data >>= 2;
  }
}

void FM_Tx::dacSent(uint16_t cyc, int dur)
{
  for (uint16_t c = 0; c < cyc; c++)
  {
    for (uint16_t i = 0; i < 4; i++)
    {
      dac.setVoltage(S_DAC[i], false);
      delayMicroseconds(dur);
    }
  }
}
