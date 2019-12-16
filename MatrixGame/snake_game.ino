#include <LedControl.h>

#include <LiquidCrystal.h>

#include <EEPROM.h>
//lcd pins


const int rsPin = 7;
const int ePin = 6;
const int d4Pin = 5;
const int d5Pin = 4;
const int d6Pin = 3;
const int d7Pin = 2;

//lcd init

LiquidCrystal lcd(rsPin, ePin, d4Pin, d5Pin, d6Pin, d7Pin);

/* Joystick PINs */
const int joyXPin = A0;
const int joyYPin = A1;
const int joySwitch   = 9;

bool joyMovedX = false;
bool joyMovedY = false;
int minThreshold = 300;
int maxThreshold = 700;

/* Display PINs */
const int clockPin = 11;
const int loadPin = 10;
const int dinPin = 12;

const int size = 8;
int advanceDelay = EEPROM.read(0);


int snake[size*size][2];
int length;
int food[2], v[2];
bool is_game_over = false;
long current_time;
long prev_advance;
int blink_count;
const short messageSpeed = 5;



LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

void init_game() {
  prev_advance = current_time = 0;
  blink_count = 3;
  int half = size / 2;
  length = size / 3;

  for (int i = 0; i < length; i++) {
      snake[i][0] = half - 1;
      snake[i][1] = half + i;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  v[0] = 0;
  v[1] = -1;
}

void render() {
  for (int i = 0; i < length; i++) {
    lc.setLed(0, snake[i][0], snake[i][1], 1);
  }
  lc.setLed(0, food[0], food[1], 1);
}

void clearScreen() {
  for (int x = 0; x < size; x++) {
    for (int y = 0; y < size; y++) {
      lc.setLed(0, x, y, 0);
    }
  }
}

/**
 * moves the snake forward
 * returns true if the game is over
 */

int score=0;
 
bool advance() {
  int head[2] = {snake[0][0] + v[0], snake[0][1] + v[1]};

  /*if (head[0] < 0 || head[0] >= size) {

            delay(1000);
    showGameOverMessage();
      return true;
  }
  */
  /*
  if (head[1] < 0 || head[1] >= size) {

            delay(1000);
    showGameOverMessage();
      return true;
  }
  */
  for (int i = 0; i < length; i++) {
      if (snake[i][0] == head[0] && snake[i][1] == head[1]) {
            delay(1000);
        showGameOverMessage();
          return true;
      }
  }

  bool grow = (head[0] == food[0] && head[1] == food[1]);
  if (grow) {
      score++;
      length++;  
      randomSeed(current_time);    
      food[0] = random(size);
      food[1] = random(size);
  }

  for (int i = length - 1; i >= 0; i--) {
      snake[i + 1][0] = snake[i][0];
      snake[i + 1][1] = snake[i][1];
  }
  if(head[0] < 0){
    snake[0][0]=size-1;
  } else if(head[0] >= size){
    snake[0][0]=0;
  } else { snake[0][0] += v[0]; }
  //snake[0][1] += v[1];
  if(head[1] <0){
    snake[0][1]=size-1;
  } else if(head[1] >= size){
    snake[0][1]=0;
  } else {snake[0][1] +=v[1];}
  return false;
}

int menuState = 1;
int startMenuOption = 1;
//int prevStartMenuOption = 1;

int xValue;
int yValue;

void readJoy(){
   xValue = analogRead(joyXPin);
   if (xValue < minThreshold && joyMovedX == false && menuState==1 ) {
     if(startMenuOption<3){
      startMenuOption++;
    }
    joyMovedX = true;
  }
   if (xValue > maxThreshold && joyMovedX == false && menuState==1 ) {
    if(startMenuOption>1){
      startMenuOption--;
    }
   
    joyMovedX = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMovedX = false;
  }

  yValue = analogRead(joyYPin);
   if (yValue < minThreshold && joyMovedY == false ) {
     if(menuState>1){
      menuState--;
    }
    joyMovedY = true;
  }
   if (yValue > maxThreshold && joyMovedY == false ) {
    if(menuState<3){
      menuState++;
    }
   
    joyMovedY = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMovedY = false;
  }
}

bool inGame = false;
String username;
char incomingByte;
bool stringComplete=false;
int temp;
int address = 0;


void showMenu(){
  delay(25);
  if(menuState == 1){
    
    //if(startMenuOption!=prevStartMenuOption){
    if(menuState==1){
      if(startMenuOption == 1){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">Start  HS");
        lcd.setCursor(0,1);
        lcd.print(" Options");
      }
        if(startMenuOption == 2){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Start >HS");
        lcd.setCursor(0,1);
        lcd.print(" Options");
      }
      if(startMenuOption == 3){
         lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Start  HS");
        lcd.setCursor(0,1);
        lcd.print(">Options");
      }
    }
   }
   else if(menuState==2){
      if(startMenuOption == 1){
        lcd.clear();
        lcd.setCursor(0,0);
       // lcd.print("Playing...S:");
        //lcd.print(length-2);
        inGame=true;
      }
      if(startMenuOption == 2){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Highscore: lol");
        lcd.setCursor(0,1);
        lcd.print("Username: ");
        lcd.print(username);
      }
      if(startMenuOption == 3){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Difficulty: ");
        lcd.setCursor(0,1);
        lcd.print(20-advanceDelay);
      }
   } else if(menuState==3){
     if(startMenuOption == 1){
      menuState--;
     }
     if(startMenuOption == 2){
      lcd.print("Reading...");
      if (Serial.available() > 0) {
        incomingByte=Serial.read();
        username+=incomingByte;
        if(incomingByte=='\n'){
          stringComplete=true;
        }
      }
     }
      if(startMenuOption == 3){
      lcd.print("Reading...");
      if (Serial.available() > 0) {
        temp=Serial.parseInt();
        advanceDelay=20-temp;
        if(advanceDelay!=EEPROM.read(0))
            EEPROM.write(0,advanceDelay);
        }
      }
   }
  
}

int currentDelay=advanceDelay;
long startTime=0;
long currentTime;


void runGame(){
  if (!is_game_over) {
    clearScreen();
    render();
    currentTime=millis()/300;
    if((currentTime-startTime)>currentDelay&&currentDelay>1){
      currentDelay--;
      startTime=currentTime;
    }
    lcd.setCursor(0,0);
    lcd.print("Playing...");
    lcd.setCursor(0,1);
    lcd.print("            ");
    lcd.setCursor(0,1);
    lcd.print("S: ");
    lcd.print(score);
    lcd.print(" Diff: ");
    lcd.print(20-currentDelay);
    if (current_time - prev_advance > currentDelay) {
      is_game_over = advance();
      prev_advance = current_time;    
    }
  } else {
    while (blink_count > 0) {
      clearScreen();
      delay(300);
      render();
      delay(300);
      blink_count--;     
         
    }
  }
  readControls();
  current_time++;
}




/////////////////////////setup/////////////////////////////////
void setup() {
  lcd.setCursor(4, 1);
  lcd.begin(16,2);
  Serial.begin(9600);
  showMenu();
  
  pinMode(joySwitch, INPUT_PULLUP);
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(joySwitch), restart, RISING);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);

  init_game();
  render();
}

///////////////////////////loop///////////////////////////////////

void loop() {
  //runGame();
  if(inGame==false){
    readJoy();
    showMenu();
  } else {
    runGame();
  }
}

void restart() {  
  init_game();
  is_game_over = false;
}

void readControls() {
  int dx = map(analogRead(joyXPin), 0, 906, 2, -2);
  int dy = map(analogRead(joyYPin), 0, 906, -2, 2);
  if (dx != 0) {dx = dx / abs(dx);}
  if (dy != 0) {dy = dy / abs(dy);}

  if (dy != 0 && v[0] != 0) {
    v[0] = 0;
    v[1] = dy;    
  }

  if (dx != 0 && v[1] != 0) {
    v[0] = dx;
    v[1] = 0;
  }

}

const PROGMEM bool gameOverMessage[8][90] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0}
}; 
void showGameOverMessage() {
  for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
    for (int col = 0; col < 8; col++) {
      delay(messageSpeed);
      for (int row = 0; row < 8; row++) {
        // this reads the byte from the PROGMEM and displays it on the screen
        lc.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d])));
      }
    }
  }
  
}
