#include<String.h>
#include "FM_tx.h"
#include "FM_Rx.h"
FM_Rx *receiver;
FM_tx *transmitter;

//State
//String[3] = {"INIT","WAIT","SEND"};
String state = "INIT";

//Variables
String temp;
String input;
String tmp;
String frameNo;
String frameData;
String fullData;
int count_frame = 0;
boolean isTimeout = false;
String inp = "";

//Times
long camera_timeout;



//Analyze State
String data[3];
String data2[3];
int data_index = 0;


//void addChecksum(uint8_t* to, uint8_t* in, uint8_t s) {
//  int i;
//  int sum = 0;
//  to[0] = 2;
//  for (i = 0; i < s; i += 2) {
//    sum += in[i] * 256;
//    to[i + 1] = in[i];
//    if (i + 1 < s) {
//      sum += in[i + 1];
//      to[i + 2] = in[i + 1];
//    }
//    if (sum >= 65536) {
//      sum = sum - 65535;
//    }
//  }
//  int comply = 16 * 16 * 16 * 16 - sum - 1;
//  to[s + 1] = comply / (16 * 16);
//  comply %= (16 * 16);
//  to[s + 2] = comply;
//}
//
//bool checkSum(uint8_t* in, uint8_t s) {
//  int i;
//  int sum = 0;
//  for (i = 1; i <= s + 2; i += 2) {
//    sum += in[i] * 256;
//    if (i + 1 < s) {
//      sum += in[i + 1];
//    }
//    if (sum >= 65536) {
//      sum = sum - 65535;
//    }
//  }
//  if (sum == 65535) {
//    return true;
//  }
//  return false;
//}


void setup() {
  Serial.begin(115200);
  Serial.flush();
  receiver = new FM_Rx();
  transmitter = new FM_tx();
  Serial.println("\n======= PROGRAM STARTING =======");
  Serial.println(". ");
  delay(500);
  Serial.println(". ");
  delay(500);
  Serial.println(".");
  delay(500);
  Serial.println("///// Binary Image Capture \\\\\\\\\\");
  Serial.println("    Developed by Group No. 8\n\n");
  Serial.println("================================");

}


void loop() {
  //New Code
  //INIT STATE
  if (state == "INIT") {
    //Variables
    boolean isWaiting = true;
    temp = "";
    data_index = 0;
    input = "";


    //CS
    if (isWaiting) {
      Serial.println("===============================");
      Serial.println("Waiting for PC2 to connect");
      isWaiting = false;
    }


    //NS
    state = "WAIT";
  }


  //WAIT STATE
  if (state == "WAIT") {
    temp = receiver -> Receive();
    if (Serial.available() >0){
      String inp = Serial.readString();
      if (inp == "0"){
      Serial.println();
      Serial.println("===============================");
      Serial.println("Restart");
      Serial.println("===============================");
      Serial.println();
      state = "ORDER";
    }
    }
     
    if (temp[temp.length() - 1] == '~') {
      Serial.println("Connected.");
      Serial.println("===============================");
      Serial.println();
      Serial.println("===============================");
      Serial.println("Data : " + temp);
      Serial.println("Received Data Complete.");
      transmitter -> Transmit("A");    //Send ACK
      Serial.println("Sent ACK Complete.");
      Serial.println("===============================");
      receiver -> Clear();
      Serial.println("Analyzing");
      Serial.println(".");
      Serial.println(".");
      state = "ANALYZE";
    }

    if(input == "1"){
        //Serial.println("Data Reset.");
        data2[0] = "";
      }
      if(input == "2"){
        //Serial.println("Data Reset.");
        data2[1] = "";
      }
      if(input == "3"){
        //Serial.println("Data Reset.");
        data2[2] = "";
      }
      if(input == "4"){
        //Serial.println("Data Reset.");
        data[0] = "";
        data[1] = "";
        data[2] = "";
      }
  }


  //ANALYZE STATE
  if (state == "ANALYZE") {
    //CS
    data_index = 0;
    for (uint8_t i = 0; i < temp.length(); i++) {
      if (temp[i] == ',') {
        data_index++;
      }
      //NS
      else if (temp[i] == '~') {
        Serial.println("Analyzed Complete.");
        Serial.println("===============================");
        delay(1000);
        state = "DISPLAY";
      }
      else {
        data[data_index] += temp[i];
      }
    }
  }



  //DISPLAY STATE
  if (state == "DISPLAY") {
    //CS
    Serial.println("Colors");
    Serial.println();
    Serial.print("Picture 1: ");
    Serial.println(data[0]);
    Serial.print("Picture 2: ");
    Serial.println(data[1]);
    Serial.print("Picture 3: ");
    Serial.println(data[2]);
    Serial.println();
    //NS
    Serial.println("===============================");
    state = "ORDER";
  }


  //ORDER STATE
  if (state == "ORDER") {
    //CS
    Serial.println("-----MENU-----");
    Serial.println("1. Take Left Picture");
    Serial.println("2. Take Middle Picture");
    Serial.println("3. Take Right Picture");
    Serial.println("4. Take All Pictures");
    while (!Serial.available());
    input = Serial.readString();
    if (input == "1") {
      Serial.println();
      Serial.println("===============================");
      Serial.println("Waiting for Respond.");
      Serial.println("===============================");
      Serial.println();
      Serial.println("Taking Left Picture.");
      transmitter -> Transmit("L");
      state = "RECEIVE";
    }
    if (input == "2") {
      Serial.println();
      Serial.println("===============================");
      Serial.println("Waiting for Respond.");
      Serial.println("===============================");
      Serial.println();
      Serial.println("Taking Middle Picture.");
      transmitter -> Transmit("C");
      state = "RECEIVE";
    }
    if (input == "3") {
      Serial.println();
      Serial.println("===============================");
      Serial.println("Waiting for Respond.");
      Serial.println("===============================");
      Serial.println();
      Serial.println("Taking Right Picture.");
      transmitter -> Transmit("R");
      state = "RECEIVE";
    }
    if (input == "4") {
      Serial.println();
      Serial.println("===============================");
      Serial.println("Waiting for Respond.");
      Serial.println("===============================");
      Serial.println();
      Serial.println("Taking All Picture.");
      transmitter -> Transmit("T");
      state = "WAIT";
    }




  }


  //RECEIVE STATE
  if (state == "RECEIVE") {

    //CS
    tmp = receiver -> Receive();
    if (Serial.available() >0){
      String inp = Serial.readString();
      if (inp == "0"){
      Serial.println();
      Serial.println("===============================");
      Serial.println("Restart");
      Serial.println("===============================");
      Serial.println();
      state = "ORDER";
    }
    }
    if (tmp[tmp.length() - 1] == '~') {
      fullData = tmp;
      Serial.println();
      Serial.println("===============================");
      tmp = "";
      Serial.println("Received Data Complete.");
      Serial.println();
      Serial.println("DATA : " + fullData);
      transmitter -> Transmit("A");
      Serial.println("Sent ACK Complete.");
      Serial.println("===============================");
      Serial.println("Analyzing");
      Serial.println(".");
      Serial.println(".");
      receiver -> Clear();
      state = "FULL_ANALYZE";
    }

    if(input == "1"){
        //Serial.println("Data Reset.");
        data2[0] = "";
      }
      if(input == "2"){
        //Serial.println("Data Reset.");
        data2[1] = "";
      }
      if(input == "3"){
        //Serial.println("Data Reset.");
        data2[2] = "";
      }
      if(input == "4"){
        //Serial.println("Data Reset.");
        data[0] = "";
        data[1] = "";
        data[2] = "";
      }
  }


  //FULL_ANALYZE State
  if (state == "FULL_ANALYZE") {
    //CS
    data_index = 0;
    for (uint8_t i = 0; i < fullData.length(); i++) {
      Serial.println(fullData);
      if (fullData[i] == ',') {
        data_index++;
      }
      //NS
      else if (fullData[i] == '~') {
        Serial.println("Analyzed Complete.");
        Serial.println("===============================");
        delay(1000);
        state = "FULL_DISPLAY";
      }
      else {
        if (input == "1") {
          data2[0] += fullData[i];
          data[0] = data2[0];
          data2[1] = data[1];
          data2[2] = data[2];
        }
        if (input == "2") {
          data2[1] += fullData[i];
          data[1] = data2[1];
          data2[0] = data[0];
          data2[2] = data[2];
        }
        if (input == "3") {
          data2[2] += fullData[i];
          data[2] = data2[2];
          data2[0] = data[0];
          data2[1] = data[1];
        }

      }
    }
  }


  //FULL DISPLAY State
  if (state == "FULL_DISPLAY") {
    //CS
    Serial.println("Colors");
    Serial.println();
    Serial.print("Picture 1: ");
    Serial.print(data2[0]);
    Serial.println();
    Serial.print("Picture 2: ");
    Serial.print(data2[1]);
    Serial.println();
    Serial.print("Picture 3: ");
    Serial.print(data2[2]);
    Serial.println();

    //NS
    Serial.println("===============================");
    state = "ORDER";

  }

}
