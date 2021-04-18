// Built-in libraries
#include <Stepper.h>
#include <binary.h>
#include <EEPROM.h>

//7 segment library
#include "SevSeg.h"

SevSeg sevseg;

//Pins a los que está conectado el motor
int motorPin1 = 22;
int motorPin2 = 23; 

int doorLed = 53;
char message[] = "1234";

int upcheck;
int downcheck;
int gencheck;

//Pins de final de carrera
int piso1 = 24;
int piso2 = 25;
int piso3 = 26;
int piso4 = 27;

//pins de botones internos
int b1 = 49;
int b2 = 50;
int b3 = 51;
int b4 = 52;
int abrir = 53;

int piso = 0;
int target = 0;
int dir = 0; //0 es detenido, 1 es arriba, 2 es abajo

char dirChar = ' ';

bool up[4] = {false,false,false,false};
bool down[4] = {false,false,false,false};
bool internal[4] = {false, false, false, false};

void setup() {
  //inicializar pins como output
  pinMode(motorPin1, OUTPUT); 
  pinMode(motorPin2, OUTPUT);  

  byte numDigits = 4;
  byte digitPins[] = {44, 45, 46, 47};
  byte segmentPins[] = {40, 41, 38, 39, 37, 36, 34};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

//  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);

  //Initiate Serial communication.
  Serial.begin(9600);
}

void loop() {
//  Serial.println(digitalRead(piso1));
//  Serial.println(digitalRead(piso2));
//  Serial.println(digitalRead(piso3));
//  Serial.println(digitalRead(piso4));
//  Serial.println(up[0]);
//  Serial.println(up[1]);
//  Serial.println(up[2]);
//  Serial.println(down[1]);
//  Serial.println(down[2]);
//  Serial.println(down[3]);
//  Serial.println(internal[0]);
//  Serial.println(internal[1]);
//  Serial.println(internal[2]);
//  Serial.println(internal[3]);
//  Serial.println("-----------------------------------------------------------------------");
//  Serial.println(digitalRead(8));
//  Serial.println(digitalRead(9));
//  Serial.println(digitalRead(10));
//  Serial.println(digitalRead(11));
//  Serial.println(digitalRead(12));
//  Serial.println(digitalRead(13));
//  Serial.println(digitalRead(b1));
//  Serial.println(digitalRead(b2));
//  Serial.println(digitalRead(b3));
//  Serial.println(digitalRead(b4));
//  Serial.println(digitalRead(abrir));
//  Serial.println("-----------------------------------------------------------------------");

  if(dir == 0) {
    dirChar = ' ';    
  }
  else if(dir == 1) {
    dirChar = 'u';
  }
  else if(dir == 2) {
    dirChar = 'd';
  }

    if(digitalRead(piso1)) {
    piso = 0;
//    sevseg.setChars("abcd");
  }
  else if(digitalRead(piso2)) {
    piso = 1;
//    sevseg.setChars(message);
  }
  else if(digitalRead(piso3)) {
    piso = 2;
//    sevseg.setChars(message);
  }
  else if(digitalRead(piso4)) {
    piso = 3;
//    sevseg.setChars(message);
  }

//  sevseg.refreshDisplay();

  Serial.print("FLOOR: ");
  Serial.print(piso+1);
  Serial.print("\t");
  Serial.print("TARGET: ");
  Serial.print(target+1);
  Serial.print("\t");
  Serial.print("DIR: ");
  Serial.print(dir);
  Serial.println();

  
  if(piso == target) {
    if(dir == 1) {
      up[piso] = false;
      internal[piso] = false;
      stopMove();
      openDoors();
    }
    else if(dir == 2) {
      down[piso] = false;
      internal[piso] = false;
      stopMove();
      openDoors();
    }
    else {
      up[piso] = false;
      down[piso] = false;
      internal[piso] = false;
    }
    dir = 0;
  }


  //marca los botones como prendidos
  if(!digitalRead(8)) {
    up[0] = true;
  }
  if(!digitalRead(9)) {
    down[1] = true;
  }
  if(!digitalRead(10)) {
    up[1] = true;
  }
  if(!digitalRead(11)) {
    down[2] = true;
  }
  if(!digitalRead(12)) {
    up[2] = true;
  }
  if(!digitalRead(13)) {
    down[3] = true;
  }
  if(!digitalRead(b1)) {
    internal[0] = true;
  }
  if(!digitalRead(b2)) {
    internal[1] = true;
  }
  if(!digitalRead(b3)) {
    internal[2] = true;
  }
  if(!digitalRead(b4)) {
    internal[3] = true;
  }
  upcheck = upCheck(piso);
  downcheck = downCheck(piso);
  gencheck = genCheck();

  if(dir == 0) {
    if((gencheck != -1) && (gencheck > piso)) {
      dir = 1;
      target = gencheck;
    }
    else if ((gencheck != -1) && (gencheck < piso)) {
      dir = 2;
      target = gencheck;
    }
  }
  else if (dir == 1){
    if(upcheck != -1) {
      target = upcheck;
    }
  }
  else if (dir == 2){
    if(downcheck != -1) {
      target = downcheck;
    }
  }
  moveDir(dir);
}

void openDoors() {
  digitalWrite(doorLed, HIGH);
  Serial.println("LED");
  delay(1000);
  while(!digitalRead(abrir))
  {
    
  }
  delay(1000);
  digitalWrite(doorLed, LOW);
}


void moveDir(int dir) {
  if(dir == 1) {
    goingUp();
  }
  else if (dir == 2) {
    goingDown();
  }
  else {
    stopMove();
  }
}

int genCheck() {
  for(int i = 0; i < 3; i++) {
    if(internal[i] == 1) {
      return i;
    }
  }
  for(int i = 0; i < 3; i++) {
    if(up[i] == 1) {
      return i;
    }
  }
  for(int i = 3; i > 0; i--) {
    if(down[i] == 1) {
      return i;
    }
  }
  return -1;
}

int upCheck(int piso) {
  for(int i = piso; i <= 3; i++) {
    if(((internal[i] == 1) ||(up[i] == 1)) && (i > piso)) {
      return i;
    }
  }
  return -1;
}

int downCheck(int piso) {
  for(int i = piso; i >= 0; i--) {
    if(((internal[i] == 1) ||(down[i] == 1)) && (i < piso)) {
      return i;
    }
  }
  return -1;
}

void goingUp() {
  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
  digitalWrite(motorPin2, HIGH); //rotates motor
}

void goingDown() {
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin1 LOW
  digitalWrite(motorPin1, HIGH); //rotates motor
}

void stopMove() {
  digitalWrite(motorPin2, LOW);    // set the Pin motorPin1 LOW
  digitalWrite(motorPin1, LOW); //rotates motor
}
