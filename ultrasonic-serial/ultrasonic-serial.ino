#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal.h>

SoftwareSerial bolt(6,10);
//pin connection
const int trigPin = A0;
const int echoPin = 8;
const int buzzer = 7;
const int doorPin = 9;
const int ledPin = 13;
Servo door;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int alert = A1;
int alert2 = A2;
float duration, distance;
bool checkB, activateB, reachingOwner = false, doorClosed = true;

//function prototypes
void someoneAtTheDoor();
void openDoor();
void check();
void activate();
void closeDoor();

void setup() {
  //initializing the software and hardware serial
  Serial.begin(9600);
  bolt.begin(9600);
  //initializing the pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(alert, OUTPUT);
  pinMode(alert2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  door.attach(doorPin);
  checkB = true;
  activateB = false;
  
}
void loop() 
{
  if(!reachingOwner)
    lcd.clear();
  if(checkB)
    check();
  else if(activateB)
    activate();
  
}

//for check thread
void check()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.println(distance);
  if(distance <= 25 && distance > 1) {
    reachingOwner = true;
    lcd.clear();
    digitalWrite(buzzer, HIGH);
    delay(20);
    lcd.setCursor(0, 0);
    lcd.print("Reaching owner.");
    lcd.setCursor(0, 1);
    lcd.print("Please wait...");
    digitalWrite(buzzer, LOW);
    checkB = false;
    activateB = true;
    digitalWrite(ledPin, HIGH);
    return;
  } 
  else {
    reachingOwner = false;
    digitalWrite(alert, LOW);
  }
  delay(1000);
}

//for activate thread
void activate()
{
  //arduino to the cloud
  digitalWrite(alert, HIGH);

  //waiting for the users reply
  String msg1, msg2;
  unsigned long start = millis();
  bool flag = false;
  char letter, command = '\0';
  String message = "";
  //waiting till there is something on the software serial for the Serial
  //Serial.println(start);
  while(millis() < 180000+start){
    //Serial.println(millis());
    if(bolt.available()){
      //to be printed to the lcd
      char blah = bolt.read();
      if(blah == '@'){
        Serial.println("Inside if after @");
        flag = true;
        while(bolt.available()){
          letter = bolt.read();
          Serial.println(letter);
          if(letter != '|')
            message += letter;
          else
            break;
        }
        break;
      }
    }
  }
  digitalWrite(ledPin, LOW);
  //waiting for the command
  start = millis();
  while(millis() < 20000+start  && message != "" ){
    if(bolt.available()){
      command = bolt.read();
      break;
    }
  }
  //if there is a message from the user
  if(flag){
    //print to lcd and buzzer
    //digitalWrite(alert2, HIGH);
    delay(40);
    digitalWrite(buzzer, HIGH);
    //digitalWrite(alert2, LOW);
    delay(500);
    digitalWrite(buzzer, LOW);
    Serial.println(message);
    if(message.length() > 16){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(message.substring(0, 16));
      lcd.setCursor(0, 1);
      lcd.print(message.substring(16, message.length()));
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(message);
    }
    //switch off the led
    //based on command open
    if(command == 'o'){
      openDoor();
      start = millis();
      Serial.println(start);
      while(start+20000 > millis()){
        //Serial.println("blalblabl");
        if(bolt.available()){
          command = bolt.read();
          if(command == 'R' || command == 'D' || command == ' ')
            continue;
          break;
        }
      }
    }
    else{
      digitalWrite(alert2, HIGH);
      delay(1000);
      digitalWrite(alert2, LOW);
    }
  }
  else{
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    Serial.println("Please come later ");
    lcd.setCursor(0, 0);
    lcd.print("Owner did not ");
    lcd.setCursor(0, 1);
    lcd.print("respond. Sorry!");
    delay(10000);
  }
  if(command == 'c' || !doorClosed)
    closeDoor();
  digitalWrite(alert, LOW);
  checkB = true;
  activateB = false;
}

void openDoor(){
  doorClosed = false;
  for(int i = 90; i>=0; i--){
    door.write(i);
    delay(30);
  }
  
}

void closeDoor(){
  doorClosed = true;
  for(int i = 0; i<=90; i++){
    door.write(i);
    delay(30);
  }
  delay(1000);
  digitalWrite(alert2, HIGH);
  delay(1000);
  digitalWrite(alert2, LOW);
}
