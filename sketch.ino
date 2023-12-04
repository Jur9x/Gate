//current state
#include <IRremote.h>
#include <ezButton.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long kodStartu = 3389572121;
unsigned long kodStopu = 3439970883;

int stav = 1; //1 doleva 2 doprava
int n;

ezButton levy(10);
ezButton pravy(11);

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(10, INPUT_PULLUP);//spinac levy
  pinMode(11, INPUT_PULLUP);//spinac pravy
  pinMode(2, OUTPUT);//motor //strany vůbec nevim
  pinMode(3, OUTPUT);//motor //strany vůbec nevim
  pinMode(8, INPUT);//IR senzor LOW A HIHG JE NAOPAK
  pravy.setDebounceTime(100);
  levy.setDebounceTime(100);
}

void loop(){
  //nevim jak udelat at to bere ten kod
  if (irrecv.decode(&results)){             //
        Serial.println(results.value, DEC); // Precte signal
        irrecv.resume();                    //
  }
  if (results.value != kodStopu && results.value == kodStartu && digitalRead(10) == LOW && digitalRead(11) == HIGH && digitalRead(8) == HIGH) {
    n=1;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(10) == HIGH && digitalRead(11) == LOW && digitalRead(8) == HIGH) {
    n=2;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(10) == HIGH && digitalRead(11) == LOW && digitalRead(8) == HIGH) {
    n=1;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(10) == LOW && digitalRead(11) == LOW && digitalRead(8) == HIGH) {
    n=1;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(10) == LOW && digitalRead(11) == HIGH && digitalRead(8) == HIGH) {
    n=2;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(10) == LOW && digitalRead(11) == LOW && digitalRead(8) == LOW) {
    n=2;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(10) == HIGH && digitalRead(11) == LOW && digitalRead(8) == LOW) {
    n=2;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(10) == LOW && digitalRead(11) == HIGH && digitalRead(8) == LOW) {
    n=2;
  }else if (results.value == kodStopu && results.value != kodStartu && digitalRead(10) == LOW && digitalRead(11) == LOW && digitalRead(8) == HIGH) {
    n=2;
  }
  if (digitalRead(10) == HIGH && stav == 1){
    stav = 2;
  }else if (digitalRead(11) == HIGH && stav == 2){
    stav = 1;
  }
  switch (n) {
  case 1:
    if(stav == 1){
          digitalWrite(2, HIGH);
          digitalWrite(3, LOW);
          Serial.println("jede doleva");
    }else if (stav == 2){
          digitalWrite(2, LOW);
          digitalWrite(3, HIGH);
          Serial.println("jede doprava");
    }
    break;
  case 2:
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    Serial.println("stoji");
    break;
  default:
    break;
}
}
