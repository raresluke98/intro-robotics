const int potPinR = A2;
const int potPinG = A1;
const int potPinB = A0;

const int ledPinR = 11;
const int ledPinG = 10;
const int ledPinB = 9;

int potValueR = 0;
int potValueG = 0;
int potValueB= 0;
int ledValueR= 0;
int ledValueG= 0;
int ledValueB= 0;
int time;

void setup(){
  Serial.begin(9600);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
}

void loop(){
  potValueR = analogRead(potPinR);
  potValueG = analogRead(potPinG);
  //potValueB = analogRead(potPinB);
  ledValueR = map(potValueR, 0, 1023, 0, 255);
  ledValueG = map(potValueG, 0, 1023, 0, 255);
  //ledValueB = map(potValueB, 0, 1023, 0, 255);
  analogWrite(ledPinR, ledValueR);
  analogWrite(ledPinG, ledValueG);
  Serial.println(potValueR);
  //analogWrite(ledPinB, ledValueB);
  delay(1);
}
