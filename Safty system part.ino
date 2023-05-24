#include <Wire.h>
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

String command;

#define REL1 8
#define REL2 9
#define REL3 10
#define REL4 11

int minVal=265;
int maxVal=402;

double x;
double y;
double z;

int trigger_pin1 = 5;
int echo_pin1 = 4;
int trigger_pin2 = 7;
int echo_pin2 = 6;
int trigger_pin3 = 3;
int echo_pin3 = 2;
int buzzer_pin = 13; 
int time;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(REL1, OUTPUT);
  pinMode(REL2, OUTPUT);
  pinMode(REL3, OUTPUT);
  pinMode(REL4, OUTPUT);

  pinMode (trigger_pin1, OUTPUT); 
  pinMode (echo_pin1, INPUT);
  pinMode (trigger_pin2, OUTPUT); 
  pinMode (echo_pin2, INPUT);
  pinMode (trigger_pin3, OUTPUT); 
  pinMode (echo_pin3, INPUT);
  pinMode (buzzer_pin, OUTPUT);
  delay(2000);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.println("Type Command (white, blue, red, all, off)");
}

void loop() {
  int ultra1 = ultra(trigger_pin1,echo_pin1);
  delay(50);
  int ultra2 = ultra(trigger_pin2,echo_pin2);
  delay(50);
  int ultra3 = ultra(trigger_pin3,echo_pin3);
  delay(50);
  
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println(command);
    if (command.equals("1010")) {//forward
      digitalWrite(REL1, HIGH);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, HIGH);
      digitalWrite(REL4, LOW);
    }

    else if (command.equals("0101")) {//backward
      digitalWrite(REL1, LOW);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);
      delay(20);
      digitalWrite(REL1,LOW );
      digitalWrite(REL2, HIGH);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, HIGH);
      delay(2000);
      digitalWrite(REL1, LOW);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);
    }
    else if (command.equals("1000")) {//left
      digitalWrite(REL1, LOW);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);
      delay(20);
      digitalWrite(REL1, HIGH);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);

    }
    else if (command.equals("0010")) {//right
      digitalWrite(REL1, LOW);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);
      delay(20);
      digitalWrite(REL1, LOW);
     digitalWrite(REL2, LOW);
      digitalWrite(REL3, HIGH);
      digitalWrite(REL4, LOW);
    }  
    else if (command.equals("0000")) {
      digitalWrite(REL1, LOW);
      digitalWrite(REL2, LOW);
      digitalWrite(REL3, LOW);
      digitalWrite(REL4, LOW);
    }
    else {
      Serial.println("bad command");
    }
    Serial.print("Command: ");
    Serial.println(command);
  }


  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
 AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);


  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  Serial.print("AngleX= ");
  Serial.println(x);

  Serial.print("AngleY= ");
  Serial.println(y);

  Serial.print("AngleZ= ");
 Serial.println(z);
  Serial.println("-----------------------------------------");
  delay(400);

  if ((ultra1<50 && ultra1>=0) || (ultra2<50 && ultra2>=0) || (ultra3<10 && ultra3>=0 && command.equals("0101") )){
   Serial.print (" Distance= ");              
    Serial.println (distance);        
    tone(buzzer_pin,1000,500);
    delay(200);
    noTone(buzzer_pin);
    delay(200);
    digitalWrite(REL1, LOW);
    digitalWrite(REL2, LOW);
    digitalWrite(REL3, LOW);
    digitalWrite(REL4, LOW);
 }

  else {

    Serial.print (" Distance= ");              
    Serial.println (distance);
  }

}


int ultra(int trigger_pin,int echo_pin){
  digitalWrite (trigger_pin, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigger_pin, LOW);

  time = pulseIn (echo_pin, HIGH);
  distance = (time * 0.034) / 2;
  return distance;
}


