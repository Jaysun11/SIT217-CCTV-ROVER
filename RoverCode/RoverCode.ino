#include <CuteBuzzerSounds.h>

int buzzer = 5;

bool testingInterDeviceConnection = false;

//Comment out (used to test intruder alarm without second HC-10
int testIncrement = 0;

//Initialize Motor Code
// Motor One
int ENA = 6;
int IN1 = 7;
int IN2 = 8;
// Motor Two
int ENB = 11;
int IN3 = 9;
int IN4 = 10;

const int LED = 12;

byte alarmOn = false;
byte torchOn = false;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);

void setup() {
  // put your setup code here, to run once:

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(buzzer,OUTPUT);


  mySerial.begin(9600);
  Serial.begin(9600);

  digitalWrite(ENA, 255);
  digitalWrite(ENB, 255);

cute.init(buzzer);
cute.play(S_CONNECTION);

  //Use this to test for distinction task
 testingInterDeviceConnection = false;
}

void loop() {
  char transmission;

  if(mySerial.available()){
    transmission = mySerial.read();
    delay(100);
  
    // (used to test intruder alarm without second HC-10
    if (testingInterDeviceConnection){
      transmission = 'H';
    }
    
    Serial.println(transmission);
    
  } 

  if (alarmOn){
    alarm();
  } if (!alarmOn){
    alarmOff();
  }
  
  if (torchOn){
    torch();
  } else if (torchOff){
    torchOff();
  }

  switch (transmission) {
    
  case 'A':
    goStraight();
  
  break;
    
  case 'C':
    reverse();
    
  break;

  case 'D':
   turnLeft();
   
  break;
   
  case 'B':
   turnRight();
   
  break;
  
  //These stop when no button pushed any longer
  case 'a':  
  break;
  case 'b':  
  break;
  case 'c':  
  break;
  case 'd':  
  break;
  

  case 'G': //activate torch with X button
  
   if (!torchOn){
    Serial.println("Torch activated");
    torchOn = true;
  } else {
    Serial.println("Torch disabled");
    torchOn = false;
  }
   
  break;

  case 'F': // This case triggers an alarm circle button
  if (!alarmOn){
     Serial.println("On bard alarm activated");
    alarmOn = true;
  } else if (alarmOn){
     Serial.println("On bard alarm disabled");
    alarmOn = false;
    alarmOff();
  }
  break;
  
  //square button - intruder alert this case triggers a third party alarm
  case 'H':
  Serial.println("Intruder alert, send to third party alarm unit");
  sendMessageToAlarm();
  break;

  //Triangle button - Hello! (interact with user) 
  case 'E':
  playEmotion();
  break;
   
  default:
  
  stopMoving();
  
  break;
}

  // (used to test intruder alarm without second HC-10
  if (testingInterDeviceConnection){
    transmission = ' ';
    delay(2000);
  }

}

void sendMessageToAlarm(){
    String message = "Intruder";
    //(used to test intruder alarm without second HC-10 (testIncrement)
    if (testingInterDeviceConnection){
      message.concat(testIncrement);
      mySerial.print(message);
      delay(100);
      Serial.print("Message sent to Alarm: ");
      Serial.println(message);
      testIncrement++;
    } else {
       mySerial.print(message);
      Serial.print("Message sent to Alarm: ");
      Serial.println(message);
      delay(100);
    }
}

void playEmotion() {
  //currently just chooses a random emotion
  int randNumber = random(10);
  switch (randNumber){
    case 0:
    cute.play(S_CUDDLY);
    Serial.println("Feeling Cuddly");
    break;
    case 1:
    cute.play(S_OHOOH);
    Serial.println("Feeling WOW!");
    break;
    case 2:
    cute.play(S_SURPRISE);
    Serial.println("Feeling Surprise");
    break;
    case 3:
    cute.play(S_OHOOH2);
    Serial.println("Feeling Wow");
    break;
    case 4:
    cute.play(S_SLEEPING);
    Serial.println("Feeling Sleepy");
    break;
    case 5:
    cute.play(S_HAPPY);
    Serial.println("Feeling Happy");
    break;
    case 6:
    cute.play(S_SUPER_HAPPY);
    Serial.println("Feeling Super happy");
    break; 
    case 7:
    cute.play(S_CONFUSED);
    Serial.println("Feeling Confused");
    break;
    case 8:
    cute.play(S_SAD);
    Serial.println("Feeling Sad");
    break;
    case 9:
    cute.play(S_FART1);
    Serial.println("Feeling Smelly");
    break;
    default: 
    break;
  }

  
}

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  

}

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
}

void stopMoving() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void goStraight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}

void reverse() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

}

void alarm(){

  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(500);        // ...for 0.5 sec
  tone(buzzer, 750); // Send 0.5KHz sound signal...
  delay(500);        // ...for 0.5 sec

}
void alarmOff(){

  noTone(buzzer);
 
}

void torch(){
  digitalWrite(LED, HIGH);
}
void torchOff(){
  digitalWrite(LED, LOW);
}
