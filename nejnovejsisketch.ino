#include <IRremote.h>
//knihovna pro IR přijímač

//proměnné a konstanty
const int RECV_PIN = 12;
const int LED_Uprostred_Pin = 6;
const int LED_Vlevo_Pin = 4;
const int LED_Vpravo_Pin = 5;
const int Motor_Pravy_Pin = 2;
const int Motor_Levy_Pin = 3;
const int Button_Levy_Pin = 10;
const int Button_Pravy_Pin = 11;
const int IR_Senzor_Pohybu_Pin = 9;

//kódy tlačítek
const unsigned long OPEN_CODE = 3328992116;
const unsigned long CLOSE_CODE = 404571636;
const unsigned long STOP_CODE = 204228740;

//směry
enum Direction { STOP = 0, LEFT = 1, RIGHT = 2 };

//proměnná pro IR přijímač
IRrecv irrecv(RECV_PIN);
decode_results results;

int direction = STOP;
int n = 0;

//povolení všech pinů a povolení IR aby přijímal
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
  //zapíná ledky v závislosti na proměnnou n a strany
  digitalWrite(LED_Uprostred_Pin, n == 1);
  digitalWrite(LED_Vlevo_Pin, n == 1 && direction == LEFT);
  digitalWrite(LED_Vpravo_Pin, n == 1 && direction == RIGHT);
  //zapíná motor v závistlo na proměnnou n a strany, vždycky musí být zaplý jenom 1 pin aby se motor rozjel na správnou stranu
  digitalWrite(Motor_Pravy_Pin, n == 1 && direction == LEFT);
  digitalWrite(Motor_Levy_Pin, n == 1 && direction == RIGHT);

  //delay na cílené spoždění koncových spínačů, aby nereagovaly hned
  if (n == 1) {
    delay(180);
    }
}
void checkIRSensorAndButtons() {
  //vytvořeny nové proměnné, aby se v kódu furt nemusel opakovat digitalRead
  bool buttonLevyPressed = (digitalRead(Button_Levy_Pin) == HIGH);
  bool buttonPravyPressed = (digitalRead(Button_Pravy_Pin) == HIGH);
  bool irSensorHigh = (digitalRead(IR_Senzor_Pohybu_Pin) == HIGH);

  //n = 2 znamená že motor nejede, n = 2 znamená že motor jede
  if (irSensorHigh) { //kontroluje senzor pohybu
    if (results.value != STOP_CODE) { //kontroluje tlačítko na zastavení
      if (results.value != OPEN_CODE && results.value != CLOSE_CODE) { //ptá se, jestli ani jedno tlačítko není zmáčklé, jestli není, tak to povoluje aby se aktivovala podmínka níže
        if ((buttonPravyPressed && !buttonLevyPressed) || (!buttonPravyPressed && buttonLevyPressed)) { // podmínky na zastavení brány, když se dotkne koncového spínače
          n = 2;
        }
      } else if (!buttonLevyPressed || !buttonPravyPressed) {//ptá se, jestli není aktivní 1 koncový spínač, jestli není tak se brána může rozjet
        n = 1;
      }
    } else {
      n = 2;
    }
  } else {
    n = 2;
    results.value = STOP_CODE; // nastavení results.value na stop kód, aby se brána znovu nerozjela po deaktivování senzoru pohybu
  }
}

//mění směr v závislosti na kódu tlačítka a resetuje results.value, aby správně fungovali podmínky v předešlé funkci
void processIRResults() {
  if (results.value == OPEN_CODE) {
    direction = LEFT;
    results.value = 0;
  } else if (results.value == CLOSE_CODE) {
    direction = RIGHT;
    results.value = 0;
  }
}

//vyvolává všechny funkce a ukládá kód tlačítka do results.value v případě že je přijat nějaký signál
void loop() {
  if (irrecv.decode(&results)) {
    irrecv.resume();
  }
  checkIRSensorAndButtons();
  processIRResults();
  handleLEDsAndMotors();
}
