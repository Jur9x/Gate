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

int ledVlevo = 4; // otevira se - jede doprava
int ledVpravo = 5; //zavira se - jede doleva
int ledUprostred = 6;
int ledJede = 7;
int ledNejede = 8;

int levySpinac = 10;
int pravySpinac = 11;

int IRsenzorPohybu = 9;

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
  pinMode(levySpinac, INPUT_PULLUP);
  pinMode(pravySpinac, INPUT_PULLUP);
  pinMode(motorLevy, OUTPUT);//strany vůbec nevim
  pinMode(motorPravy, OUTPUT);//strany vůbec nevim
  pinMode(IRsenzorPohybu, INPUT);
  pravy.setDebounceTime(100);
  levy.setDebounceTime(100);
}

void loop(){
  if (irrecv.decode(&results)){
        Serial.println(results.value, DEC);
        irrecv.resume(); 
  }
  if(digitalRead(IRsenzorPohybu) == HIGH){
      //senzor nic nesnímá
      if(results.value != kodStopu){
        if(results.value != kodStartu){
          if((digitalRead(levySpinac) == HIGH && digitalRead(pravySpinac) == LOW) || (digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == HIGH)){
            n = 2;
          }
        }else if(((digitalRead(levySpinac) == HIGH && digitalRead(pravySpinac) == LOW) || (digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == HIGH)) || (digitalRead(levySpinac) == LOW && digitalRead(pravySpinac) == LOW)){
          n = 1;
      }
    }else(n=2);
  }else if(digitalRead(IRsenzorPohybu == LOW)){
    //senzor něco snímá
    n=2;
  }
  if (digitalRead(levySpinac) == HIGH && stav == 1){
    stav = 2;
    results.value = 0;
  }else if (digitalRead(pravySpinac) == HIGH && stav == 2){
    stav = 1;
    results.value = 0;
  }
  switch (n) {
  case 1:
    digitalWrite(ledUprostred, HIGH);
    if(stav == 1){
          digitalWrite(motorPravy, HIGH);
          digitalWrite(motorLevy, LOW);
          Serial.println("jede doleva");
          digitalWrite(ledVpravo, HIGH);
          digitalWrite(ledVlevo, LOW);
          Serial.println(results.value, DEC);
          Serial.println(n);
    }else if (stav == 2){
          digitalWrite(motorPravy, LOW);
          digitalWrite(motorLevy, HIGH);
          Serial.println("jede doprava");
          digitalWrite(ledVlevo, HIGH);
          digitalWrite(ledVpravo, LOW);
          Serial.println(n);
    }
    break;
  case 2:
    digitalWrite(ledUprostred, LOW);
    digitalWrite(ledVlevo, LOW);
    digitalWrite(ledVpravo, LOW);
    digitalWrite(motorPravy, LOW);
    digitalWrite(motorLevy, LOW);
    Serial.println("stoji");
    Serial.println(n);
    break;
  default:
    break;
  }}
