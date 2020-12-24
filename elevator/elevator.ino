// Built-in libraries
#include <Stepper.h>
#include <binary.h>
#include <EEPROM.h>

//Pins a los que está conectado el motor
int motorPin1 = 12;
int motorPin2 = 13; 

int upcheck;
int downcheck;
int gencheck;

int piso = 0;
int height = 0;
int target = 0;
int dir = 0; //0 es detenido, 1 es arriba, 2 es abajo

//cppQueue up(sizeof(int), 4, IMPLEMENTATION);
//cppQueue down(sizeof(int), 4, IMPLEMENTATION);

bool up[4] = {false,false,false,false};
bool down[4] = {false,false,false,false};

void setup() {
  //inicializar pins como output
  pinMode(motorPin1, OUTPUT); 
  pinMode(motorPin2, OUTPUT);  

  //Initiate Serial communication.
  Serial.begin(9600);
}

void loop() {
//  Serial.println(up[0]);
//  Serial.println(up[1]);
//  Serial.println(up[2]);
//  Serial.println(down[1]);
//  Serial.println(down[2]);
//  Serial.println(down[3]);
//  Serial.println("-----------------------------------------------------------------------");

  delay(10);

  if(height == 0) {
    piso = 0;
  }
  else if(height == 3000) {
    piso = 1;
  }
  else if(height == 6000) {
    piso = 2;
  }
  else if(height == 9000) {
    piso = 3;
  }

  Serial.print("FLOOR: ");
  Serial.print(piso);
  Serial.print("\t");
  Serial.print("TARGET: ");
  Serial.print(target);
  Serial.print("\t");
  Serial.print("HEIGHT: ");
  Serial.print(height);
  Serial.println();
  
  if(piso == target) {
    if(dir == 1) {
      up[piso] = 0;
      stopMove();
      delay(5000);
    }
    else if(dir == 2) {
      down[piso] = 0;
      stopMove();
      delay(5000);
    }
    else {
      up[piso] = 0;
      down[piso] = 0;
    }
    dir = 0;
  }

  //marca los botones como prendidos
  if(!digitalRead(2)) {
    up[0] = true;
  }
  if(!digitalRead(3)) {
    down[1] = true;
  }
  if(!digitalRead(4)) {
    up[1] = true;
  }
  if(!digitalRead(5)) {
    down[2] = true;
  }
  if(!digitalRead(6)) {
    up[2] = true;
  }
  if(!digitalRead(7)) {
    down[3] = true;
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

void moveDir(int dir) {
  if(dir == 1) {
    goingUp();
    height += 10;
  }
  else if (dir == 2) {
    goingDown();
    height -= 10;
  }
  else {
    stopMove();
  }
}

int genCheck() {
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
  for(int i = piso; i < 3; i++) {
    if((up[i] == 1) && (i > piso)) {
      return i;
    }
  }
  return -1;
}

int downCheck(int piso) {
  for(int i = piso; i > 0; i--) {
    if((down[i] == 1) && (i < piso)) {
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

//void goingUp(int distance) {
//  digitalWrite(motorPin1, LOW);    // set the Pin motorPin1 LOW
//  digitalWrite(motorPin2, HIGH); //rotates motor
//  delay(distance); //waits
//  digitalWrite(motorPin2, LOW);    // set the Pin motorPin2 LOW
//}
//
//void goingDown(int distance) {
//  digitalWrite(motorPin2, LOW);    // set the Pin motorPin1 LOW
//  digitalWrite(motorPin1, HIGH); //rotates motor
//  delay(distance); //waits
//  digitalWrite(motorPin1, LOW);    // set the Pin motorPin2 LOW
//}
