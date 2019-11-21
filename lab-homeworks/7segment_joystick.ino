///////7segment 4 digit joystick//////
/////////good version////////!!!!!!!!!!
// declare all the joystick pins
const int pinSW = A2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
// declare all the segments pins
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;
//dp on or off
int dpState = LOW;
// segments array, similar to before
int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};
byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
for (int i = 0; i < segSize - 1; i++) {
digitalWrite(segments[i], digitMatrix[digit][i]);
}
// write the decimalPoint to DP pin
digitalWrite(segments[segSize - 1], decimalPoint);
}
// activate the display no. received as param
void showDigit(int num) {
for (int i = 0; i < noOfDisplays; i++) {
digitalWrite(digits[i], HIGH);
}
digitalWrite(digits[num], LOW);
}



///joystick variables
int swState = 0;
int lastSwState = 0;
int modifyState = 1;
int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;


int unit=1;
int currentNumber=1111;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool modify = 0;

unsigned long lastBlinkTime =0;
unsigned long blinkDelay = 200;

void setup() {
  Serial.begin(9600);
  // joystick initialization
  for (int i = 0; i < segSize; i++) {
     pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);

  displayNumber(digit, dpState); // initial value displayed

  /////7seg initialization
  for (int i = 0; i < segSize - 1; i++)
{
pinMode(segments[i], OUTPUT);
}
for (int i = 0; i < noOfDisplays; i++)
{
pinMode(digits[i], OUTPUT);
}
}



void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  // On Ox axis, if the value is lower than a chosen min threshold, then
  // decrease by 1 the digit value.
  if (xValue < minThreshold && joyMovedX == false && modifyState == 1) {
    if (currentNumber%(unit*10)/unit > 0) {
      currentNumber= currentNumber-unit;
    } else {
      currentNumber=currentNumber+9*unit;
    }
    joyMovedX = true;
  }
  if (yValue > maxThreshold && joyMovedY == false && modifyState == 0) {
    if (unit > 1) {
      unit/=10;
    } else {
      unit = 1000;
    }
    joyMovedY = true;
  }
  // On Ox axis, if the value is bigger than a chosen max threshold, then
   // increase by 1 the digit value
  if (xValue > maxThreshold && joyMovedX == false && modifyState == 1) {
  if (currentNumber%(10*unit)/unit < 9) {
    currentNumber=currentNumber+unit;
  } else {
    currentNumber=currentNumber-9*unit;
  }
  joyMovedX = true;
  }
  if (yValue < minThreshold && joyMovedY == false && modifyState == 0) {
  if (unit < 1000) {
    unit*=10;
  } else {
    unit = 1;
  }
  joyMovedY = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMovedX = false;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMovedY = false;
  }
  ///////debounce.
  int reading = digitalRead(pinSW);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if((millis() - lastDebounceTime) > debounceDelay){
   if(reading != swState){
    swState = reading;
    if( swState == HIGH){
      modifyState = !modifyState;
    }
   }
  }
  Serial.print(modifyState);
  Serial.print("   ");
  Serial.println(currentNumber);
  
  lastSwState = reading;
  int number;
  int digit;
  int lastDigit;
  number = currentNumber;
  digit = 0;
  int cursorPosition =1;
  do  {
  lastDigit = number % 10; // get the last digit
  showDigit(digit);
  if(unit == cursorPosition){
    if(modifyState==1){
      if(millis()-lastBlinkTime < blinkDelay)
        displayNumber(lastDigit, HIGH);
      else if(millis()-lastBlinkTime > blinkDelay)
        displayNumber(lastDigit, LOW);
      if(millis() - lastBlinkTime > blinkDelay*2)
         lastBlinkTime=millis();
    }
    else{
      displayNumber(lastDigit, HIGH);
    }
  }
  else
    displayNumber(lastDigit, LOW); 
  // increase this delay to see multiplexing in action. At about 100 it
  //becomes obvious
  delay(2);
  digit++; // move to next display
  cursorPosition*=10;
  number = number / 10;
}while(number != 0);
  
  //displayNumber(digit, dpState);
  delay(1);
}
