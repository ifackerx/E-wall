#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Servo.h"
#include "LedControl.h"
byte hf[8]= {B00111100,B01000010,B10100101,B10000001,B10100101,B10011001,B01000010,B00111100};
byte nf[8]={B00111100, B01000010,B10100101,B10000001,B10111101,B10000001,B01000010,B00111100};
byte sf[8]= {B00111100,B01000010,B10100101,B10000001,B10011001,B10100101,B01000010,B00111100};
SoftwareSerial mySoftwareSerial(10, 11);
Servo head;
Servo base;
DFRobotDFPlayerMini sound;
int  mood=1, command=0;
char wheel;
long duration,cm,inches;
unsigned long timer;
int enA=9,enB=3,in1=8,in2=7,in3=4,in4=2,trig=6,echo=5;
LedControl lc=LedControl(13,12,A0,2);//DIN,CLK,CS,num of display
void setup() {
  //pin setup
  pinMode(in4,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  timer = millis();
  
  //servo setup
  head.attach(A1);
  base.attach(A2);
  head.write(90);
  base.write(90);
  
  //led setup
  lc.shutdown(0,false); //start display
  lc.setIntensity(0,0);// Set brightness
  lc.clearDisplay(0);   // Clear the display
  lc.shutdown(1,false);
  lc.setIntensity(1,0);
  lc.clearDisplay(1);
  
  //sd card setup
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!sound.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  sound.volume(25);  //Set volume value. From 0 to 30
} 
void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    command = Serial.read(); // Reads the data from the serial port
 }
  if (millis() - timer >= 5000){//timer of mood 
    timer = millis();
    mood--;
    Serial.println(mood);
  }
  if (command == '1') {
    mood++;
    command = 0;
  }

  if (mood <= 0) {
    mood = 0;
    sad();
    sound.play(4);
   
  }
  else if(mood==1){
    normal();
    sound.play(1);
  }
  else if(mood==2){
    happy();
    sound.play(2);
    }
  else if(mood>=3){
    //angry
    sound.play(3);
    mood=3;
    }
  sonic();
  delay(250);
  Serial.println(mood);

  if (wheel == 'f') {
  digitalWrite(in4,HIGH);
  digitalWrite(in3,LOW);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA,255);
  Serial.println("'f'");
  wheel = 0;
 }
 else if (wheel == 'b') {
  digitalWrite(in4,LOW);
  digitalWrite(in3,HIGH);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(enA,255);
  Serial.println("'b'");
  wheel = 0;
 }
  else if (wheel == 'r') {
  digitalWrite(in4,LOW);
  digitalWrite(in3,LOW);
  analogWrite(enB,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA,255);
  Serial.println("'r'");
  wheel = 0;
 } 
  else if (wheel == 'l') {
  digitalWrite(in4,HIGH);
  digitalWrite(in3,LOW);
  analogWrite(enB,255);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  analogWrite(enA,255);
  Serial.println("'l'");
  wheel = 0;
 } 
}
// use to find distance
void sonic(){
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  // convert the time into a distance
  cm = (duration/2) / 29.1;
  if (cm<10){
  wheel = 'l';
  }
  else if (cm>=10){
  wheel = 'f';
  }
  Serial.println();
  
}
//check sd card detail
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
void happy(){
  for(int i=0;i<8;i++){
      lc.setColumn(0,i,hf[i]);
      lc.setColumn(1,i,hf[i]);
    }
  }
  
void sad(){
  for(int i=0;i<8;i++){
      lc.setRow(0,i,sf[i]);
      lc.setRow(1,i,sf[i]);
    }
  }
  
void normal(){
  for(int i=0;i<8;i++){
      lc.setColumn(0,i,nf[i]);
    }
  }

