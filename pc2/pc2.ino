#include "FM_Rx.h"
#include "FM_Tx.h"
#include "servo.h"

FM_Rx *receiver;
FM_Tx *transmitter;
servo *director;

uint8_t buff[20];
char angle[3] = {'L', 'C', 'R'};
String bin[3];
//1,27,28,26,27,27,29,27,29,29,28,26,29,27,29,27,199,196,185,176,189
String data[3][21];
uint8_t data_index = 0;

<<<<<<< HEAD
=======

void addChecksum(uint8_t* to, uint8_t* in, uint8_t s) {
  int i;
  int sum = 0;
  to[0] = 2;
  for (i = 0; i < s; i += 2) {
    sum += in[i] * 256;
    to[i+1] = in[i];
    if (i + 1 < s) {
      sum += in[i + 1];
      to[i + 2] = in[i + 1];
    }
    if (sum >= 65536) {
      sum = sum - 65535;
    }
  }
  int comply = 16 * 16 * 16 * 16 - sum - 1;
  to[s+1] = comply / (16 * 16);
  comply %= (16 * 16);
  to[s + 2] = comply;
}

bool checkSum(uint8_t* in, uint8_t s) {
  int i;
  int sum = 0;
  for (i = 1; i <= s+2; i += 2) {
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
  uint8_t out[size_data + 3];
  memset(out, 0, size_data + 3);
  data_pac(out, data, size_data);
  transmitter->sendFrame((char *)out);
  int8_t ack[1]={0};
  int8_t ch= -1;
  while (ch[0] < 0) {
    ch = receiver->receiveFrame(ack);
    if (ch[0]==2 && checkSum(ch)){
      if (ch >= 0&&ack[1] == 'a') {
        return ch;
      }
      else {
        transmitter->sendFrame((char *)out);
      }
    }
    
  }
}

int8_t receiveAndSendAck(uint8_t* data, uint8_t size_data, unsigned long t_out) {
  int8_t ch = receiver->receiveFrame(data, size_data, t_out);
  if (ch>0){
    uint8_t out[4];
    uint8_t ack = 'a';
    memset(out, 0, 4);
    data_pac(out, ack, 1);
    transmitter->sendFrame((char *)out);
  }
  return ch;
}

>>>>>>> 0a33201d459635d8a696faa42ee1b12cc738dc01
char state[3][10] = {"INIT", "WAIT", "SENDING"};
String nowState = "INIT";

void setup()
{
  Serial.begin(115200);
  Serial.flush();

  director = new servo();
  receiver = new FM_Rx(97.5);
  transmitter = new FM_Tx();

  //  transmitter -> setVoltage(0);
  director -> startCam();
  Serial.flush();
}

String *tmp;

void loop()
{

  if (nowState == "INIT") {
    
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
    for(int i=0;i<3;i++){
      transmitter -> fskTransmit(data[i][0]);
      transmitter -> fskTransmit(",");
    }

    Serial.write('S');
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
