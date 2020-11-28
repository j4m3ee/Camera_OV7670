#include "FM_rx.h"
#include "FM_tx.h"
#include "servo.h"

FM_rx *receiver;
FM_tx *transmitter;
servo *director;

uint8_t buff[20];
char angle[3] = {'L', 'C', 'R'};
String bin[3];
//0010,27,28,26,27,27,29,27,29,29,28,26,29,27,29,27,199,196,185,176,189
String data[3][21];
String tmpData;
uint8_t data_index = 0;
uint8_t idx = 0;
char tmpOpr;

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
  Serial.flush();
}

String *tmp;

void loop()
{

  if (nowState == "INIT") {

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
    connection();
    delay(500);
    Serial.write('S');
    receiver -> Clear();

    //    writeString();
    nowState = "WAIT";
  }
  if (nowState == "WAIT") {
    tmpOpr = receiver -> Receive()[0];
        //Serial.write(tmpOpr);

    if (tmpOpr == 'L') {
      receiver -> Clear();
      //transmitter -> fskTransmit("A");
      bin[0] = director -> Capture(angle[idx]);
      idx = 0;
      data[idx][data_index] = "";
      for (uint8_t j = 0; j < bin[idx].length(); j++) {
        if (bin[idx][j] == ',') {
          data_index++;
        } else {
          data[idx][data_index] += bin[idx][j];
        }
      }
      data_index = 0;

      tmpData = bin[idx];
      nowState = "SENDING";
    } else if (tmpOpr == 'C' or tmpOpr == 'B') {
      receiver -> Clear();
      
      //transmitter -> fskTransmit("A");
      idx = 1;
      data[idx][data_index] = "";
      bin[idx] = director -> Capture(angle[idx]);
      for (uint8_t j = 0; j < bin[idx].length(); j++) {
        if (bin[idx][j] == ',') {
          data_index++;
        } else {
          data[idx][data_index] += bin[idx][j];
        }
      }
      data_index = 0;

      tmpData = bin[idx];
      nowState = "SENDING";
    } else if (tmpOpr == 'R') {
      receiver -> Clear();
      //transmitter -> fskTransmit("A");
      idx = 2;
      bin[idx] = director -> Capture(angle[idx]);
      data[idx][data_index] = "";
      for (uint8_t j = 0; j < bin[idx].length(); j++) {
        
        if (bin[idx][j] == ',') {
          data_index++;
        } else {
          data[idx][data_index] += bin[idx][j];
        }
      }
      data_index = 0;

      tmpData = bin[idx];
      nowState = "SENDING";
    }
    else if (tmpOpr == 'T') {
      delay(500);
//      transmitter -> fskTransmit("A");
      flushData();
      nowState = "INIT";
    }
    receiver -> Clear();
    //receiver -> receiveFrame(100);
  }

  if (nowState == "SENDING") {
    sentPackData(data[idx][0]);
    delay(500);
    Serial.write('z');
    delay(500);
    tmpData = "";
    Serial.write('S');
    nowState = "WAIT";

  }
}

void connection() {
  for (int i = 0; i < 3; i++) {
    transmitter -> fskTransmit(data[i][0]);
    if (i != 2) {
      transmitter -> fskTransmit(",");
    }
  }
  transmitter -> fskTransmit("~");
  Serial.write('w');
  long t = millis();
  while (waitAck()) {
    if (millis() - t > 5000) {
      Serial.write('l');
      nowState = "WAIT";
      delay(500);
      return;      
    }
  }
  delay(500);
  Serial.write('A');
}

void sentPackData(String data) {
  data += "~";
  transmitter -> fskTransmit(data);
  Serial.write('w');

  long t = millis();
  while (waitAck()) {
    if (millis() - t > 5000) {
      Serial.write('l');
      tmpOpr = '-';
      nowState = "WAIT";
      delay(500);
      return;   
    }
  }
  delay(500);
  Serial.write('A');
}

bool waitAck() {
  if (receiver -> Receive()[0] != 'A') {
    return true;
  } else {
    receiver -> Clear();
    return false;
  }
}

void flushData() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 21; j++) {
      data[i][j] = "";
    }
  }
}

String addCheckSum(String in) {
  String to;
  int s = in.length();
  char tem[s];
  in.toCharArray(tem,s);
  int i;
  int sum = 0;
  to = 2 + in;
  for (i = 0; i < s; i ++) {
    sum += tem[i];
    if (sum >= 256) {
      sum = sum - 255;
    }
  }
  char comply = 16 * 16 - sum - 1;
  to += comply;
  return to;
}

String dataDepack(String in){
  String out;
  uint8_t frameSize = in.length();
  char tem[frameSize];
  in.toCharArray(tem,in.length());
  for(int i=1;i<frameSize-1;i++){
    out += tem[i];
  }
  return out;
}
