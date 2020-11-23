#include "FM_rx.h"

FM_rx::FM_rx(float freq)
{
    sbi(ADCSRA, ADPS2); // this for increase analogRead speed
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);

    Wire.begin();
    radio.setFrequency(freq);
}

int FM_rx::receiveFrame(int timeout)
{
    unsigned long start = millis();
    uint8_t aData[40];
    int cIdx = 0;

    while (millis() - start < timeout)
    {
        uint16_t tmp = analogRead(A2);
        if (tmp - r_slope and prev < r_slope and !check_amp)
        {
            check_amp = true;
            if (!check_baud)
            {
                baud_begin = micros();
                bit_check++;
            }
        }

        if (tmp < r_slope and check_baud)
        {
            if (micros() - baud_begin > baudTime)
            {
                uint16_t last = (int(ceil(float(count - 1) / 5)) & 3) << (bit_check * 2);
                ;
                data |= last;
                baud_check++;
                if (baud_check == 4)
                {
                    //return char(data);
                    aData[cIdx] = data;
                    cIdx++;
                    data = 0;
                    baud_check = 0;
                    bit_check = -1;
                }
                check_baud = false;
                count = 0;
            }
        }
        if (micros() - baud_begin > 40000)
        {
            data = 0;
            baud_check = 0;
            bit_check = -1;
            check_amp = false;
            check_baud = false;
            count = 0;
        }

        if (tmp > r_slope and check_amp)
        {
            count++;
            check_baud = true;
            check_amp = false;
        }
        prev = tmp;
    }
    return aData;
}
