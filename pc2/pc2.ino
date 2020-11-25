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
uint8_t data[3][22];
uint8_t int8array[3][21];
String tmpData;
uint8_t data_index = 0;
uint8_t idx = 0;
uint8_t maxResent = 5, nowMaxResent = 0;
char tmpOpr[1];

char state[3][10] = {"INIT", "WAIT", "SENDING"};
String nowState = "INIT";

void setup()
{
  Serial.begin(115200);
  Serial.flush();

  director = new servo();
  receiver = new FM_rx(97.5);
  transmitter = new FM_tx();

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
      String tem = "";
      for (uint8_t j = 0; j < bin[i].length(); j++) {
        if (bin[i][j] == ',') {
          data[i][data_index] = tem.toInt();
          data_index++;
          tem = "";
        } else {
          tem += bin[i][j];
        }

      }
      data_index = 0;
    }
    connection();
    delay(500);
    Serial.write('S');
    receiver -> Clear();
    nowState = "WAIT";
  }
  if (nowState == "WAIT") {
    tmpOpr[0] = '-';
    receiver -> Receive(tmpOpr);
    //Serial.write(tmpOpr[0]);

    if (tmpOpr[0] == 'L') {
      receiver -> Clear();
      //transmitter -> Transmit("A");
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
    } else if (tmpOpr[0] == 'C') {
      receiver -> Clear();
      //transmitter -> Transmit("A");
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
    } else if (tmpOpr[0] == 'R') {
      receiver -> Clear();
      //transmitter -> Transmit("A");
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
    else if (tmpOpr[0] == 'T') {
      delay(500);
      transmitter -> Transmit("A", 1);
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
  uint8_t tem[30];
  memset(tem,0,30);
  for (int i = 0; i < 3; i++) {
    tem[i] = data[i][0];
    //Serial.write(int(tem[1]));
    //transmitter -> Transmit(tem, 1);
  }
  tem[3] = 145;
  //Serial.write(tem[1]);
  transmitter -> Transmit(tem, 4);
  Serial.write('w');
  long t = millis();
  while (waitAck()) {
    /*if (millis() - t > 5000) {
      if(nowMaxState == 5){
        Serial.write('l');
        nowState = "WAIT";
        delay(500);
        return;
      }*/
      nowMaxResent++;
      Serial.write('l');
      nowState = "INIT";
      delay(500);
      return;
    //}
  }
  delay(500);
  nowMaxResent = 0;
  Serial.write('A');
}

void sentPackData(uint8_t* data) {
  uint8_t out[23];
  addCheckSum(out, data, 21);
  out[22] = 145;
  //  monitorData(data, 23);
  transmitter -> Transmit(data, 23);
  Serial.write('w');

  long t = millis();
  while (waitAck()) {
    //if (millis() - t > 5000) {
      /*if(nowMaxState == 5){
        Serial.write('l');
        nowState = "WAIT";
        delay(500);
        return;
      }*/
      Serial.write('l');
      nowMaxResent++;
      nowState = "WAIT";
      delay(500);
      //return;
    //}
  }
  nowMaxResent = 0;
  delay(500);
  Serial.write('A');
}

bool waitAck() {
  uint8_t tem[10];
  receiver -> Receive(tem);
  if (tem[0] != 'A') {
    return true;
  } else {
    receiver -> Clear();
    return false;
  }
}

void writeString(String str) {
  for (int i = 0; i < str.length(); i++) {
    Serial.write(str[i]);
  }
}

void flushData() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 21; j++) {
      data[i][j] = "";
    }
  }
}

void addCheckSum(uint8_t* to, uint8_t* in, uint8_t s) {
  int i;
  int sum = 0;
  to[0] = 2;
  for (i = 0; i < s; i ++) {
    sum += in[i];
    to[i + 1] = in[i];
    if (sum >= 256) {
      sum = sum - 255;
    }
  }
  int comply = 16 * 16 - sum - 1;
  to[s + 1] = comply;
}

bool checkSum(uint8_t* in, uint8_t frameSize) {
  int i;
  int sum = 0;
  for (i = 1; i < frameSize; i ++) {
    sum += in[i];
    if (sum >= 256) {
      sum = sum - 255;
    }
  }
  if (sum == 255) {
    return true;
  }
  return false;
}

void dataDepack(uint8_t* out, uint8_t* in, uint8_t frameSize) {
  for (int i = 1; i < frameSize - 1; i++) {
    out[i - 1] = in[i];
  }
}
