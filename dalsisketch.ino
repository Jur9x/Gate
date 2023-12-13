#include <IRremote.h>
#include <ezButton.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long kodStartu = 3389572121;
unsigned long kodStopu = 3439970883;

int smer = 1; // 1 doleva 2 doprava
int n;

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
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, DEC);
    irrecv.resume();
  }
  int irSensorState = digitalRead(IRsenzorPohybu);
  if (irSensorState == HIGH) {
    if (results.value != kodStopu && ((results.value == kodStartu && (buttonPins[0] != HIGH || buttonPins[1] != HIGH)) || (buttonPins[0] == LOW && buttonPins[1] == LOW))) {
      n = 1;
    } else {
      n = 2;
    }
  } else {
    n = 2;
    results.value = kodStopu;
  }

  if ((digitalRead(buttonPins[0]) == HIGH && smer == 1) || (digitalRead(buttonPins[1]) == HIGH && smer == 2)) {
    results.value = 0;
    smer = 3 - smer;
  }

  handleMovement();
}
