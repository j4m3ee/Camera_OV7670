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

void addChecksum(uint8_t* to, uint8_t* in, uint8_t s) {
  int i;
  int sum = 2;
  to[0] = 2;
  for (i = 0; i < s; i += 2) {
    sum += in[i] * 256;
    to[i + 1] = in[i];
    if (i + 1 < s) {
      sum += in[i + 1];
      to[i + 1 + 1] = in[i + 1];
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

bool checkSum(uint8_t* in, uint8_t s) {
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

int8_t sendAndWaitAck(uint8_t* data, uint8_t size_data, unsigned long t_out) {
  uint8_t out[size_data + 2];
  memset(out, 0, size_data + 2);
  data_pac(out, data, size_data);
  transmitter->sendFrame((char *)out);
  int8_t ack[] = {'a'};
  int8_t ch[40] = { -1};

  while (ch[0] < 0) {
    ch = receiveFrame(1000);
    if (ch >= 0) {
      return ch;
    }
    else {
      transmitter->sendFrame((char *)out);
    }
  }
}

int8_t receiveAndSendAck(uint8_t* data, uint8_t size_data, unsigned long t_out) {
  //int8_t ch = receiveFrame(data, size_data, 1000);

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

<<<<<<< HEAD
=======
    
>>>>>>> a7a29de62801e046d30255be5d3b39f3a6e402a0

    nowState = "WAIT";
  }
  if (nowState == "WAIT") {
  
    //receiver -> receiveFrame(100);
  }


}
