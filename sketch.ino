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

int ledVlevo = 4;
int ledVpravo = 5;
int ledUprostred = 6;

int levySpinac = 10;
int pravySpinac = 11;

int IRsenzorPohybu = 8;

int motorPravy = 2;
int motorLevy = 3;

ezButton levy(10);
ezButton pravy(11);

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(ledVlevo, OUTPUT);
  pinMode(ledVpravo, OUTPUT);
  pinMode(ledUprostred, OUTPUT);
  pinMode(levySpinac, INPUT_PULLUP);//spinac levy
  pinMode(pravySpinac, INPUT_PULLUP);//spinac pravy
  pinMode(motorLevy, OUTPUT);//motor //strany vůbec nevim
  pinMode(motorPravy, OUTPUT);//motor //strany vůbec nevim
  pinMode(IRsenzorPohybu, INPUT);//IR senzor LOW A HIHG JE NAOPAK
  pravy.setDebounceTime(100);
  levy.setDebounceTime(100);
}

void loop(){
  //nevim jak udelat at to bere ten kod
  if (irrecv.decode(&results)){             //
        Serial.println(results.value, DEC); // Precte signal
        irrecv.resume();                    //
  }
  else if(digitalRead(levySpinac) == HIGH){
    digitalWrite(ledVlevo, HIGH);
    digitalWrite(ledVpravo, LOW);
  }
  else if(digitalRead(pravySpinac) == HIGH){
    digitalWrite(ledVpravo, HIGH);
    digitalWrite(ledVlevo, LOW);
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == HIGH && digitalRead(IRsenzorPohybu) == HIGH) {
    n=1;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(levySpinac) == HIGH && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == HIGH) {
    n=2;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(levySpinac) == HIGH && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == HIGH) {
    n=1;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == HIGH) {
    n=1;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == HIGH && digitalRead(IRsenzorPohybu) == HIGH) {
    n=2;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value != kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == LOW) {
    n=2;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(levySpinac) == HIGH && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == LOW) {
    n=2;
    results.value = 1;
  }
  else if (results.value != kodStopu && results.value == kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == HIGH && digitalRead(IRsenzorPohybu) == LOW) {
    n=2;
    results.value = 1;
  }else if (results.value == kodStopu && results.value != kodStartu && digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == LOW && digitalRead(IRsenzorPohybu) == HIGH) {
    n=2;
    results.value = 1;
  }
  if (digitalRead(levySpinac) == HIGH && stav == 1){
    stav = 2;
  }else if (digitalRead(pravySpinac) == HIGH && stav == 2){
    stav = 1;
  }
  switch (n) {
  case 1:
    digitalWrite(ledUprostred, HIGH);
    if(stav == 1){
          digitalWrite(motorPravy, HIGH);
          digitalWrite(motorLevy, LOW);
          Serial.println("jede doleva");
    }else if (stav == motorPravy){
          digitalWrite(motorPravy, LOW);
          digitalWrite(motorLevy, HIGH);
          Serial.println("jede doprava");
    }
    break;
  case 2:
    digitalWrite(ledUprostred, LOW);
    digitalWrite(motorPravy, LOW);
    digitalWrite(motorLevy, LOW);
    Serial.println("stoji");
    break;
  default:
    break;
}
}
