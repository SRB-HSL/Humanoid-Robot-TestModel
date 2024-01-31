#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Time.h>
/* 
  servoID 
  0 -> TR
  1 -> MR
  2 -> DR
  3 -> TL
  4 -> ML
  5 -> DL
*/
#define TR 0
#define MR 1
#define DR 2
#define TL 3
#define ML 4
#define DL 5
#define RX 11
#define TX 12
#define LEDPin 14

int meanPitchArray[10] = {0,0,0,0,0,0,0,0,0,0};

// make virtual usart
SoftwareSerial gySerial(RX, TX);
// gy25 degree & buf
float Roll,Pitch,Yaw;
int counter;
int fc = 40;//tokhmi tarif krde
bool isFc = true;//tokhmi
int befTime;
unsigned char readBuf[8];//data GY
// make servos
Servo myservo[6];  

// def functions
void readGy();
void setServo(char servoID, int degree);
void setBalance(int pitch);
int gyMeanPitch();

void setup() {
  // setup main serial port
  Serial.begin(9600);
  // setup gy serial port
  gySerial.begin(115200);

  delay(500);

  // calibrate gy25 
  gySerial.write(0XA5); 
  gySerial.write(0X55);
  // time for setup gy25
  delay(4000);

  // make output & attach all servo pin
  for(int i = 0; i < 6; i++){
    pinMode(i+2,  OUTPUT);
    myservo[i].attach(i+2);
  }

  //LEDPin
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, 1);
}

void loop() {
  //set all servo on 90 degree
  // for(int i = 0; i < 6; i++){
  //   setServo(i, 0);
  // }
  if(millis() - befTime > 40){
    setServo(TR, fc);
    setServo(MR, -fc);
    // setServo(DR, 0);
    setServo(TL, -fc);
    setServo(ML, fc);
    // setServo(DL, 0);
    if(isFc) fc --;
    else fc ++;
    if(fc < -40 || fc > 40) isFc = !isFc; 
    befTime = millis();
  }
  // read gy degrees
  readGy();
  // setServo(DR, 0);

  // Serial.print("roll= ");
  // Serial.print(Roll);
  // Serial.print(" pitch= "); 
  // Serial.print(Pitch);
  // Serial.print(" yaw= "); 
  // Serial.println(Yaw);

  // delay(100);
}

void setBalance(int pitch){
  setServo(DL, -pitch);
  setServo(DR, pitch);
}

void setServo(char servoID, int degree){
  if(servoID == MR || servoID == TL || servoID == DL) degree = -degree;
  else if(servoID == TR) degree += 20;
  myservo[servoID].write(degree + 90);
}

void readGy(){
  gySerial.write(0XA5); 
  gySerial.write(0X51);//send it for each read
  while (gySerial.available()) {   
    readBuf[counter]=(unsigned char)gySerial.read();
    if(counter==0&&readBuf[0]!=0xAA) return;        
    counter++;       
    if(counter==8)  // package is complete              
    {    
      counter=0;                  
      if(readBuf[0]==0xAA && readBuf[7]==0x55)  // data package is correct      
      {          
        Yaw=(int16_t)(readBuf[1]<<8|readBuf[2])/100.00;   
        Pitch=(int16_t)(readBuf[3]<<8|readBuf[4])/100.00;
        Roll=(int16_t)(readBuf[5]<<8|readBuf[6])/100.00;
        int readPitch = gyMeanPitch();
        setBalance(readPitch);
        Serial.println(readPitch);
        
      }      
    }
  }
}


int gyMeanPitch(){
  for(int i = 8;i > 0;i -= 1){
    meanPitchArray[i+1] = meanPitchArray[i];
  }
  meanPitchArray[0] = Pitch;

  int sum = 0;

  for(int l = 0;l < 10;l += 1){
    sum = sum + meanPitchArray[l];
  }
  return sum / 10;
}


