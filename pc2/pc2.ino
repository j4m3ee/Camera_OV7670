#include "FM_rx.h"
#include "FM_tx.h"
#include "servo.h"

FM_rx *receiver;
FM_tx *transmitter;
servo *director;

uint8_t buff[20];
char angle[3] = {'L', 'C', 'R'};
String bin[3];
//1,27,28,26,27,27,29,27,29,29,28,26,29,27,29,27,199,196,185,176,189
String data[3][21];
uint8_t data_index = 0;

void data_pac(uint8_t* to, uint8_t* in, uint8_t s) {
  int i;
  int sum = 0;
  for (i = 0; i < s; i += 2) {
    sum += in[i] * 256;
    to[i] = in[i];
    if (i + 1 < s) {
      sum += in[i + 1];
      to[i + 1] = in[i + 1];
    }
    if (sum >= 65536) {
      sum = sum - 65535;
    }
  }
  int comply = 16 * 16 * 16 * 16 - sum - 1;
  to[s] = comply / (16 * 16);
  comply %= (16 * 16);
  to[s + 1] = comply;
}

bool err_check(uint8_t* in, uint8_t s) {
  int i;
  int sum = 0;
  for (i = 0; i < s; i += 2) {
    sum += in[i] * 256;
    if (i + 1 < s) {
      sum += in[i + 1];
    }
    if (sum >= 65536) {
      sum = sum - 65535;
    }
  }
  if (sum == 65535) {
    return true;
  }
  return false;
}

char state[3][10] = {"INIT", "WAIT", "SENDING"};
String nowState = "INIT";

void setup()
{
  Serial.begin(115200);
  Serial.flush();

  director = new servo();
  receiver = new FM_rx(97.5);
  transmitter = new FM_tx();

  //  transmitter -> setVoltage(0);
  director -> startCam();



  //  output = createWriter
  Serial.flush();
}

String *tmp;

void loop()
{

  if (nowState == "INIT") {
    transmitter -> Transmit("ABC");
    Serial.write('S');
    //transmitter -> sentFrame("A");
    
    //transmitter -> testDac();
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
    }
    //    for (uint8_t i = 0; i < 3; i++) {
    //      for (uint8_t j = 0; j < 21; j++) {
    //        for (uint8_t n = 0; n < data[i][j].length(); n++) {
    //          Serial.write(data[i][j][n]);
    //          data[i][j][n] = int(data[i][j][n]);
    //        }
    //        Serial.write(' ');
    //
    //      }
    //      delay(500);
    //    }

    //transmitter -> FullTransmit("A");

    

    nowState = "WAIT";
  }
  if (nowState == "WAIT") {
  
    //receiver -> receiveFrame(100);
  }

}
