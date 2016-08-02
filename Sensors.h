#include <Arduino.h>

template <byte em, byte re > //emitter, receiver

class Sensors{

int actualVal = 0;
int ambientVal = 0;
int combinedVal = 0;

  public:

  void configure(){
    pinMode(em, OUTPUT);
    pinMode(re, INPUT);
  }

  int getVal(){
    ambientVal = getAmbient(); 
    combinedVal = getCombined();
    actualVal = -1 * (combinedVal - ambientVal);
    //actualVal = map(actualVal,0,400,0,255);
    return actualVal;
  }

  void view(){

    ambientVal = getAmbient();  
    Serial.print(ambientVal);
    Serial.print("\t");

    combinedVal = getCombined();
    Serial.print(combinedVal);
    Serial.print("\t");

    actualVal = -1 * (combinedVal - ambientVal);
    Serial.println(actualVal);
  }


  private:

  int getAmbient(){
    digitalWrite(em, LOW);
    delay(1);
    int amb =0 ;
    for(int i=0; i<5; i++){
      amb+= analogRead(re);
      delay(1);
    }
      return (amb/5);
  }

  int getCombined(){
    digitalWrite(em, HIGH);
    delay(1);
    int comb =0 ;
    for(int i=0; i<5; i++){
      comb+= analogRead(re);
      delay(1);
    }
    return (comb/5);
  }
};

