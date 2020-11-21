#include "FM_rx.h"

FM_rx::FM_rx(float freq)
{
    sbi(ADCSRA, ADPS2); // this for increase analogRead speed
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
    Serial.begin(115200);
    Serial.flush();

    Wire.begin();
    radio.setFrequency(freq);
}
