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
void print(short int& cursorPos, short int& lastCursorPos);
void inputs();

int main() {
	init();

	inputs();

	return 0;
}

void inputs() { //converte digitalRead in numeri e chiama se necessario/possibile la funzione print()
	std::string buffer = "";
	static short int cursorPos = 16;
	short int lastCursorPos = 15;
	short int startingPos = 0;
	print(cursorPos, lastCursorPos);
}

void print(short int& cursorPos, short int& lastCursorPos) {
	std::string buffer = "0123456789abcdef0123456789abcdef";
	static short int stringStartingPos = 0;
	static short int inputStartingPos = 0;
	static short int stringCursorPos = 15;
	std::string cursorString = "----------------";
	short int movement = cursorPos - lastCursorPos;
	
	short int lastInputLenght = 2; //Numero provvisorio, da sostuire con la lunghezza di input[lastCursorPos+movement]
	stringCursorPos += movement * (lastInputLenght);
	if (stringCursorPos > 16 || stringCursorPos < 0) {
		stringStartingPos += movement * (lastInputLenght);
		stringCursorPos -= movement * (lastInputLenght);
	}
	
	
	
	std::string output = "";
	for (int i = stringStartingPos; i < stringStartingPos + 16 && i < buffer.length(); i++) {
		output += buffer[i];
	}
	cursorString[stringCursorPos] = 'J';
	std::cout << output << std::endl;
	std::cout << cursorString;
	lastCursorPos = cursorPos;
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
