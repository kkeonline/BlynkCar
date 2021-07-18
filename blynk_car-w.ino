/* 
 * Remote Control Car (2 wheels) using Blynk App
 * Board NodeMCU V2,V3 or WeMosD1 Mini
 * Motor driver TB6612FNG
 * Blynk App V0,V1 = AnalogJoystick / V2 = Slider
 * by KKEonline
 */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define joy1Pin V0
#define joy2Pin V1
#define finetunePin V2

int STBY = D0; //standby

//Motor A
int AIN1 = D3; //Direction
int AIN2 = D4; //Direction
int PWMA = D5; //Speed control 

//Motor B
int BIN1 = D6; //Direction
int BIN2 = D7; //Direction
int PWMB = D8; //Speed control

int driveX = 0;
int driveY = 0;
int finetune = 0;
int gap = 10;
int diff = 0;
int motorL = 0;
int motorR = 0;

char auth[] = "abcdefabcdefabcdefabcdef";
char ssid[] = "SSID";
char pass[] = "PASSWD";

void checkLimit() {
  if (motorL > 1024) { motorL = 1024; }
  if (motorR > 1024) { motorR = 1024; }
  if (motorL < 0) { motorL = 0; }
  if (motorR < 0) { motorR = 0; }
}

void calc() {
  if ((driveX < gap) and (driveX > -gap)) {
    diff = finetune;
  } else {
    diff = finetune + driveX;
  }
  if ((driveY < gap) and (driveY > -gap)) {
     stop();
  } else if (driveY > 0) {
    //forward
    motorL = driveY + diff;
    motorR = driveY - diff;
    checkLimit();
    move(1, motorR, 1);
    move(2, motorL, 1);
  } else {
    //backward
    motorL = -driveY - diff;
    motorR = -driveY + diff;
    checkLimit();
    move(1, motorR, 0);
    move(2, motorL, 0);
  }
}

BLYNK_WRITE(finetubePin) {
  finetune = param.asInt();
  calc();
}

BLYNK_WRITE(joy1Pin) {
  driveY = param.asInt();
  calc();
}

BLYNK_WRITE(joy2Pin) {
  driveX = param.asInt();
  calc();
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }

  digitalWrite(STBY, HIGH); //disable standby
}

void stop(){
  digitalWrite(STBY, LOW);
}

void setup()
{
  Serial.begin(115200);
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  stop();
}

void loop()
{
  Blynk.run();
}
