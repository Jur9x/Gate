//current state
#include <IRremote.h>

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, DEC);
        irrecv.resume();
  }
  if(digitalRead(9) == HIGH){
    digitalWrite(2, HIGH);
  }
   if(digitalRead(10) == HIGH){
    digitalWrite(2, LOW);
  }
}
