#include <Arduino.h>

#include<Wire.h>
#include<Adafruit_MCP4725.h>
#include<Adafruit_ADS1015.h>

#define defaultFreq 1700
#define f0 100
#define f1 600
#define f2 1100
#define f3 1650



class FM_tx
{
  public:

    void Transmit(String data);
    FM_tx();
    //        void sentFrame(String data);
    //        void setVoltage(int vol);
    //        void FullTransmit(String data);
    //        void testDac();
  private:
    Adafruit_MCP4725 dac;
    char inData[30];
    const uint16_t S_DAC[4] = {0, 1000, 2000, 1000};  // 10 bits input

    int delay0, delay1, delay2, delay3;
    //        int Delay[num_Delay];
    //        uint16_t freq[num_Freq];
    //        uint16_t cycle[num_cycle];
    //        uint16_t S_DAC[num_S_Dac];
    //
    //        void transmit(char data);
    //        void dacSent(uint16_t cyc,int dur);
    //        char packFrame(String data);
    //        void sentHead();
};
