#include <iostream>
#include <cmath>

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
#define _equ_ 20

#define maxInputLength 50

short int input[maxInputLength];
double numbers[maxInputLength / 2 + 1];
short int operators[maxInputLength / 2];
short int fromInputToEquation();
short int pars[maxInputLength / 2 + 1][2];
double ans;
void init();
double solve();

int main() {
	init();

	std::string textInput = "10.26*-1+5.26";
	std::cout << "operation = " << textInput << std::endl;
	for (int i = 0; i < textInput.length(); i++) {
		switch (textInput[i]) {
		case '0':
			input[i] = _0_;
			break;
		case '1':
			input[i] = _1_;
			break;
		case '2':
			input[i] = _2_;
			break;
		case '3':
			input[i] = _3_;
			break;
		case '4':
			input[i] = _4_;
			break;
		case '5':
			input[i] = _5_;
			break;
		case '6':
			input[i] = _6_;
			break;
		case '7':
			input[i] = _7_;
			break;
		case '8':
			input[i] = _8_;
			break;
		case '9':
			input[i] = _9_;
			break;
		case '.':
			input[i] = _dot_;
			break;
		case '+':
			input[i] = _plu_;
			break;
		case '-':
			input[i] = _min_;
			break;
		case '*':
			input[i] = _for_;
			break;
		case '/':
			input[i] = _div_;
			break;
		case '(':
			input[i] = _opa_;
			break;
		case ')':
			input[i] = _cpa_;
			break;
		}
	}

	int error = fromInputToEquation();
	if (!error) {
		ans = solve();
		std::cout << "ans = " << ans;
	}
	else {
		std::cout << "something went horribly wrong";
	}

	return 0;
}

double solve() {

	double sol = 0;
	static short int currentPar = 0;
	
	if (pars[currentPar][1] > pars[currentPar + 1][0]) {
		numbers[pars[currentPar++][0]] = solve();
		short int delta = pars[currentPar][1] - pars[currentPar][0]; 
		
		for (int i = pars[currentPar][0] + 1; i <= pars[0][1] - delta; i++) {
			numbers[i] = numbers[i + delta];
			operators[i] = operators[i + delta];
		}
		
		for (int i = currentPar; pars[i][0] != maxInputLength; i++) {
			pars[i][0] -= delta;
			pars[i][1] -= delta;
		}
		
		pars[0][1] -= delta;
	}
	
	for (int i = pars[0][0]; i <= pars[0][1]; i++) { // * & /
		if (operators[i] == _for_) {
			numbers[i + 1] *= numbers[i];
			numbers[i] = 0;
			operators[i] = _plu_;
		}
		else if (operators[i] == _div_) {
			numbers[i + 1] = numbers[i] / numbers[i + 1];
			numbers[i] = 0;
			operators[i] = _plu_;
		}
	}

	sol += numbers[pars[0][0]];
	for (int i = pars[0][0] + 1; i <= pars[0][1]; i++) { // + & -
		sol += (operators[i - 1] == _plu_) ? numbers[i] : -numbers[i];
	}
	
	return sol;
}

short int fromInputToEquation() {
	short int numNum = 0; // numero di numeri
	short int opNum = 0; // numero di operatori
	bool isPositive = true;
	short int digitOverZero = 0; // 1 = true, -1 = false, 0 = sconosciuto
	
	short int parNum = 1;
	short int lastParToClose = 0;
	
	for (int i = 0; i < maxInputLength; i++) {
		if (input[i] <= _dot_) { // e' una cifra o un punto
			if (input[i - 1] == _cpa_) {
				return 1;
			}
		
			std::cout << " - number at i = " << i << " -> number = " << input[i] - 1 << std::endl;;

			if (input[i] == _dot_) {
				if (digitOverZero >= 0) {
					digitOverZero = -1;
				}
				else {
					return 1;
				}
			}
			else {
				if (digitOverZero >= 0) {
					numbers[numNum] = numbers[numNum] * 10 + (input[i] - 1);
					digitOverZero = 1;
				}
				else {
					numbers[numNum] += (input[i] - 1) * pow(10, digitOverZero--);
				}
			}
		}
		else if (digitOverZero != 0) {
			digitOverZero = 0;
	
			if (input[i] < _opa_) {
				operators[opNum++] = input[i];
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1	
			} else if (input[i] == _opa_) {
				operators[opNum++] = _for_;
				if (parNum == -1) {
					lastParToClose = parNum;
				}
				pars[parNum++][0] = numNum;
			} else if (input[i] == _cpa_) {
				if (parNum == lastParToClose) {
					pars[parNum][1] = numNum;
				} else {
					for (int i = parNum; parNum >= lastParToClose; i--) {
						if (pars[i][1] == maxInputLength) {
							pars[i][1] = numNum;
							if (i == lastParToClose) {
								lastParToClose = 0;
							}
							break;
						}
					}
				}
			}
		
			isPositive = true;
			std::cout << "-> " << numNum - 1 << " number = " << numbers[numNum - 1] << "\n - operator at i = " << i << " -> operator = " << input[i] << std::endl;
		}
		else if (input[i] == _min_) {
			isPositive = !isPositive;
		}
		else if (input[i] == _plu_) {
			continue;
		}
		else if (input[i] == _opa_) {
			if (lastParToClose == 0) {
				lastParToClose = parNum;
			}
			pars[parNum++][0] = numNum;
		}
		else if (input[i] == 32) {
			pars[0][1] = numNum;
			return 0;
		}
		else if (input[i] != 32) {
			return 2;
		}
	}
}

void init() {
	ans = 0;
	pars[0][0] = 0;
	for (int i = 1; i < maxInputLength; i++) {
		pars[i][0], pars[i][0] = maxInputLength;
	}
	
	for (int i = 0; i < maxInputLength; i++) {
		input[i] = 32; //32 non e' rappresentabile da 5 bits
	}
}
