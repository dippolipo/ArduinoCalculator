#include <LiquidCrystal.h>

#define _0_ 1
#define _1_ 2
#define _2_ 3
#define _3_ 4
#define _4_ 5
#define _5_ 6
#define _6_ 7
#define _7_ 8
#define _8_ 9
#define _9_ 10
#define _dot_ 11
#define _plu_ 12
#define _min_ 13
#define _for_ 14
#define _div_ 15
#define _opa_ 16
#define _cpa_ 17
#define _pow_ 18 
#define _sqr_ 19 
#define _sin_ 20 
#define _cos_ 21 
#define _tan_ 22 
#define _log_ 23 
#define _ln_ 24  
#define _abs_ 25 
#define _ans_ 26 // DA IMPLEMENTARE
#define _xsq_ 27 
#define _hsi_ 28 
#define _hco_ 29 
#define _hta_ 30 
#define _pi_ 31  
#define _B_ 32   // DA IMPLEMENTARE
#define _C_ 33   // DA IMPLEMENTARE
#define _A_ 34   // DA IMPLEMENTARE

#define _shi_ 30
#define _equ_ 31

#define BIT0PIN 6
#define BIT1PIN 7
#define BIT2PIN 8
#define BIT3PIN 9
#define BIT4PIN 10

#define maxInputLength 50

double ans;
double a;
double b;
double c;
byte stringCursorPos;
byte inputStartingPos;
byte stringStartingPos;
byte programState; //0=starting,1=prendendoinputpercalcolo,2=calcolando,3=calcolofinito
short int input[maxInputLength];

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte inputs[maxInputLength];
byte inputs1[maxInputLength];
byte inputs2[maxInputLength];
byte inputs3[maxInputLength];
byte pars[maxInputLength / 2 + 1][3];

void getInputs();
void printInputs(byte *inArray);
void newCalculation();
void clearSolving();

void setup() {
  ans = 0;
  a = 0;
  b = 0;
  c = 0;
  inputs[0] = 255; //serve a impedire che si vada su calcoli inesistenti
  newCalculation();

  Serial.begin(9600); //DEBUG

  //inizializzazione tastierino
  pinMode(BIT0PIN, INPUT);
  pinMode(BIT1PIN, INPUT);
  pinMode(BIT2PIN, INPUT);
  pinMode(BIT3PIN, INPUT);
  pinMode(BIT4PIN, INPUT);
  
  // inizializzazione schermino
  lcd.begin(16, 2);
}

void loop() {
  byte bit0 = digitalRead(BIT0PIN);
  byte bit1 = digitalRead(BIT1PIN);
  byte bit2 = digitalRead(BIT2PIN);
  byte bit3 = digitalRead(BIT3PIN);
  byte bit4 = digitalRead(BIT4PIN);
  int buttonID = bit0 + bit1 * 2 + bit2 * 4 + bit3 * 8 + bit4 * 16;
  
  Serial.println(buttonID);

  lcd.setCursor(0, 0);
  if (buttonID == 0) {
    Serial.println("no signal");
    lcd.print("no signal");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    lcd.clear();
    lcd.print(bit4);
    lcd.setCursor(1, 0);
    lcd.print(bit3);
    lcd.setCursor(2, 0);
    lcd.print(bit2);
    lcd.setCursor(3, 0);
    lcd.print(bit1);
    lcd.setCursor(4, 0);
    lcd.print(bit0);
    lcd.noAutoscroll();
    lcd.setCursor(0, 1);
    lcd.print(buttonID); 
  }
  
  Serial.println(" ");

  delay(1000);
}

void getCalcInputs() {
  byte input = 255;
  bool shift = false;
  bool buttonRead = true;
  byte bit0;
  byte bit1;
  byte bit2;
  byte bit3;
  byte bit4;
  
  byte cursor = 0;

  do {
    bit0 = digitalRead(BIT0PIN);
    bit1 = digitalRead(BIT1PIN);
    bit2 = digitalRead(BIT2PIN);
    bit3 = digitalRead(BIT3PIN);
    bit4 = digitalRead(BIT4PIN);
    input = bit0 + bit1 * 2 + bit2 * 4 + bit3 * 8 + bit4 * 16;
    
    if (input != 0) {
      if (buttonRead) {
        continue;
      } else {
        buttonRead = true;
      }
    } else {
      continue;
      buttonRead == false;
    }

    if (input == 30) {        // shift
      shift = true;
    } else if (input <= _ans_ && inputs[maxInputLength] == 255) {
      if (inputs[cursor] != 255) {
        byte nextNumber = inputs[cursor];
        for (int i = cursor + 1; i < maxInputLength && inputs[i] != 255; i++) {
          inputs[cursor] = inputs[i];
          inputs[i] = nextNumber;
          nextNumber = inputs[cursor];
        }
      } 
      inputs[cursor++] = input;
      if (input > _pow_ && shift) {
        inputs[cursor - 1] += 8;
      }
    } else if (input == 27) { // DEL & AC 
      if (shift) {
        for (int i = 0; i < maxInputLength && inputs[i] != 255; i++) {
          inputs[i] = 255;
        }
        cursor = 0;
      } 
      else if (cursor!= 0){
        int i = cursor;
        while (i < maxInputLength - 1 && inputs[i] != 255) {
          inputs[i] = inputs[i+1];
          i++;
        }
        inputs[i] = 255;
        cursor = 0;
      }
    } else if (input == 28) { // ->
      if (inputs[cursor] != 255 && cursor + 1 < maxInputLength) {
        cursor++;
      }
    } else if (input == 29) { // <-
      if (cursor != 0) {
        cursor--;
      } else { // sposta il cursore alla fine del calcolo
        for (int i = 0; inputs[i] != 255 && i < maxInputLength; i--) {
          cursor = i;
        }
      }
    } else if (input == 31) { // =
      //fromInputToEquation();TODO
      cursor = 0;
      break;
    }
    
    shift=false;
    
    buttonRead = input;
    buttonRead = !buttonRead;
    Serial.print(buttonRead);
    delay(1000);

  } while (input != _equ_);
}

void newCalc() {
	for (int i = 0; i < maxInputLength; i++) {
    inputs3[i] = inputs2[i];
    inputs2[i] = inputs1[i];
    inputs1[i] = inputs[i];
		inputs[i] = 255;
	}
  programState = 1;
}

void clearSolving() {
  stringCursorPos = 0;
  inputStartingPos = 0;
  stringStartingPos = 0;
  pars[0][0] = 0;
	pars[0][2] = 0;
	for (int i = 1; i < maxInputLength / 2 + 1; i++) {
		pars[i][0] = maxInputLength;
		pars[i][1] = 255;
		pars[0][2] = 0;
	}
}
