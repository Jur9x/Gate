//current state
#include <IRremote.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

int spinacZavreno = 10;
int spinacOtevreno = 11;

int stavOtevreSe = 0;
int stavZavreSe = 1;

byte speed = 255;

unsigned long kodStartu = 3389572121;
unsigned long kodStopu = 3439970883;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(10, INPUT);//spinca
  pinMode(11, INPUT);//spinac
  pinMode(2, OUTPUT);//motor
  pinMode(3, OUTPUT);//motor
  pinMode(8, INPUT);//IR senzor
}

void startMotor(){ //nevim na jakou stranu se jak toci//
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  /*if(stavOtevreSe = 1){   ^^^^ kod na test h mustku
    analogWrite(5, speed); real kod
    analogWrite(6, 0);
  }
  if(stavZavreSe = 1){
    analogWrite(6, speed);
    analogWrite(5, 0);
  }*/
}

void stopMotor(){
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

void loop(){
  if(spinacZavreno == HIGH){
    //prednastavi stav brany
    stavOtevreSe = 1;
    stavZavreSe = 0;
  }
  if(spinacOtevreno == HIGH){
    //prednastavi stav brany
    stavOtevreSe = 0;
    stavZavreSe = 1;
  }
  if (irrecv.decode(&results)){             //
        Serial.println(results.value, DEC); // Precte signal
        irrecv.resume();                    //
  }
  if (results.value == kodStartu){
    startMotor();
  }
  if (results.value == kodStopu || digitalRead(10) == HIGH || digitalRead(11) == HIGH || digitalRead(8) == LOW) { //10 a 11 jsou spinace na koncich koleje
    stopMotor();
  }
}
