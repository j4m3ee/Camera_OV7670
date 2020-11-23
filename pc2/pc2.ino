#include "FM_rx.h"
#include "FM_tx.h"
#include "servo.h"

FM_rx *receiver;
FM_tx *transmitter;
servo *director;

uint8_t buff[20];
char angle[3] = {'L', 'C', 'R'};
String bin[3];
String data[3][21];
uint8_t data_index = 0;

char state[3][10] = {"INIT", "WAIT", "SENDING"};
String nowState = "INIT";

void setup()
{
  Serial.begin(115200);
  Serial.flush();

  director = new servo();
  receiver = new FM_rx(97.5);
  transmitter = new FM_tx();

  //  output = createWriter
}

String *tmp;

void loop()
{
  if (nowState == "INIT") {
    delay(1000);
    for (uint8_t i = 0; i < 3; i++) {
      bin[i] = director -> Capture(angle[i]);
    }
    for (uint8_t i = 0; i < 3; i++) {
      for (uint8_t j = 0; j < bin[i].length(); j++) {
        if (bin[i][j] == ',') {
          data_index++;
        } else {
          data[i][data_index] += bin[i][j];
        }

      }
      data_index = 0;
      delay(500);
    }
    for (uint8_t i = 0; i < 3; i++) {
      for (uint8_t j = 0; j < 21; j++) {
        for (uint8_t n = 0; n < data[i][j].length(); n++) {
          Serial.write(data[i][j][n]);
        }
        Serial.write(' ');
      }
      delay(500);
    }

    
    nowState = "WAIT";
  }

}
