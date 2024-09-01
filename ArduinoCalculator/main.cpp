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
#define _pow_ 18 // ^
#define _sqr_ 19 // #
#define _sin_ 20 // s
#define _cos_ 21 // c
#define _tan_ 22 // t
#define _log_ 23 // l
#define _ln_ 24  // n
#define _abs_ 25 // a
#define _ans_ 26 // DA IMPLEMENTARE
#define _xsq_ 27 // [
#define _hsi_ 28 // d
#define _hco_ 29 // v
#define _hta_ 30 // z
#define _pi_ 31  // p
#define _B_ 32   // DA IMPLEMENTARE
#define _C_ 33   // DA IMPLEMENTARE
#define _A_ 34   // DA IMPLEMENTARE

#define PI 3.14159265358979323846
#define maxInputLength 50

short int input[maxInputLength];
double numbers[maxInputLength / 2 + 1];
short int operators[maxInputLength / 2];
short int fromInputToEquation();
short int pars[maxInputLength / 2 + 1][3];
double ans;
void init();
void removeData(short int& firstNum, short int delta, short int& currentPar, short int& firstParAnalyzed);
double solve();

int main() {
	init();

	std::string textInput = "sp/2)";
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
		case '^':
			input[i] = _pow_;
			break;
		case '#':
			input[i] = _sqr_;
			break;
		case 's':
			input[i] = _sin_;
			break;
		case 'c':
			input[i] = _cos_;
			break;
		case 't':
			input[i] = _tan_;
			break;
		case 'l':
			input[i] = _log_;
			break;
		case 'n':
			input[i] = _ln_;
			break;
		case 'a':
			input[i] = _abs_;
			break;
		case '[':
			input[i] = _xsq_;
			break;
		case 'd':
			input[i] = _hsi_;
			break;
		case 'v':
			input[i] = _hco_;
			break;
		case 'z':
			input[i] = _hta_;
			break;
		case 'p':
			input[i] = _pi_;
			break;
		default:
			std::cout << "Hai dimenticato di aggiungere qualcosa alla lista idiota";
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
	static short int parAnalyzed = 0;
	short int currentPar = parAnalyzed;

	std::cout << "currentPar = " << currentPar << std::endl;
	std::cout << "par starting on " << pars[currentPar][0] << " and par ending on " << pars[currentPar][1] << std::endl;


	while (pars[parAnalyzed][1] != 255) {

		parAnalyzed++;
		if (pars[currentPar][1] >= pars[parAnalyzed][0]) {

			short int firstParAnalyzed = parAnalyzed;
			numbers[pars[firstParAnalyzed][0]] = solve();
			removeData(pars[firstParAnalyzed][0], pars[firstParAnalyzed][1] - pars[firstParAnalyzed][0], currentPar, firstParAnalyzed);

		}
		else {
			parAnalyzed--;
			break;
		}
	}

	std::cout << "end par\n";
	for (int i = pars[currentPar][0]; i <= pars[currentPar][1]; i++) {
		std::cout << numbers[i] << "\nop: " << operators[i] << std::endl;
	}

	for (short int i = pars[currentPar][0]; i < pars[currentPar][1]; i++) { // ^ & xsqr
		if (operators[i] > _div_) {
			numbers[i] = (operators[i] == _pow_) ?pow(numbers[i], numbers[i + 1]) : numbers[i] = pow(numbers[i+1], 1.f / numbers[i]);;
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
	std::cout << "after * \n";
	for (int i = pars[currentPar][0]; i <= pars[currentPar][1]; i++) {
		std::cout << numbers[i] << "\nop: " << operators[i] << std::endl;
	}
	sol += numbers[pars[currentPar][0]];
	for (int i = pars[currentPar][0] + 1; i <= pars[currentPar][1]; i++) { // + & -
		sol += (operators[i - 1] == _plu_) ? numbers[i] : -numbers[i];
	}

	bool isPositive = (pars[currentPar][2] > 0x7F);
	pars[currentPar][2] = pars[currentPar][2] & 0x7F;
	switch (pars[currentPar][2]) {
	case _sqr_:
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
		sol = tan(sol);
		break;
	case _ln_:
		sol = tan(sol);
		break;
	case _abs_:
		sol = abs(sol);
		break;
	case _hsi_:
		sol = asin(sol);
		break;
	case _hco_:
		sol = acos(sol);
		break;
	case _hta_:
		sol = atan(sol);
		break;
	}
	std::cout << "current sol = " << sol << std::endl;
	return sol * (1 - 2 * isPositive);
}

short int fromInputToEquation() {
	short int numNum = 0; // numero di numeri
	short int opNum = 0; // numero di operatori
	short int parNum = 0;
	short int funcNum = 0;

	bool isPositive = true;
	short int digitOverZero = 0; // 1 = true, -1 = false, 0 = sconosciuto
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
		else if (digitOverZero != 0 && (input[i] <= _div_ || input[i] == _pow_)) {
			digitOverZero = 0;
			operators[opNum++] = input[i];
			numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1	
			isPositive = true;
			std::cout << "-> " << numNum - 1 << " number = " << numbers[numNum - 1] << "\n - operator at i = " << i << " -> operator = " << input[i] << std::endl;
		}
		else if (input[i] == _min_) {
			isPositive = !isPositive;
		}
		else if (input[i] == _plu_) {
			continue;
		}
		else if (input[i] == _opa_ || (input[i] >= _sqr_ && input[i] <= _abs_) || (input[i] >= _hsi_ && input[i] <= _hta_)) {

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

			if (input[i] != _opa_) {
				pars[parNum][2] = pars[parNum][2] | input[i];
			}

			std::cout << "ParNum = " << parNum << std::endl;
		}
		else if (input[i] == _cpa_ || input[i] == _xsq_) {	
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
						std::cout << " -> pars[" << j << "][1] = " << pars[j][1] << " (input is "  << input[i] << ")" << std::endl;
						break;
					}
				}
			}
			else {
				return 1;
			}

			if (input[i] == _xsq_) {
				operators[opNum++] = input[i];
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
				
				pars[++parNum][2] = (1 - isPositive) * 0x80;
				pars[parNum][0] = numNum;
				if (lastParToClose == 0) {
					lastParToClose = parNum;
				}
			}
		}
		else if (input[i] == 255) {
			pars[0][1] = numNum;
			for (int j = parNum; j >= lastParToClose; j--) {
				if (pars[j][1] == 255) {
					pars[j][1] = numNum;
					if (j == lastParToClose) {
						lastParToClose = 0;
					}
					std::cout << " -> (at the end) pars[" << i << "][1] = " << pars[i][1] << std::endl;
					break;
				}
			}
			return 0;
		}
		else if (input[i] >= _pi_) {
			if (digitOverZero != 0) {
				operators[opNum++] = _for_;
				numbers[numNum++] *= 1 + 2 * (-1 + isPositive); // se il numero e' negativo allora sara' moltiplicato per -1
				isPositive = true;
			}
			switch (input[i]) {
			case _pi_:
				numbers[numNum] = PI;
				break;
			default:
				return 2;
				break;
			}
			if (input[i + 1] <= _dot_) {
				return 2;
			}
			digitOverZero = 1;
		}
		else if (input[i] != 255) {
			return 2;
		}
	}
}

void removeData(short int& firstNum, short int delta, short int& currentPar, short int& firstParAnalyzed) {

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

void init() {
	ans = 0;
	pars[0][0] = 0;
	pars[0][2] = 0;
	for (int i = 1; i < maxInputLength / 2 + 1; i++) {
		pars[i][0] = maxInputLength;
		pars[i][1] = 255;
		pars[0][2] = 0;
	}

	for (int i = 0; i < maxInputLength; i++) {
		input[i] = 255;
	}
}