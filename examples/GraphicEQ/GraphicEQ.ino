/*
* MSGEQ7 Graphic EQ Example
* Reads Amplitude at 160Hz and Maps it to a 3 LED EQ
*
*Wiring:
*MSGEQ7
*Strobe: D2
*Reset:  D3
*Output: A0
*LED's
*Green:  D9
*Yellow: D10
*Red:    D11
*
* By: Joseph Diaz
*/

#include <MSGEQ7.h>

#define redLED 11
#define yellowLED 10
#define greenLED 9

#define resetPin 3
#define strobePin 2
#define outputPin 0

#define NOP __asm__ __volatile__ ("nop\n\t")

MSGEQ7 eqOne(resetPin,strobePin,outputPin);

void setup(){
  Serial.begin(9600);
  Serial.print("Offset: ");
  Serial.println(eqOne.detectOffset());
  
  eqOne.correctOffset(true);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  
  pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
}

int value;
void loop(){
  value =eqOne.read160();
  if(value>510){
    analogWrite(redLED,value-510);
    analogWrite(yellowLED,255);
    analogWrite(greenLED,255);
  }
  else if(value>255){
    analogWrite(redLED,0);
    analogWrite(yellowLED,value-255);
    analogWrite(greenLED,255);
  }
  else{
    analogWrite(redLED,0);
    analogWrite(yellowLED,0);
    analogWrite(greenLED,value);
  }
}



