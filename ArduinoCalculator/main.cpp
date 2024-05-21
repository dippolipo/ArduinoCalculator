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

	std::string textInput = "(10-4)*2*3+1";
	std::cout << "operation = " << textInput << std::endl;
	std::cout << "input length = " << textInput.length() << std::endl;
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
	switch (error) {
	case 0:
		std::cout << "Trying to solve\n\n";
		ans = solve();
		std::cout << "ans = " << ans;
		break;
	case 1:
		std::cout << "User Error";
		break;
	case 2:
		std::cout << "System Error";
		break;
	}

	return 0;
}

double solve() {

	double sol = 0;
	static short int currentPar = 0;

	std::cout << "currentPar = " << currentPar << std::endl;
	std::cout << "par starting on " << pars[currentPar][0] << " and par ending on " << pars[currentPar][1] << std::endl;

	if (pars[currentPar][1] > pars[currentPar + 1][0]) {
		
		numbers[pars[++currentPar][0]] = solve();
		short int delta = pars[currentPar][1] - pars[currentPar][0]; 
		
		for (int i = pars[currentPar][0] + 1; i <= pars[0][1] - delta; i++) {
			numbers[i] = numbers[i + delta];
			operators[i - 1] = operators[i + delta - 1];
		}
		
		for (int i = 1; i < currentPar; i++) {
			if (pars[i][1] >= pars[currentPar][1]) {
				pars[i][1] -= delta;
			}
		}

		for (int i = currentPar; pars[i][0] != maxInputLength; i++) {
			pars[i][0] -= delta;
			pars[i][1] -= delta;
		}
		
		pars[0][1] -= delta;
		currentPar--;
	}

	std::cout << "end par\n";
	for (int i = pars[currentPar][0]; i <= pars[currentPar][1]; i++) {
		std::cout << numbers[i] << "\nop: " << operators[i] << std::endl;
	}
	short int firstNOfForDiv = -1;
	for (int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // * & /
		if (operators[i] > _min_) {
			firstNOfForDiv = (firstNOfForDiv == -1) ? i : firstNOfForDiv;
			numbers[firstNOfForDiv] *= (operators[i] == _for_) ? numbers[i + 1] : (1 / numbers[i + 1]);
			numbers[i + 1] = 0;
			operators[i] = _plu_;
			std::cout << " op sol = " << numbers[firstNOfForDiv] << std::endl;
			continue;
		}
		std::cout << "op " << operators[i] << " at " << i << "\n";
		firstNOfForDiv = -1;
	}
	std::cout << "after * \n";
	for (int i = pars[currentPar][0]; i <= pars[currentPar][1]; i++) {
		std::cout << numbers[i] << "\nop: " << operators[i] << std::endl;
	}
	sol += numbers[pars[currentPar][0]];
	for (int i = pars[currentPar][0] + 1; i <= pars[currentPar][1]; i++) { // + & -
		sol += (operators[i - 1] == _plu_) ? numbers[i] : -numbers[i];
	}
	
	std::cout << "current sol = " << sol << std::endl;
	return sol;
}

short int fromInputToEquation() {
	short int numNum = 0; // numero di numeri
	short int opNum = 0; // numero di operatori
	bool isPositive = true;
	short int digitOverZero = 0; // 1 = true, -1 = false, 0 = sconosciuto
	
	short int parNum = 0;
	short int lastParToClose = 0;
	
	for (int i = 0; i < maxInputLength; i++) {
		std::cout << "i = " << i << " and input = " << input[i] << std::endl;
		if (input[i] <= _dot_) { // e' una cifra o un punto
			if (i > 0) {
				if (input[i - 1] == _cpa_) {
					return 1;
				}
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
		else if (digitOverZero != 0 && input[i] <= _cpa_) {
			if (input[i] < _opa_) {
				digitOverZero = 0;
				operators[opNum++] = input[i];
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1	
				isPositive = true;
				std::cout << "-> " << numNum - 1 << " number = " << numbers[numNum - 1] << "\n - operator at i = " << i << " -> operator = " << input[i] << std::endl;
			} else if (input[i] == _opa_) {
				digitOverZero = 0;
				operators[opNum++] = _for_;
				if (parNum == -1) {
					lastParToClose = parNum;
				}
				pars[parNum++][0] = numNum;
				isPositive = true;
				std::cout << "-> " << numNum - 1 << " number = " << numbers[numNum - 1] << "\n - operator at i = " << i << " -> operator = " << input[i] << std::endl;
			} else if (input[i] == _cpa_) {
				if (parNum == lastParToClose) {
					pars[parNum][1] = numNum;
					lastParToClose = 0;
				} else {
					for (int i = parNum; i >= lastParToClose; i--) {
						if (pars[i][1] == maxInputLength) {
							pars[i][1] = numNum;
							if (i == lastParToClose) {
								lastParToClose = 0;
							}
							std::cout << " -> pars[" << i << "][1] = " << pars[i][1] << std::endl;
							break;
						}
					}
				}
			}		
		}
		else if (input[i] == _min_) {
			isPositive = !isPositive;
		}
		else if (input[i] == _plu_) {
			continue;
		}
		else if (input[i] == _opa_) {
			pars[++parNum][0] = numNum;
			if (lastParToClose == 0) {
				lastParToClose = parNum;
			}
			std::cout << "ParNum = " << parNum << std::endl;
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
	for (int i = 1; i < maxInputLength / 2 + 1; i++) {
		pars[i][0] = maxInputLength;
		pars[i][1] = maxInputLength;
	}

	for (int i = 0; i < maxInputLength; i++) {
		input[i] = 32; //32 non e' rappresentabile da 5 bits
	}
}
