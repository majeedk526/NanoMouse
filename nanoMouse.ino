#include "Sensors.h"
#include "DCMotors.h"

const byte EF = 11, RF = A0;
const byte EL = 13, RL = A2;
const byte ER = 12, RR=A3;
int corL=0, corR=0, cor=0;
int CL=20, CR=50;
byte event = 0;


byte cmd=0;
int frontVal=0;

const byte KEY_FORWARD = '2';
const byte KEY_LEFT = '4';
const byte KEY_RIGHT = '6';
const byte KEY_BACK = '5';
const byte KEY_STOP = '8';

Sensors<EF,RF> sensorFront;
Sensors<EL,RL> sensorLeft;
Sensors<ER,RR> sensorRight;
DCMotors<9,10,8,3,2,4> motors;//<enR, inpR1, inpR2, enL, inpL1, inpL2 >

void setup() {
  // put your setup code here, to run once:
  sensorFront.configure();
  sensorLeft.configure();
  sensorRight.configure();
  motors.configure();
 
  Serial.begin(9600);
}

void loop() {

  event = detect(20);
  btControl();    
  delay(10);
}

byte detect(byte threshold){
  byte event=0;
 // byte threshold = 20;
  corL = sensorLeft.getVal();
  corR = sensorRight.getVal();
  frontVal = 2*sensorFront.getVal();
  Serial.println(frontVal);
  
  if(frontVal > (threshold)){event +=1;}
  if(corL > threshold){event+=2;}
  if(corR > threshold){event+=4;}

  if(corL-corR>threshold){
    cor = map(corL,0,(corL+150),0,CL) *2;  
  } else if(corR-corL>threshold) {
     cor = map(corR,0,(corR+150),0,CR);
     cor*=-1;
  } else {
    cor=0;
  }
  
  return event;
}

bool avoid(byte event){

  switch(event){

    case 1:
      Serial.println("avoid : turning left");
      motors.stopMoving();
      delay(500);
      motors.turnLeft();
      cmd = KEY_FORWARD;
      return true;
      break;

    case 2:
      Serial.println("avoid : cor left");
      cor=30;
      motors.driveForward(cor);
      cmd = KEY_FORWARD;
      return true;

    case 3:
      Serial.println("avoid : turning right");
      motors.stopMoving();
      delay(500);
      motors.turnRight();
      cmd = KEY_FORWARD;
      return true;
      break;

     case 4:
      Serial.println("avoid : cor right");
        cor = -30;
        motors.driveForward(cor);
        cmd = KEY_FORWARD;
        return true;

     case 5:
     Serial.println("avoid : turning left");
       motors.stopMoving();
       delay(500);
      motors.turnLeft();
      cmd = KEY_FORWARD;
      return true;
      break;
        
      case 7:
         motors.driveBack(-cor);
         Serial.println("avoid : back");
         delay(500);
         cmd = KEY_FORWARD;
         return true;
         break;
   }

   return false;
   
}



  void btControl(){
    
      if(Serial.available()){
        cmd = Serial.read();
        Serial.print("cmd : ");
        Serial.println(cmd);

      Serial.print("event : ");
      Serial.println(event);
    }

     if(!avoid(event)){
        if(cmd==KEY_FORWARD){
            motors.driveForward(cor); 
            
        } else if (cmd==KEY_LEFT){
            cmd=KEY_FORWARD;
            motors.turnLeft();
            
        } else if (cmd==KEY_RIGHT){
          cmd=KEY_FORWARD;
          motors.turnRight();
      
        } else if (cmd == KEY_BACK){
            //cmd=KEY_FORWARD;
            motors.driveBack(-cor);
            delay(10);
            motors.turnLeft();
        } else if (cmd == KEY_STOP){
            motors.stopMoving();
            
        }
        delay(10);
      }
}



