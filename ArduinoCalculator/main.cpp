##include <LiquidCrystal.h>

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
double numbers[maxInputLength / 2 + 1];
byte stringCursorPos;
byte inputStartingPos;
byte stringStartingPos;
byte programState; //0=starting,1=prendendoinputpercalcolo,2=calcolando,3=calcolofinito
short int input[maxInputLength];

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte inputs[maxInputLength];
byte pars[maxInputLength / 2 + 1][3];
byte operators[maxInputLength / 2];

void getInputsCalc();
void printInputs(byte *inArray);
void newCalc();
void clearSolving();
void printCalc(byte stringShift);
byte printCursor(short int movement);
void removeData(short int& firstNum, short int delta, short int& currentPar, short int& firstParAnalyzed);
double solve();
void printSol();
void inBetween();

void setup() {
  ans = 0;
  a = 0;
  b = 0;
  c = 0;

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
}

void loop() {
  getInputsCalc();
  fromInputToEquation();
  ans = solve();
  printSol();
  delay(2000);
  inBetween();
}

void getInputsCalc() {
  byte input = 255;
  bool shift = false;
  byte buttonDead = 0; // 0 = bottone è premuto
  byte lastInput;
  byte check = 0;

  short int movement;
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

    if (input == 30) {        // shift
      movement = 0;
      shift = true;
      continue;
    } else if (input <= _ans_) {
      if (inputs[cursor] != 255 && inputs[maxInputLength - 1] == 255) {
        for (int i = maxInputLength - 1; i >= cursor; i--) {
          inputs[i] = inputs[i-1];
        }
      }
      inputs[cursor++] = input;
      if (input > _pow_ && shift) {
        inputs[cursor - 1] += 8;
      }
      if ((inputs[cursor-1] > _ln_ && inputs[cursor-1] <= _hta_) || (inputs[cursor-1] >= _sin_ && inputs[cursor-1] < _ln_)) {
        movement = 3;
      } else if (inputs[cursor-1] == _ln_) {
        movement = 2;
      } else {
        movement = 1;
      }
    } else if (input == 27) { // DEL & AC 
      if (shift) {
        movement = 255;
        for (int i = 0; i < maxInputLength && inputs[i] != 255; i++) {
          inputs[i] = 255;
        }
        cursor = 0;
      } 
      else if (cursor!= 0) {
        cursor--;
        if ((inputs[cursor] > _ln_ && inputs[cursor] <= _hta_) || (inputs[cursor] >= _sin_ && inputs[cursor] < _ln_)) {
          movement = -3;
        } else if (inputs[cursor] == _ln_) {
          movement = -2;
        } else {
          movement = -1;
        }
        for (int i = cursor; i < maxInputLength - 1; i++) {
          inputs[i] = inputs[i+1];
          inputs[i+1] = 255;
        }
      }
    } else if (input == 28) { // ->
      if (inputs[cursor] != 255 && cursor + 1 < maxInputLength) {
        if ((inputs[cursor] > _ln_ && inputs[cursor] <= _hta_) || (inputs[cursor] >= _sin_ && inputs[cursor] < _ln_)) {
          movement = 3;
        } else if (inputs[cursor] == _ln_) {
          movement = 2;
        } else {
          movement = 1;
        }
        cursor++;
      }
    } else if (input == 29) { // <-
      if (cursor != 0) {
        cursor--;
        if ((inputs[cursor] > _ln_ && inputs[cursor] <= _hta_) || (inputs[cursor] >= _sin_ && inputs[cursor] < _ln_)) {
          movement = -3;
        } else if (inputs[cursor] == _ln_) {
          movement = -2;
        } else {
          movement = -1;
        }
      } /*else { // sposta il cursore alla fine del calcolo
        movement = 3; //TODO
        for (int i = 0; inputs[i] != 255 && i < maxInputLength; i--) {
          cursor = i;
        }
      }*/
    } else if (input == 31) { // =
      movement = 255;
      cursor = 0;
    }
    
    shift=false;

    printCalc(printCursor(movement));
    movement = 0;
    delay(10);
  } while (input != _equ_);
}

byte printCursor(short int movement) {
  static byte stringShift = 0;
  static short int cursorString = 0;
  String cursorPrint = "0123456789abcdef";

  if (movement == 255) {
    stringShift = 0;
    cursorString = 0;
    movement = 0;
  }

  cursorString += movement;

  if (cursorString >= 16) {
    byte delta = cursorString - 16;
    cursorString = 15;
    stringShift += delta + 1;
  } else if (cursorString < 0) {
    stringShift += cursorString;
    cursorString = 0;
  }
  

  cursorPrint[cursorString] = 'L';
  lcd.setCursor(0, 1);
  lcd.print(cursorPrint);

  return stringShift;
}

void printCalc(byte shift) {
  String complete = "";
  String toPrint = "";

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
        complete += "sin";
        break;
      case _cos_:
        complete += "cos";
        break;
      case _tan_:
        complete += "tan";
        break;
      case _log_:
        complete += "log";
        break;
      case _ln_:
        complete += "ln";
        break;
      case _abs_:
        complete += "abs";
        break;
      case _ans_:
        complete += "ans";
        break;
      case _xsq_:
        complete += "]V";
        break;
      case _hsi_:
        complete += "hsi";
        break;
      case _hco_:
        complete += "hco";
        break;
      case _hta_:
        complete += "hta";
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
  
  for (int i = 0; i < 16; i++) {
    toPrint += (i + shift < complete.length()) ? complete[i + shift] : ' ';
  }

  lcd.setCursor(0, 0);
  lcd.print(toPrint);
}

void newCalc() {
  pars[0][0] = 0;
  pars[0][2] = 0;
  for (int i = 1; i < maxInputLength / 2 + 1; i++) {
    pars[i][0] = maxInputLength;
    pars[i][1] = 255;
    pars[0][2] = 0;
  }
	for (int i = 0; i < maxInputLength; i++) {
		inputs[i] = 255;
	}
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

short int fromInputToEquation() {
	byte numNum   = 0; // numero di numeri
	byte opNum    = 0; // numero di operatori
	byte parNum   = 0;
	byte funcNum  = 0;

	bool isPositive = true;
	short int digitOverZero = 0; // 1 = true, > -1 = false, 0 = sconosciuto
	byte lastParToClose = 0;

	for (int i = 0; i < maxInputLength; i++) {
		if (inputs[i] <= _dot_) { // e' una cifra o un punto
			if (i > 0) {
				if (inputs[i - 1] == _cpa_ || inputs[i - 1] <= _pi_) {
					return 1;
				}
			}


			if (inputs[i] == _dot_) {
				if (digitOverZero >= 0) {
					digitOverZero = -1;
				}
				else {
					return 1;
				}
			}
			else {
				if (digitOverZero >= 0) {
					numbers[numNum] = numbers[numNum] * 10 + (inputs[i] - 1);
					digitOverZero = 1;
				}
				else {
					numbers[numNum] += (inputs[i] - 1) * pow(10, digitOverZero--);
				}
			}
		}
		else if (digitOverZero != 0 && (inputs[i] <= _div_ || inputs[i] == _pow_)) {
			digitOverZero = 0;
			operators[opNum++] = inputs[i];
			numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1	
			isPositive = true;
		}
		else if (inputs[i] == _min_) {
			isPositive = !isPositive;
		}
		else if (inputs[i] == _plu_) {
			continue;
		}
		else if (inputs[i] == _opa_ || (inputs[i] >= _sqr_ && inputs[i] <= _abs_) || (inputs[i] >= _hsi_ && inputs[i] <= _hta_)) {

			if (digitOverZero != 0) {
				digitOverZero = 0;
				operators[opNum++] = _for_;
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
			}

			pars[++parNum][2] = (1 - isPositive) * 0x80;
			pars[parNum][0] = numNum;
			
			if (lastParToClose == 0) {
				lastParToClose = parNum;
			}
			isPositive = true;

			if (inputs[i] != _opa_) {
				pars[parNum][2] = pars[parNum][2] | inputs[i];
			}

		}
		else if (inputs[i] == _cpa_ || inputs[i] == _xsq_) {	
			if (digitOverZero != 0) {
				for (int j = parNum; j >= lastParToClose; j--) {
					if (pars[j][1] == 255) {
						pars[j][1] = numNum;
						if (j == lastParToClose) {
							lastParToClose = 0;
						}
						if (pars[j][0] == 255) {
							pars[j][0] = numNum;
						}
						break;
					}
				}
			}
			else {
				return 1;
			}

			if (inputs[i] == _xsq_) {
				operators[opNum++] = inputs[i];
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
				
				pars[++parNum][2] = (1 - isPositive) * 0x80;
				pars[parNum][0] = numNum;
				if (lastParToClose == 0) {
					lastParToClose = parNum;
				}
			}
		}
		else if (inputs[i] == 255) {
			numbers[numNum] *= 1 + 2 * (-1 + isPositive);
			pars[0][1] = numNum;
			for (int j = parNum; j >= lastParToClose; j--) {
				if (pars[j][1] == 255) {
					pars[j][1] = numNum;
					if (j == lastParToClose) {
						lastParToClose = 0;
					}
					break;
				}
			}
			return 0;
		}
		else if (inputs[i] >= _pi_) {
			if (digitOverZero != 0) {
				operators[opNum++] = _for_;
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
			}
			switch (inputs[i]) {
			case _pi_:
				numbers[numNum] = PI;
				break;
			default:
				return 2;
				break;
			}
			if (inputs[i + 1] <= _dot_) {
				return 2;
			}
			digitOverZero = 1;
		}
		else if (inputs[i] != 255) {
      return 2;
		}
	}
}

void printSol() {
  lcd.clear();
  printCalc(0);
  lcd.setCursor(0,1);
  lcd.print(ans);
}

double solve() {

	static byte parAnalyzed = 0;
	byte currentPar = parAnalyzed;

	while (pars[parAnalyzed + 1][1] != 255 && pars[currentPar][1] >= pars[parAnalyzed + 1][0]) {
		parAnalyzed++;
		byte firstParAnalyzed = parAnalyzed;
		numbers[pars[firstParAnalyzed][0]] = solve();
		removeData(pars[firstParAnalyzed][0], pars[firstParAnalyzed][1] - pars[firstParAnalyzed][0], currentPar, firstParAnalyzed);
	}

  double sol = 0;

	for (short int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // ^ & xsqr
		if (operators[i] > _div_) {
			numbers[i] = (operators[i] == _pow_) ?pow(numbers[i], numbers[i + 1]) : pow(numbers[i+1], 1.f / numbers[i]);;
			removeData(i, 1, currentPar, currentPar);
		} 
	}

	short int firstNOfForDiv = 255;
	for (int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // * & /
		if (operators[i] > _min_) {
			firstNOfForDiv = (firstNOfForDiv == 255) ? i : firstNOfForDiv;
			numbers[firstNOfForDiv] *= (operators[i] == _for_) ? numbers[i + 1] : (1 / numbers[i + 1]);
			numbers[i + 1] = 0;
			operators[i] = _plu_;
			continue;
		}
		firstNOfForDiv = 255;
	}

	sol += numbers[pars[currentPar][0]];
	for (int i = pars[currentPar][0] + 1; i <= pars[currentPar][1]; i++) { // + & -
		sol += (operators[i - 1] == _plu_) ? numbers[i] : -numbers[i];
	}

	bool isPositive = (pars[currentPar][2] > 0x7F);
	pars[currentPar][2] = pars[currentPar][2] & 0x7F;
	
	switch (pars[currentPar][2]) {
	case _sqr_:
		if (sol < 0) {
			//TODOprintError(uError);
		}
		sol = sqrt(sol);
		break;
	case _sin_:
		sol = sin(sol);
		break;
	case _cos_:
		sol = cos(sol);
		break;
	case _tan_:
		sol = tan(sol);
		break;
	case _log_:
		if (sol <= 0) {
			//TODOprintError(uError);
		}
		sol = tan(sol);
		break;
	case _ln_:
		if (sol <= 0) {
			//TODOprintError(uError);
		}
		sol = tan(sol);
		break;
	case _abs_:
		sol = abs(sol);
		break;
	case _hsi_:
		if (sol < -1 || sol > 1) {
			//TODOprintError(uError);
		}
		sol = asin(sol);
		break;
	case _hco_:
		if (sol < -1 || sol > 1) {
			//TODOprintError(uError);
		}
		sol = acos(sol);
		break;
	case _hta_:
		sol = atan(sol);
		break;
	}
	return sol * (1 - 2 * isPositive);
}

void removeData(byte firstNum, short int delta, byte currentPar, byte firstParAnalyzed) {

	for (int i = firstNum + 1; i <= pars[0][1] - delta; i++) {
		numbers[i] = numbers[i + delta];
		operators[i - 1] = operators[i + delta - 1];
	}

	for (int i = 1; i < firstParAnalyzed; i++) {
		if (pars[i][1] >= pars[currentPar][1]) {
			pars[i][1] -= delta;
		}
	}

	for (int i = firstParAnalyzed + 1; pars[i][0] != maxInputLength; i++) {
		pars[i][0] -= delta;
		pars[i][1] -= delta;
	}

	pars[0][1] -= delta;
}

void inBetween() {
  byte input = 0;
  while (input == 0) {
    input = 0;
    input += digitalRead(BIT0PIN);
    input += digitalRead(BIT1PIN) * 2;
    input += digitalRead(BIT2PIN) * 4;
    input += digitalRead(BIT3PIN) * 8;
    input += digitalRead(BIT4PIN) * 16;
  }
}
