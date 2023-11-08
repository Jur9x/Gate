//current state
#include <IRremote.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long kodStartu = 3389572121;
unsigned long kodStopu = 3439970883;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(10, INPUT);//spinca
  pinMode(11, INPUT);//spinac
  pinMode(2, OUTPUT);//dioda
}

void startMotor(){
  digitalWrite(2, HIGH);//simulace zapnuti motoru
}

void stopMotor(){
  digitalWrite(2, LOW);//simulace vypnuti motoru
}

void loop(){
  if (irrecv.decode(&results)){             //
        Serial.println(results.value, DEC); // Precte signal
        irrecv.resume();                    //
  }
  if (results.value == kodStartu){
    startMotor();
  }
  if (results.value == kodStopu || digitalRead(10) == HIGH || digitalRead(11) == HIGH) { //10 a 11 jsou spinace na koncich koleje
    stopMotor();
  }
}
