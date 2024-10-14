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

void getInputsCalc();
void printInputs(byte *inArray);
void newCalc();
void clearSolving();
void printCalc(byte cursorInput);

void setup() {
  ans = 0;
  a = 0;
  b = 0;
  c = 0;
  inputs[0] = 255; //serve a impedire che si vada su calcoli inesistenti
  newCalc();

  Serial.begin(9600); //DEBUG

  //inizializzazione tastierino
  pinMode(BIT0PIN, INPUT);
  pinMode(BIT1PIN, INPUT);
  pinMode(BIT2PIN, INPUT);
  pinMode(BIT3PIN, INPUT);
  pinMode(BIT4PIN, INPUT);
  
  // inizializzazione schermino
  lcd.begin(16, 2);
  lcd.print("---D'Ippolito---");
  lcd.setCursor(0,1);
  lcd.print("premere un tasto");
  getInputsCalc();
}

void loop() {
  byte bit0 = digitalRead(BIT0PIN);
  byte bit1 = digitalRead(BIT1PIN);
  byte bit2 = digitalRead(BIT2PIN);
  byte bit3 = digitalRead(BIT3PIN);
  byte bit4 = digitalRead(BIT4PIN);
  int buttonID = bit0 + bit1 * 2 + bit2 * 4 + bit3 * 8 + bit4 * 16;

  short int inputChars; // numero di caratteri corrispondenti
  
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

  //delay(1000);
}

void getInputsCalc() {
  byte input = 255;
  bool shift = false;
  byte buttonDead = 0; // 0 = bottone è premuto
  byte lastInput;
  byte check = 0;

  byte movement;
  byte cursor = 0;

  do {
    input = 0;
    input += digitalRead(BIT0PIN);
    input += digitalRead(BIT1PIN) * 2;
    input += digitalRead(BIT2PIN) * 4;
    input += digitalRead(BIT3PIN) * 8;
    input += digitalRead(BIT4PIN) * 16;

    if (input != 0) {
      input = input | lastInput;
      check++;
      
      if (check < 5) {
        continue;
      } else {
        check = 128;
      }

      if (buttonDead < 10) {
        continue;
      }
      else {
        buttonDead = 0;
      }
    } else {
      check = 0;
      buttonDead += (buttonDead < 255) ? 1 : 0;
      lastInput = 0;
      continue;
    }

    Serial.print("input: ");
    Serial.println(input);

    if (input == 30) {        // shift
      movement = 0;
      shift = true;
    } else if (input <= _ans_) {
      Serial.println("CHAR!");
      movement = 2;
      if (inputs[cursor] != 255 && inputs[maxInputLength - 1] == 255) {
        for (int i = maxInputLength - 1; i >= cursor; i--) {
          inputs[i] = inputs[i-1];
        }
      }
      inputs[cursor++] = input;
      if (input > _pow_ && shift) {
        inputs[cursor - 1] += 8;
      }
    } else if (input == 27) { // DEL & AC 
      if (shift) {
        movement = 4;
        for (int i = 0; i < maxInputLength && inputs[i] != 255; i++) {
          inputs[i] = 255;
        }
        cursor = 0;
      } 
      else if (cursor!= 0) {
        cursor--;
        movement = 1;
        for (int i = cursor; i < maxInputLength - 1; i++) {
          inputs[i] = inputs[i+1];
          inputs[i+1] = 255;
        }
      }
    } else if (input == 28) { // ->
      if (inputs[cursor] != 255 && cursor + 1 < maxInputLength) {
        Serial.println("Right movement");
        movement = 2;
        cursor++;
      }
    } else if (input == 29) { // <-
      if (cursor != 0) {
        Serial.println("Left movement");
        movement = 1;
        cursor--;
      } /*else { // sposta il cursore alla fine del calcolo
        movement = 3; //TODO
        for (int i = 0; inputs[i] != 255 && i < maxInputLength; i--) {
          cursor = i;
        }
      }*/
    } else if (input == 31) { // =
      //fromInputToEquation();TODO
      movement = 4;
      cursor = 0;
      break;
    }
    
    shift=false;
    printCalc(cursor, movement);
    movement = 0;
    delay(10);
  } while (input != _equ_);
}

void printCalc(byte cursorInput, short int movement) {
  Serial.println("start");
  Serial.print("movement: ");
  Serial.println(movement);

  static byte stringShift = 0;
  static int cursorString = 0;
  String complete = "";
  String toPrint = "";
  String cursorPrint = "0123456789abcdef";

  for (int i = 0; i < maxInputLength; i++) {
    switch (inputs[i]) {
      case _0_:
        complete += "0";
        break;
      case _1_:
        complete += "1";
        break;
      case _2_:
        complete += "2";
        break;
      case _3_:
        complete += "3";
        break;
      case _4_:
        complete += "4";
        break;
      case _5_:
        complete += "5";
        break;
      case _6_:
        complete += "6";
        break;
      case _7_:
        complete += "7";
        break;
      case _8_:
        complete += "8";
        break;
      case _9_:
        complete += "9";
        break;
      case _dot_:
        complete += ".";
        break;
      case _plu_:
        complete += "+";
        break;
      case _min_:
        complete += "-";
        break;
      case _for_:
        complete += "*";
        break;
      case _div_:
        complete += "/";
        break;
      case _opa_:
        complete += "(";
        break;
      case _cpa_:
        complete += ")";
        break;
      case _pow_:
        complete += "^";
        break;
      case _sqr_:
        complete += "#";
        break;
      case _sin_:
        complete += "s";
        break;
      case _cos_:
        complete += "c";
        break;
      case _tan_:
        complete += "t";
        break;
      case _log_:
        complete += "l";
        break;
      case _ln_:
        complete += "e";
        break;
      case _abs_:
        complete += "a";
        break;
      case _ans_:
        complete += "Q";
        break;
      case _xsq_:
        complete += "x";
        break;
      case _hsi_:
        complete += "d";
        break;
      case _hco_:
        complete += "v";
        break;
      case _hta_:
        complete += "z";
        break;
      case _pi_:
        complete += "p";
        break;
      case _B_:
        complete += "B";
        break;
      case _C_:
        complete += "C";
        break;
      case _A_:
        complete += "A";
        break;
      default:
        i = maxInputLength;
        break;
    }
  }

  if (movement == 1) {
    cursorString--;
  } else if (movement == 2) {
    cursorString++;
  } else if (movement == 4) {
    stringShift = 0;
    cursorString = 0;
  }

  if (cursorString >= 16) {
    cursorString = 15;
    stringShift++;
  } else if (cursorString < 0) {
    cursorString = 0;
    stringShift--;
  }
  
  for (int i = 0; i < 16; i++) {
    toPrint += (i + stringShift < complete.length()) ? complete[i + stringShift] : ' ';
  }
  
  cursorPrint[cursorString] = 'L';
  lcd.setCursor(0, 0);
  lcd.print(toPrint);
  lcd.setCursor(0, 1);
  lcd.print(cursorPrint);
  Serial.println(complete);
  Serial.println(toPrint);
  Serial.println("endPrint");
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
