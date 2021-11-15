#include <Servo.h>

Servo Servo1; //Base
Servo Servo2; //Right
Servo Servo3; //Left
Servo Servo4; //Claw

const int Servo1Pin = 1; //Base
const int Servo2Pin = 2; //Right
const int Servo3Pin = 3; //Left
const int Servo4Pin = 4; //Claw

const int buttonPin = 7; //Pin of the Button
const int ledPin = 0; //Pin of the LED

int buttonState = 0;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 50;
int phase;

int initialS1 = 30;
int initialS2 = 30;
int initialS3 = 60;

int downS1 = 90;
int downS2 = 140;
int downS3 = 60;

int upS2 = 90;
int upS3 = 90;
  
int OpenClaw = 0;
int CloseClaw = 50;

void setup() {
  pinMode(Servo1Pin, OUTPUT);
  pinMode(Servo2Pin, OUTPUT);
  pinMode(Servo3Pin, OUTPUT);
  pinMode(Servo4Pin, OUTPUT);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Servo1.attach(Servo1Pin); 
  Servo2.attach(Servo2Pin); 
  Servo3.attach(Servo3Pin); 
  Servo4.attach(Servo4Pin); 

  startMillis = millis();
  int phase = 5;
}

void loop() {
  currentMillis = millis();
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    phase = 0;
  } 
  
  int S1Now = Servo1.read();
  int S2Now = Servo2.read();
  int S3Now = Servo3.read();
  //Does something every "period that passes" (Period = 0,1s)
  if(currentMillis - startMillis >= period){
    

    
    Serial.println("Period passed");
    
    if(phase == 0){
      Serial.println("Phase 0, initializing");
      //Initialize positions
      Servo1.write(initialS1);
      Servo2.write(initialS2);
      Servo3.write(initialS3);
      Servo4.write(CloseClaw);
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(750);
      phase += 1;
    }
    
    //Phase to move the grabber down
    if(phase == 1){
      
      Serial.println("Phase 1");
      Servo4.write(OpenClaw);
      
      if(S1Now < downS1){
        Servo1.write(S1Now+1);
      }
      if(S3Now < downS3){
        Servo2.write(S3Now+1);
      }
      if(S1Now == downS1 && S3Now == downS3){
        if(S2Now < downS2){
           Servo2.write(S2Now+2);
        }if(S2Now == downS2){
          Servo4.write(CloseClaw);
          digitalWrite(ledPin, HIGH);
          delay(250);
          digitalWrite(ledPin, LOW);
          delay(250);
          digitalWrite(ledPin, HIGH);
          delay(250);
          digitalWrite(ledPin, LOW);
          delay(750);
          phase += 1;
        }
      }
    }

    //Phase to move the grabber up
    if(phase == 2){
      Serial.println("Phase 2");
      if(S2Now > upS2){
        Servo2.write(S2Now-1);
      }
      if(S3Now < upS3){
        Servo3.write(S3Now+1);
      }
      if(S2Now <= upS2 && S3Now <= upS3){
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        delay(250);
        digitalWrite(ledPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        phase += 1;
      }
    }

    //Phase to move the grabber back to the initial position
    if(phase == 3){
      Serial.println("Phase 3");
      if(S1Now > initialS1){
        Servo1.write(S1Now-1);
      }
      if(S2Now > initialS2){
        Servo2.write(S2Now-1);
      }
      if(S3Now > initialS3){
        Servo3.write(S3Now-1);
      }
      if(S1Now <= initialS1 && S2Now <= initialS2 && S3Now <= initialS3){
      phase = 0;
      Servo4.write(CloseClaw);
      }
    } else {
      
    }
    startMillis = currentMillis;
  }
}
