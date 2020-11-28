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
    FM_Rx();
    String Receive();
    void Clear();

  private:
    int prev = 0;
    int count = 0;

    uint16_t baud_check = 0;
    uint16_t data = 0;
    uint16_t bit_check = -1;

    bool check_amp = false;
    bool check_baud = false;

    uint32_t baud_begin = 0;

    String output_data;
};
