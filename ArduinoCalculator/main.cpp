#include <LiquidCrystal.h>


#define BIT0PIN 6
#define BIT1PIN 7
#define BIT2PIN 8
#define BIT3PIN 9
#define BIT4PIN 10

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);

  //inizializzazione tastierino
  pinMode(BIT0PIN, INPUT);
  pinMode(BIT1PIN, INPUT);
  pinMode(BIT2PIN, INPUT);
  pinMode(BIT3PIN, INPUT);
  pinMode(BIT4PIN, INPUT);
  
  // inizializzazione lcd
  lcd.begin(16, 2);
}

void loop() {
  int bit0 = digitalRead(BIT0PIN);
  int bit1 = digitalRead(BIT1PIN);
  int bit2 = digitalRead(BIT2PIN);
  int bit3 = digitalRead(BIT3PIN);
  int bit4 = digitalRead(BIT4PIN);
  int buttonID = bit0 + bit1 * 2 + bit2 * 4 + bit3 * 8 + bit4 * 16;
  
  Serial.println(buttonID);

  lcd.setCursor(0, 0);
  if (buttonID == 0) {
    Serial.println("no signal");
    lcd.print("no signal");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else {
    lcd.print("                ");
    lcd.setCursor(0, 0);
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
