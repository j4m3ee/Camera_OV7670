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

    void Transmit(uint8_t[],uint8_t);
    void fskTransmit(String data);
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
            int Delay[4];
            uint16_t freq[4];
            uint16_t cycle[4];
            uint16_t S_DAC_fsk[4] = {1000, 2000, 1000, 0};;
    //
    //        void transmit(char data);
    //        void dacSent(uint16_t cyc,int dur);
    //        char packFrame(String data);
    //        void sentHead();
};
