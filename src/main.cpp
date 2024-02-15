#include <Servo.h>
#include <Arduino.h>

#define TR 0
#define MR 1
#define DR 2
#define TL 3
#define ML 4
#define DL 5
#define RX 11
#define TX 12
#define LEDPin 13

int deg = 0;
int mochDeg = 15;

Servo myservo[6];

void setServo(int servoID, int degree);

void setup()
{
  for(int i = 0; i < 6; i++){
    pinMode(i+2,  OUTPUT);
    myservo[i].attach(i+2);
  }

  //LEDPin
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, 1);

  for(int i = 0;i < 6;i++){
    setServo(i,deg);
  }
}

void loop(){
  while(mochDeg <= 15){
    setServo(DR,-1 * mochDeg);
    setServo(DL,mochDeg);
    mochDeg ++;
    delay(10);
  }
  delay(1000);
  while (deg <= 20)
  {
    setServo(TR,-deg);
    setServo(TL,deg);
    setServo(MR,deg);
    setServo(ML,-deg);
    deg ++;
    delay(40);
  }
  delay(1000);
  while(mochDeg >= 0){
    setServo(DR,-1 * mochDeg);
    setServo(DL,mochDeg);
    mochDeg --;
    delay(10);
  }
  delay(1000);
  while(mochDeg >= -15){
    setServo(DR,-1 * mochDeg);
    setServo(DL,mochDeg);
    mochDeg --;
    delay(10);
  }
  delay(1000);
  while (deg >= -20)
  {
    setServo(TR,-deg);
    setServo(TL,deg);
    setServo(MR,deg);
    setServo(ML,-deg);
    deg --;
    delay(40);
  }
  delay(1000);
  while(mochDeg <= 0){
    setServo(DR,-1 * mochDeg);
    setServo(DL,mochDeg);
    mochDeg ++;
    delay(10);
  }
  delay(1000);
}

void setServo(int servoID, int degree){
  if(servoID == MR || servoID == TL || servoID == DL) degree = -degree;
  myservo[servoID].write(degree + 90);
}