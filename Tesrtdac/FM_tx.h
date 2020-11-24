#include <Arduino.h>

#include<Wire.h>
#include<Adafruit_MCP4725.h>
#include<Adafruit_ADS1015.h>

#define defaultFreq 1700
#define num_Delay 4
#define num_S_Dac 4
#define num_Freq 4
#define num_cycle 4
#define f_diff 500

class FM_tx
{
    public:
        FM_tx();
        void sentFrame(String data);
        void setVoltage(int vol);
        void FullTransmit(String data);
        void testDac();
    private:
        Adafruit_MCP4725 dac;
        int Delay[num_Delay];
        uint16_t freq[num_Freq];
        uint16_t cycle[num_cycle];
        uint16_t S_DAC[num_S_Dac];

        void transmit(char data);
        void dacSent(uint16_t cyc,int dur);
        char packFrame(String data);
        void sentHead();
};
