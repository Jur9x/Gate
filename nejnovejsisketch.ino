#include <IRremote.h>

const int RECV_PIN = 12;
const int LED_Uprostred_Pin = 6;
const int LED_Vlevo_Pin = 4;
const int LED_Vpravo_Pin = 5;
const int Motor_Pravy_Pin = 2;
const int Motor_Levy_Pin = 3;
const int Button_Levy_Pin = 10;
const int Button_Pravy_Pin = 11;
const int IR_Senzor_Pohybu_Pin = 9;

const unsigned long OPEN_CODE = 3328992116;
const unsigned long CLOSE_CODE = 404571636;
const unsigned long STOP_CODE = 204228740;

enum Direction { STOP = 0, LEFT = 1, RIGHT = 2 };

IRrecv irrecv(RECV_PIN);
decode_results results;

int direction = STOP;
int n = 0;

void setup() {
  irrecv.enableIRIn();
  pinMode(LED_Uprostred_Pin, OUTPUT);
  pinMode(LED_Vlevo_Pin, OUTPUT);
  pinMode(LED_Vpravo_Pin, OUTPUT);
  pinMode(Motor_Pravy_Pin, OUTPUT);
  pinMode(Motor_Levy_Pin, OUTPUT);
  pinMode(Button_Levy_Pin, INPUT);
  pinMode(Button_Pravy_Pin, INPUT);
  pinMode(IR_Senzor_Pohybu_Pin, INPUT);
}

void handleLEDsAndMotors() {
  digitalWrite(LED_Uprostred_Pin, n == 1);
  digitalWrite(LED_Vlevo_Pin, n == 1 && direction == LEFT);
  digitalWrite(LED_Vpravo_Pin, n == 1 && direction == RIGHT);
  digitalWrite(Motor_Pravy_Pin, n == 1 && direction == LEFT);
  digitalWrite(Motor_Levy_Pin, n == 1 && direction == RIGHT);

  if (n == 1) {
    delay(180);
    }
}
void checkIRSensorAndButtons() {
  bool buttonLevyPressed = (digitalRead(Button_Levy_Pin) == HIGH);
  bool buttonPravyPressed = (digitalRead(Button_Pravy_Pin) == HIGH);
  bool irSensorHigh = (digitalRead(IR_Senzor_Pohybu_Pin) == HIGH);

  if (irSensorHigh) {
    if (results.value != STOP_CODE) {
      if (results.value != OPEN_CODE && results.value != CLOSE_CODE) {
        if ((buttonPravyPressed && !buttonLevyPressed) || (!buttonPravyPressed && buttonLevyPressed)) {
          n = 2;
        }
      } else if (!buttonLevyPressed || !buttonPravyPressed) {
        n = 1;
      }
    } else {
      n = 2;
    }
  } else {
    n = 2;
    results.value = STOP_CODE;
  }
}

void processIRResults() {
  if (results.value == OPEN_CODE) {
    direction = LEFT;
    results.value = 0;
  } else if (results.value == CLOSE_CODE) {
    direction = RIGHT;
    results.value = 0;
  }
}

void loop() {
  if (irrecv.decode(&results)) {
    irrecv.resume();
  }

  checkIRSensorAndButtons();
  processIRResults();
  handleLEDsAndMotors();
}
