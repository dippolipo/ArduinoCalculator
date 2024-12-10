#include <LiquidCrystal.h>
#include <fp64lib.h>

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
#define _ans_ 26
#define _xsq_ 27 
#define _hsi_ 28 
#define _hco_ 29 
#define _hta_ 30 
#define _pi_ 31  
#define _e_ 32
#define _Y_ 33
#define _X_ 34

#define _shi_ 30
#define _equ_ 31

#define BIT0PIN 6
#define BIT1PIN 7
#define BIT2PIN 8
#define BIT3PIN 9
#define BIT4PIN 10
#define LEDPIN 13

#define maxInputLength 50

typedef char sByte;

#define SERROR 1 // sintassi
#define MERROR 2 // matematica
#define PERROR 3 // programma

float64_t ans;
float64_t x;
float64_t y;

float64_t numbers[maxInputLength / 2 + 1];
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte inputs[maxInputLength];
byte pars[maxInputLength / 2 + 1][3];
byte operators[maxInputLength / 2];
byte error = 0;

byte checkInputs();
void getInputsCalc();
void printInputs(byte *inArray);
void newCalc();
void clearSolving();
void printCalc(byte stringShift);
byte printCursor(short int movement, bool rightShift);
void removeData(short int& firstNum, short int delta, short int& currentPar, short int& firstParAnalyzed);
float64_t solve();
void printSol();
void inBetween();
short int getMovement(byte cursor);

void setup() { // cap. 4
  ans = fp64_sd(0.f);
  y   = fp64_sd(0.f);
  x   = fp64_sd(0.f);

  Serial.begin(9600); //DEBUG

  //inizializzazione tastierino
  pinMode(BIT0PIN, INPUT);
  pinMode(BIT1PIN, INPUT);
  pinMode(BIT2PIN, INPUT);
  pinMode(BIT3PIN, INPUT);
  pinMode(BIT4PIN, INPUT);

  // inizializzazione LED
  pinMode(LEDPIN, OUTPUT);
  
  // inizializzazione schermino
  lcd.begin(16, 2);
  lcd.print("---D'Ippolito---");
  lcd.setCursor(0,1);
  lcd.print("premere un tasto");
}

void loop() { // cap. 4
  digitalWrite(LEDPIN, LOW);
  newCalc();
  getInputsCalc();
  digitalWrite(LEDPIN, LOW);
  lcd.setCursor(0, 1);
  lcd.print("solving...      ");
  Serial.print("error = ");
  Serial.println(error);
  error = fromInputToEquation();
  Serial.print("error = ");
  Serial.println(error);
  if (error == 0) {
    float64_t lastAns = ans;
    ans = solve();
    if (error != 0) {
      ans = lastAns;
    }
  }
  Serial.print("error = ");
  Serial.println(error);
  printSol();
  delay(2000);
  inBetween();
}

void getInputsCalc() { // cap. 4.3
  byte input = 0;
  bool shift = false;
  short int movement;
  byte cursor = 0;
  bool rightShift;
  do {
    input = checkInputs(input);

    Serial.print(input);
    Serial.print(", ");
    Serial.println(shift);


    movement = 0;
    rightShift = false;

    if (input == 30) { // shift
      shift = !shift;
      digitalWrite(LEDPIN, shift);
      continue;
    } else if (input <= 26 && inputs[maxInputLength - 1] == 255) { // qualcosa da inserire nel calcolo
      if (inputs[cursor] != 255) {
        for (int i = maxInputLength; i >= cursor; i--) {
          inputs[i] = inputs[i-1];
        }
      }
      inputs[cursor] = input;
      if (shift && input > _pow_) {
        inputs[cursor] += 8;
      }
      movement = getMovement(cursor++);
    
    } else if (input == 27) { // DEL & AC 
      if (shift) {
        movement = 255;
        for (int i = 0; i < maxInputLength && inputs[i] != 255; i++) {
          inputs[i] = 255;

        }
        cursor = 0;
      } 
      else if (cursor!= 0) {
        movement = getMovement(cursor--);

        for (int i = cursor; i < maxInputLength - 1; i++) {
          inputs[i] = inputs[i+1];
          inputs[i+1] = 255;
        }
        rightShift = true;
      }
    } else if (input == 28) { // ->
      if (inputs[cursor] != 255 && cursor + 1 < maxInputLength) {
          movement = getMovement(cursor++);
        }
    } else if (input == 29) { // <-
      if (cursor != 0) {
        movement = -getMovement(cursor--);
      }
    } else if (input == 31) { // =
      movement = 255;
      cursor = 0;
    }
    
    shift=false;
    digitalWrite(LEDPIN, LOW);
    printCalc(printCursor(movement, rightShift));
    delay(10);
  } while (input != _equ_);
}

short int getMovement(byte cursor) {
  short int movement;
  if (inputs[cursor] >= _hsi_ && inputs[cursor] <= _hta_) {
    movement = 5;
  }
  else if (inputs[cursor] >= _sin_ && inputs[cursor] <= _log_ || inputs[cursor] == abs) {
    movement = 4;
  }
  else if (inputs[cursor] == _ans_ || inputs[cursor] == _xsq_) {
    movement = 3;
  } else if (inputs[cursor] == _sqr_) {
    movement = 2;
  } else {
    movement = 1;
  }

  return movement;
}

byte checkInputs(byte lastInput) { // cap. 4.4
  delay(20);

  byte input = 0;
  byte check = 0;

  while (lastInput != 0) {
    lastInput = 0;
    lastInput += digitalRead(BIT0PIN);
    lastInput += digitalRead(BIT1PIN);
    lastInput += digitalRead(BIT2PIN);
    lastInput += digitalRead(BIT3PIN);
    lastInput += digitalRead(BIT4PIN);
  }

  while (check < 8) {
    input = 0;
    input += digitalRead(BIT0PIN);
    input += digitalRead(BIT1PIN) * 2;
    input += digitalRead(BIT2PIN) * 4;
    input += digitalRead(BIT3PIN) * 8;
    input += digitalRead(BIT4PIN) * 16;

    if (input != 0) {
      input = input | lastInput;
      lastInput = input;
      check++;
    } else {
      check = 0;
      lastInput = 0;
    }
  }
  return input;
}

byte printCursor(short int movement, bool rightShift) { // cap. 4.5
  static byte stringShift = 0;
  static sByte cursorString = 0;
  String cursorPrint = "----------------";

  if (movement == 255) {
    stringShift = 0;
    cursorString = 0;
    movement = 0;
  } else if (rightShift) {
    if (stringShift < movement) {
      short int delta = movement - (short int)stringShift ;
      stringShift = 0;
      cursorString -= delta;
    } else {
      stringShift -= movement;
    }
  } else {
    cursorString += movement;
  }

  if (cursorString > 15) {
    stringShift += cursorString - 15;
    cursorString = 15;
  } else if (cursorString < 0) {
    stringShift += cursorString;
    cursorString = 0;
  }
  

  cursorPrint[cursorString] = 'L';
  lcd.setCursor(0, 1);
  lcd.print(cursorPrint);

  return stringShift;
}

void printCalc(byte stringShift) { // cap. 4.6
  String complete = "";
  byte startPrint = 0;
  byte toAdd = 0;
  
  for (int i = 0; i < maxInputLength && i <= stringShift + 15; i++) {
    switch (inputs[i]) {
      case _0_:
        complete += "0";
        toAdd = 1;
        break;
      case _1_:
        complete += "1";
        toAdd = 1;
        break;
      case _2_:
        complete += "2";
        toAdd = 1;
        break;
      case _3_:
        complete += "3";
        toAdd = 1;
        break;
      case _4_:
        complete += "4";
        toAdd = 1;
        break;
      case _5_:
        complete += "5";
        toAdd = 1;
        break;
      case _6_:
        complete += "6";
        toAdd = 1;
        break;
      case _7_:
        complete += "7";
        toAdd = 1;
        break;
      case _8_:
        complete += "8";
        toAdd = 1;
        break;
      case _9_:
        complete += "9";
        toAdd = 1;
        break;
      case _dot_:
        complete += ".";
        toAdd = 1;
        break;
      case _plu_:
        complete += "+";
        toAdd = 1;
        break;
      case _min_:
        complete += "-";
        toAdd = 1;
        break;
      case _for_:
        complete += "*";
        toAdd = 1;
        break;
      case _div_:
        complete += "/";
        toAdd = 1;
        break;
      case _opa_:
        complete += "(";
        toAdd = 1;
        break;
      case _cpa_:
        complete += ")";
        toAdd = 1;
        break;
      case _pow_:
        complete += "^";
        toAdd = 1;
        break;
      case _sqr_:
        complete += "\xE8("; // radice quadrata
        toAdd = 2;
        break;
      case _sin_:
        complete += "sin(";
        toAdd = 4;
        break;
      case _cos_:
        complete += "cos(";
        toAdd = 4;
        break;
      case _tan_:
        complete += "tan(";
        toAdd = 4;
        break;
      case _log_:
        complete += "log(";
        toAdd = 4;
        break;
      case _ln_:
        complete += "ln(";
        toAdd = 3;
        break;
      case _abs_:
        complete += "abs(";
        toAdd = 4;
        break;
      case _ans_:
        complete += "ans";
        toAdd = 3;
        break;
      case _xsq_:
        complete += "]\xE8("; // radice quadrata
        toAdd = 3;
        break;
      case _hsi_:
        complete += "asin(";
        toAdd = 5;
        break;
      case _hco_:
        complete += "acos(";
        toAdd = 5;
        break;
      case _hta_:
        complete += "atan(";
        toAdd = 5;
        break;
      case _pi_:
        complete += "\xF7"; // pi greco
        toAdd = 1;
        break;
      case _X_:
        complete += "X";
        toAdd = 1;
        break;
      case _Y_:
        complete += "Y";
        toAdd = 1;
        break;
      case _e_:
        complete += "e";
        toAdd = 1;
        break;
      default:
        complete += " ";
        break;
    }

    if (complete.length() < stringShift) {
      complete = "";
      stringShift -= toAdd;
    }
  }
  
  lcd.setCursor(0, 0);
  lcd.print(complete.substring(stringShift));
}

byte fromInputToEquation() { // cap. 4.7
  Serial.println("nc");
	byte numNum = 0;
	byte opNum = 0;
	byte parNum = 0;

	bool isPositive = true;
	short int digitOverZero = 0;
	byte lastParToClose = 0;

	for (int i = 0; i < maxInputLength; i++) {
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(inputs[i]);
    Serial.print("error = ");
  Serial.println(error);
		if (inputs[i] <= _dot_) { // e' una cifra o un punto
			if (i > 0 && inputs[i - 1] == _cpa_ || inputs[i - 1] >= _pi_ || inputs[i - 1] == _ans_) {
				return SERROR;
			}


			if (inputs[i] == _dot_) {
				if (digitOverZero >= 0) {
					digitOverZero = -1;
				}
				else {
					return SERROR;
				}
			}
			else {
				if (digitOverZero >= 0) {
					numbers[numNum] = fp64_add(fp64_mul(numbers[numNum], fp64_sd(10.f)), fp64_sd(float(inputs[i] - 1)));
					digitOverZero = 1;
				}
				else {
          float64_t toAdd = fp64_mul(fp64_sd(float(inputs[i] - 1)), fp64_pow(float64_t(10.f), float64_t(float(digitOverZero))));
					numbers[numNum] = fp64_add(numbers[numNum], toAdd);
          digitOverZero--;
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
		else if (inputs[i] == _cpa_ || inputs[i] == _xsq_ && digitOverZero != 0) {
			
      if (inputs[i-1] == _opa_) {
        return SERROR;
      }

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
			
			if (inputs[i] == _xsq_) {
				operators[opNum++] = _xsq_;

				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;

				pars[++parNum][0] = numNum;

				if (lastParToClose == 0) {
					lastParToClose = parNum;
				}
			}
		}
    else if (inputs[i] == 255) {
      if (digitOverZero == 0 || inputs[i-1] == _opa_) {
        Serial.println("SERROR");
        return SERROR;
      }

      numbers[numNum] *= 1 + 2 * (-1 + isPositive);
			pars[0][1] = numNum;

			for (int j = parNum; j >= lastParToClose; j--) {
				if (pars[j][1] == 255) {
					pars[j][1] = numNum;
					if (j == lastParToClose) {
						lastParToClose = 0;
					}
				}
			}
      Serial.println("yeah");
			return 0;
		}
    else if (inputs[i] >= _pi_ || inputs[i] == _ans_) {
			if (digitOverZero != 0) {
				operators[opNum++] = _for_;
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
			}
			switch (inputs[i]) {
			case _pi_:
				numbers[numNum] = fp64_sd(3.141592653589);
				break;
      case _e_:
				numbers[numNum] = fp64_sd(2.718281828459);
				break;
      case _X_:
				numbers[numNum] = x;
				break;
      case _Y_:
				numbers[numNum] = y;
				break;
      case _ans_:
        numbers[numNum] = ans;
        break;
			default:
				return PERROR;
				break;
			}
			if (inputs[i + 1] <= _dot_) {
				return 2;
			}
			digitOverZero = 1;
		}
		else {
			return SERROR;
		}
	}
}

float64_t solve() { // cap. 4.8

	static byte parAnalyzed = 0;
	byte currentPar = parAnalyzed;

	while (pars[parAnalyzed + 1][1] != 255 && pars[currentPar][1] >= pars[parAnalyzed + 1][0] && parAnalyzed < maxInputLength / 2 + 1) {
		parAnalyzed++;
		byte firstParAnalyzed = parAnalyzed;
		numbers[pars[firstParAnalyzed][0]] = solve();
    if (error != 0) {
      return error;
    }
		removeData(pars[firstParAnalyzed][0], pars[firstParAnalyzed][1] - pars[firstParAnalyzed][0], currentPar, firstParAnalyzed);
	}

	if (error != 0) {
		return error;
	}

	float64_t sol = 0;

	for (short int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // ^ & xsqr
		if (operators[i] == _pow_) {
			numbers[i] = fp64_pow(numbers[i], numbers[i+1]);
			removeData(i, 1, currentPar, currentPar);
		}
		else if (operators[i] == _xsq_) {
			if (numbers[i + 1] < 0) {
				error = MERROR;
				return MERROR;
			}
			numbers[i] = fp64_pow(numbers[i], fp64_div( fp64_sd(1.f), numbers[i+1]));
			removeData(i, 1, currentPar, currentPar);
		}
	}

	byte firstNOfForDiv = 255;
	for (int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // * & /
    if (operators[i] == _for_) {
      firstNOfForDiv = (firstNOfForDiv == 255) ? i : firstNOfForDiv;
			numbers[firstNOfForDiv] = fp64_mul(numbers[firstNOfForDiv], numbers[i + 1]);
			numbers[i + 1] = fp64_sd (0.f);
			operators[i] = _plu_;
    }
		else if (operators[i] == _div_) {
			firstNOfForDiv = (firstNOfForDiv == 255) ? i : firstNOfForDiv;
			numbers[firstNOfForDiv] = fp64_div(numbers[firstNOfForDiv], numbers[i + 1]);
			numbers[i + 1] = fp64_sd (0.f);
			operators[i] = _plu_;
		} else {
      firstNOfForDiv = 255;
    }
	}

	sol += numbers[pars[currentPar][0]];
	for (int i = pars[currentPar][0] + 1; i <= pars[currentPar][1]; i++) { // + & -
    if (operators[i - 1] == _plu_) {
      sol = fp64_add(sol, numbers[i]);
    } else {
      sol = fp64_sub(sol, numbers[i]);
    }
	}

	bool isPositive = (pars[currentPar][2] > 0x7F);
	pars[currentPar][2] = pars[currentPar][2] & 0x7F;

	switch (pars[currentPar][2]) {
	case _sqr_:
		if (sol < 0) {
			error = MERROR;
			return MERROR;
		}
		sol = fp64_sqrt(sol);
		break;
	case _sin_:
		sol = fp64_sin(sol);
		break;
	case _cos_:
		sol = fp64_cos(sol);
		break;
	case _tan_:
		sol = fp64_tan(sol);
		break;
	case _log_:
		if (sol <= 0) {
			error = MERROR;
			return MERROR;
		}
		sol = fp64_log10(sol);
		break;
	case _ln_:
		if (sol <= 0) {
			error = MERROR;
			return MERROR;
		}
		sol = fp64_log(sol);
		break;
	case _abs_:
		sol = fp64_abs(sol);
		break;
	case _hsi_:
		if (sol < -1 || sol > 1) {
			error = MERROR;
			return MERROR;
		}
		sol = fp64_asin(sol);
		break;
	case _hco_:
		if (sol < -1 || sol > 1) {
			error = MERROR;
			return MERROR;
		}
		sol = fp64_acos(sol);
		break;
	case _hta_:
		sol = fp64_atan(sol);
		break;
	}

	if (currentPar == 0) { // Ultima iterazione
		parAnalyzed = 0;
	}

	return sol * (1 - 2 * isPositive);
}

void removeData(byte firstNum, short int delta, byte currentPar, byte firstParAnalyzed) { // cap. 4.9

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

void printSol() { // cap. 4.10
  lcd.clear();
  printCalc(0);
  lcd.setCursor(0,1);
  switch (error) {
    case 0:
      lcd.print(fp64_to_decimalExp(ans, 9, 0, NULL));
      break;
    case SERROR:
      lcd.print("Syntax Error");
      break;
    case MERROR:
      lcd.print("Math Error");
      break;
    case PERROR:
      lcd.print("System Error");
      break;
    Default:
      lcd.print("Mannaggia");
      break;
  }
}

void inBetween() { // cap. 4.11
  byte input = 0;
  bool shift;
  bool store;

  while (input == 0) {
    input = checkInputs(input);
    if (input == 30) { // shift
      shift = !shift;
      digitalWrite(LEDPIN, shift);
      continue;
    } else if (shift & input == 31) { // store
      store = !store;
      continue;
    } else if (store && input == 26) { // X
      x = ans;
      store = false;
      continue;
    } else if (store && input == 25) { // Y
      y = ans;
      store = false;
      continue;
    }
  }
}

void newCalc() { // cap. 4.12
  error = 0;
  for (int i = 0; i < maxInputLength / 2 + 1; i++) {
    numbers[i] = 0;
  }
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
