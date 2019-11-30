// Lcd menu

#include <LiquidCrystal.h>

const int rsPin = 12;
const int ePin = 11;
const int d4Pin = 5;
const int d5Pin = 4;
const int d6Pin = 3;
const int d7Pin = 2;

const int joyXPin = A1;
const int joyYPin = A0;

bool joyMovedX = false;
bool joyMovedY = false;
int minThreshold = 400;
int maxThreshold = 600;
int modifyState = 1;

LiquidCrystal lcd(rsPin, ePin, d4Pin, d5Pin, d6Pin, d7Pin);

void setup(){
  //setup the LCD's number of columns and rows
  lcd.begin(16,2);
  Serial.begin (9600);
  //lcd.print("Hello, world!");
}

int xValue;
int yValue;
int number=0;

int menu=1;
int menuType=1;

void Menu(){
  switch(menu){
    case 0:
    menu = 1;
    break;
    case 1:
    lcd.clear();
    lcd.print("                     ");
    lcd.setCursor(0,0);
    lcd.print(">Play  HS");
    //delay(1);
    lcd.setCursor(0,1);
    //lcd.print("         ");
    //lcd.setCursor(0,0);
    lcd.print(" Settings");
    break;
    case 2:
    lcd.clear();
    lcd.print("                     ");
    lcd.setCursor(0,0);
    lcd.print(" Play >HS");
    //delay(1);
    lcd.setCursor(0,1);
    //lcd.print("         ");
    //lcd.setCursor(0,0);
    lcd.print(" Settings");
    break;
    case 3:
    lcd.clear();
    lcd.print("                     ");
    lcd.setCursor(0,0);
    lcd.print(" Play  HS");
    //delay(1);
    lcd.setCursor(0,1);
    //lcd.print("         ");
    //lcd.setCursor(0,0);
    lcd.print(">Settings");
    break;
    case 4:
    menu = 3;
    break;
  }
}

int lives=1;
int level=2;
int score=0;
int startTime=millis();

void Menu1(){
  score=((millis()-startTime)/1000)*level;
  lcd.clear();
  lcd.print("lives:");
  lcd.print(lives);
  lcd.print(" level:");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print("score: ");
  lcd.print(score); 
}

char incomingByte = 0;
String username="";
int highscore=9;
bool stringComplete=false;

void Menu2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(username);
  lcd.print(" ");
  lcd.print(highscore);
}

void Menu3(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level: ");
  lcd.print(level);
  lcd.setCursor(0,1);
  lcd.print(username);
}

void Menu31(){
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reading...");
  if (Serial.available() > 0) {
    incomingByte=Serial.read();
    username+=incomingByte;
    if(incomingByte=='\n'){
      stringComplete=true;
    }
  }
}


void loop(){
  lcd.setCursor(4, 1);
  //lcd.print(millis() /1000);
  xValue = analogRead(joyXPin);
   if (xValue < minThreshold && joyMovedX == false && modifyState == 1) {
     if(menuType>=1){
      menuType--;
    }
    joyMovedX = true;
  }
   if (xValue > maxThreshold && joyMovedX == false && modifyState == 1) {
    if(menuType<=3){
      menuType++;
    }
   
    joyMovedX = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMovedX = false;
  }


  yValue = analogRead(joyYPin);
   if (yValue < minThreshold && joyMovedY == false && modifyState == 1) {
    menu++;
    joyMovedY = true;
  }
   if (yValue > maxThreshold && joyMovedY == false && modifyState == 1) {
    menu--;
    joyMovedY = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMovedY = false;
  }
  if(menuType==1){
    Menu();
  } else if(menu==1){
    Menu1();
  } else if(menu==2){
    Menu2();
  } else if(menu==3){
    Menu3();
    if(menuType==3)
      lcd.clear(),Menu31();
  }
  
  
  //delay(1);
  
  //lcd.print(number);
  //lcd.print("    ");
}
