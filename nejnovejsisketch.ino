#include <IRremote.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long kodZavrit = 3328992116;
unsigned long kodOtevrit = 404571636;
unsigned long kodStopu = 204228740;

int smer = 0; // 1 doleva 2 doprava
int n = 0;

int ledPins[] = {6, 4, 5};  // ledUprostred, ledVlevo, ledVpravo
int motorPins[] = {2, 3};   // motorPravy, motorLevy
int buttonPins[] = {10, 11}; // levySpinac, pravySpinac
int IRsenzorPohybu = 9;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  for (int pin : ledPins) pinMode(pin, OUTPUT);
  for (int pin : buttonPins) pinMode(pin, INPUT_PULLUP);
  for (int pin : motorPins) pinMode(pin, OUTPUT);
  pinMode(IRsenzorPohybu, INPUT);
}

void handleMovement() {
  digitalWrite(ledPins[0], n == 1);
  digitalWrite(ledPins[1], n == 1 && smer == 1);
  digitalWrite(ledPins[2], n == 1 && smer == 2);
  digitalWrite(motorPins[0], n == 1 && smer == 1);
  digitalWrite(motorPins[1], n == 1 && smer == 2);
  delay(160);
  Serial.println(n);
}

//idk jestli funguje tak jak má, na zavírání a otevírání tho :)
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    irrecv.resume();
  }

  if(digitalRead(IRsenzorPohybu) == HIGH){
     if (results.value != kodStopu){
        if((results.value != kodOtevrit && results.value != kodZavrit) || (results.value == kodOtevrit && results.value == kodZavrit)){
          if((digitalRead(buttonPins[1]) == LOW && digitalRead(buttonPins[0]) == HIGH) || (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == HIGH)){
            n = 2;
            Serial.println("TADYNAHORE"); 
          }
        }else if(((digitalRead(buttonPins[1]) == LOW && digitalRead(buttonPins[0]) == HIGH) || (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == HIGH)) || (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == LOW)){
          n = 1; 
        }
      }else {
      n = 2;
      Serial.println("TADYDOLE1");
      }
  }else {
    n = 2;
    results.value = kodStopu;
    Serial.println("TADYDOLE2");
  }

  if (results.value == kodOtevrit) {
    smer = 1;
    results.value = 0;
  }else if(results.value == kodZavrit){
    smer = 2;
    results.value = 0;
  }
  handleMovement();
}
