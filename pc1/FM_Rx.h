#include <Arduino.h>
#include <Wire.h>
#include <TEA5767Radio.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//edit this number
#define r_slope 300

class FM_Rx
{
  public:
    //    int receiveFrame(int timeout);
    FM_Rx();
    char Receive();

  private:
    int prev = 0;
    int count = 0;

    uint16_t baud_check = 0;
    uint16_t data = 0;
    uint16_t bit_check = -1;

    bool check_amp = false;
    bool check_baud = false;

    uint32_t baud_begin = 0;
    //    int prev = 0;
    //    int count = 0;
    //
    //    uint16_t baud_check = 0;
    //    uint16_t data = 0;
    //    uint16_t bit_check = -1;
    //
    //    bool check_amp = false;
    //    bool check_baud = false;
    //
    //    uint32_t baud_begin = 0;
    //
    //    int baudTime = 10000;
    //    TEA5767Radio radio = TEA5767Radio();
};
