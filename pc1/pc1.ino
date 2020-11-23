#include<String.h>
//String inp;
enum STATES {
  START,
  GET_3DATA,
  LAST_STATE
} state = START;
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
void setup() {
  Serial.begin(115200);
  Serial.flush();
  Serial.println("\n======= PROGRAM STARTING =======");
  Serial.print(". ");
  delay(500);
  Serial.print(". ");
  delay(500);
  Serial.println(".");
  delay(500);
  Serial.println("///// Binary Image Capture \\\\\\\\\\");
  Serial.println("Developed by Group No. 8\n\n");

}

void start() {  //Ask user to begin
  while (state == START) {
    Serial.print("Enter \"START\" to begin CAPTURE : ");
    while (!Serial.available()) {
    }
    String inp = Serial.readString();
    Serial.println(inp);
    if (inp == "START") {
      Serial.println("Alright, Let's go!");
      state = GET_3DATA;
    } else {
      Serial.println("Wrong input please try again!\n");
    }
  }
}

void get_image_data3() {
  char startCMD = 's';
  Serial.println("Requesting form PC2 . . .");
  //Send 's' to PC2 to Begin
  /*
  int temp = sendAndWaitAck(&startCMD, 1, 2000);
  delay(300);
  int size = -10;
  while (size <= 0) {
    size = receiveAndSendAck(buff, 3);
  }
  for(int i =0; i<3;i++){
    Serial.println("buff = " + String(buff[i]));
  }
  
  if (size == 3) {
    for (int i = 0; i < 3; i++) {
      pos[i] = buff[i];
    }
    show_img_types();
    state = LAST_STATE;
    delay(300);
  } else {
    Serial.println("Error data lost");
  }
  */
  //Shortcut to next state
  state = LAST_STATE;
}

void last_state() {
  while (state == LAST_STATE) {
    Serial.print("Choose Image ==> ");
/*
    for (int i = 0; i < 3; i++) {
      char key = pos[i];
      String imgType = get_image_type(key);
      String imgPrint = "[" + String(key) + "]->" + imgType + " ";
      Serial.print(imgPrint);
    }
    Serial.println(" [r]->reset program");
    Serial.print("Input : ");
    while (!Serial.available()) {
      receiveAndSendAck(buff, 48);
    }
    char in = Serial.read();
    Serial.println(in);
    if (in == pos[0] or in == pos[1] or in == pos[2] or in == 'r') {
      String sendingStatus = "Sending " + String(in) + " to PC2...";
      Serial.println(sendingStatus);
      sendAndWaitAck(&in, 1, 2000);
      if (in == 'r') {
        Serial.println("reset program");
        Serial.println();
        state = START;
      } else {
        get_points16(in);
      }
    } else if (in == 's') {
      Serial.println("reset program");
      Serial.println();
      state = START;
    } else {
      Serial.println("wrong input please try again!");
      Serial.println();
    }
    */
    //Shortcut to next state
    Serial.println("Enter \'s\' to reset program.");
    while (!Serial.available()) {
    }
    char in = Serial.read();
    if (in == 's') {
      Serial.println("reset program");
      Serial.println();
      state = START;
    }
  }
}

void loop() {
  //Ask user to begin
  if(state == START) {
    start();
  } else if (state == GET_3DATA) {
    get_image_data3();
  } else if (state == LAST_STATE) {
    last_state();
  }
  ////If yes, tell PC2 to begin capture

  ////Wait & Recieve Data from PC2

  ////Display 4 bit binary & angles

  //ASk user to restart or retake
  
  ////if retake ask user for data to retake

  ////tell pc2 which angle to capture

  ////Recieve 20 dots from pc2

  ////Display Data
}
