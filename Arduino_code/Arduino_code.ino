#include <Servo.h>

/*
 ******************AUTHOR***********************
 *     CREATED BY: MISKA ROMPPAINEN            *
 *     EMAIL: miska.romppainen@tuni.fi         *    
 *     Tampere University                      *
 *     Student ID: 274426                      *
 ***********************************************   
 *****************PROJECT***********************
 *             MeARM Project                   *
 *    Code for Arduino:                        *
 *    - Recevies message containing sequence   *
 *    list from 3rd Party controller, such     *
 *    as Matlab                                *
 *    - parses the message and creates the     *                
 *    sequence list                            *     
 *    - Controlls the Servo motors             *
 ***********************************************   
 */
//Servos:
Servo Servo1; //Base
Servo Servo2; //Left
Servo Servo3; //Right
Servo Servo4; //Claw

//Initialization values
unsigned long Time_Init = 1;
int S1_Init = 90;
int S2_Init = 60;
int S3_Init = -120;
int S4_Init = 0;

//Pins:
const int buttonPin = 7; //Pin of the Button
const int ledPin = 0; //Pin of the LED
const int Servo1Pin = 1; //Pin of the Servo 1
const int Servo2Pin = 2; //Pin of the Servo 2
const int Servo3Pin = 3; //Pin of the Servo 3
const int Servo4Pin = 4; //Pin of the Servo 4

//Variables:
String readString; //captured String
String data; //Data from PC
char d1; //Selects device
int buttonState = 0; //Variable for the pushbutton status

int counter = 1;

//Time variables
unsigned long startMillis;
unsigned long Time;

bool CapturedAll = 0;

//Structi jokaiselle "eventille"
typedef struct
{
  unsigned long Time;
  int S1Angle;
  int S2Angle;
  int S3Angle;
  int S4Angle;
} Action;

Action actionRecord[100]; // Array of 100 actions (Arduino does not have a dynamic data stroage -> Vector could not be used)

void setup() {
  Serial.begin(9600); //Connects the serialport (Matlab/GUI to Arduino)

  //Initialize the input/output
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
}

void Initialize() {
  //Initial positions
  actionRecord[0].Time = Time_Init;
  actionRecord[0].S1Angle = S1_Init;
  actionRecord[0].S2Angle = S2_Init;
  actionRecord[0].S3Angle = S3_Init;
  actionRecord[0].S4Angle = S4_Init;

  Servo1.write(S1_Init);
  Servo2.write(S2_Init);
  Servo3.write(S3_Init);
  Servo4.write(S4_Init);
}
void loop() {
  //Temporarly used variables
  String TimeStr;
  String S1Str;
  String S2Str;
  String S3Str;
  String S4Str;

  int S1;
  int S2;
  int S3;
  int S4;

  int index1;
  int index2;
  int index3;
  int index4;
  int index5;
  int Time_index1;
  int Time_index2;
  int Time_index3;

  buttonState = digitalRead(buttonPin);

  if (Serial.available() > 0) { // Checks if there is data to read

    char c = Serial.read();

    /*  ----------------------------FUNCTION-----------------------------------
        Expect inputed text: "move time;s1 angle;---;s4 angle*"
        Parses the text properly and calls the right function accordingly.
        Because of Arduinos limitations, own class could not be made.
        -----------------------------------------------------------------------
    */

    // CLEAR ACTION RECORD
    if (c == 'c') {
      counter = 1; //Reset Counter
      for (int i = 1; i < 100; i++) {
        actionRecord[i].Time = 0;
        actionRecord[i].S1Angle = 0;
        actionRecord[i].S2Angle = 0;
        actionRecord[i].S3Angle = 0;
        actionRecord[i].S4Angle = 0;
      }
      Serial.print("Action Record cleared!");

      // Signal arduino that every event has been sent and captured
    } if (c == 'k') {
      CapturedAll = 1;
      Initialize();

      // Receive incoming events over serial port
    } if (c == '*') {
      Serial.println();
      Serial.print("caputred String is : ");
      Serial.println(readString);

      //Checks indexes of the delimiters ";", parses the inputs correctly
      index1 = readString.indexOf(';');
      TimeStr = readString.substring(0, index1);
      index2 = readString.indexOf(';', index1 + 1);
      S1Str = readString.substring(index1 + 1, index2);
      index3 = readString.indexOf(';', index2 + 1);
      S2Str = readString.substring(index2 + 1, index3);
      index4 = readString.indexOf(';', index3 + 1);
      S3Str = readString.substring(index3 + 1, index4);
      index5 = readString.indexOf(';', index4 + 1);
      S4Str = readString.substring(index4 + 1, index5);

      //Converting HH:MM:ss time format to integer
      Time_index1 = TimeStr.indexOf(':');
      String Hours = TimeStr.substring(0, Time_index1);
      Time_index2 = readString.indexOf(':', Time_index1 + 1);
      String Minutes = readString.substring(Time_index1 + 1, Time_index2);
      Time_index3 = readString.indexOf(';', Time_index2 + 1);
      String Seconds = readString.substring(Time_index2 + 1, Time_index3);

      unsigned long long p_Time = Hours.toInt() * 3600000 + Minutes.toInt() * 60000 + Seconds.toInt() * 1000;

      Time = p_Time + millis();

      //Converting the captured strings to integer
      S1 = S1Str.toInt();
      S2 = S2Str.toInt();
      S3 = S3Str.toInt();
      S4 = S4Str.toInt();

      //Storing the captured values into the action record
      actionRecord[counter].Time = Time;
      actionRecord[counter].S1Angle = S1;
      actionRecord[counter].S2Angle = S2;
      actionRecord[counter].S3Angle = S3;
      actionRecord[counter].S4Angle = S4;

      //Clear variables for new inputs
      readString = "";
      TimeStr = "";
      S1Str = "";
      S2Str = "";
      S3Str = "";
      S4Str = "";
      counter += 1;
    } else {
      //Adds the character to the string
      readString += c;
    }

    if (CapturedAll == 1) {
      /*
        for (int i = 0; i < 100;i++) {
        unsigned long t = actionRecord[i].Time;
        int s1 = actionRecord[i].S1Angle;
        int s2 = actionRecord[i].S2Angle;
        int s3 = actionRecord[i].S3Angle;
        int s4 = actionRecord[i].S4Angle;

        Serial.println("Event: "+String(i));
        Serial.println("Time: "+String(t));
        Serial.println("s1: "+String(s1));
        Serial.println("s2: "+String(s2));
        Serial.println("s3: "+String(s3));
        }
      */
      startMillis = millis();
      //Checks each element of the record
      for (int i = 0; i < 100;) {
        unsigned long t = actionRecord[i].Time;

        if (t > 0) {
          String actionMsg = "Initiating action: " + String(i);
          Serial.println(actionMsg);

          int s1 = actionRecord[i].S1Angle;
          int s2 = actionRecord[i].S2Angle;
          int s3 = actionRecord[i].S3Angle;
          int s4 = actionRecord[i].S4Angle;

          // Since all the items are in chronological order -> check if time to act -> acts properly
          if (i < 99) {
            int next = i + 1;
            int t_next = actionRecord[next].Time;
            if (t_next > 0) {
              unsigned long MOVING_TIME = t_next - t;
              unsigned long progress = millis() - startMillis;

              int S1angle_now = Servo1.read();
              int S1angle_next = actionRecord[next].S1Angle;

              int S2angle_now = Servo2.read();
              int S2angle_next = actionRecord[next].S2Angle;

              int S3angle_now = Servo3.read();
              int S3angle_next = actionRecord[next].S3Angle;

              if (progress <= MOVING_TIME) {
                long S1_angle = map(progress, 0, MOVING_TIME, S1angle_now, S1angle_next);
                long S2_angle = map(progress, 0, MOVING_TIME, S2angle_now, S2angle_next);
                long S3_angle = map(progress, 0, MOVING_TIME, S3angle_now, S3angle_next);

                Serial.print("S1_angle ");
                Serial.println(S1_angle);
                Serial.print("S2_angle ");
                Serial.println(S2_angle);
                Serial.print("S3_angle ");
                Serial.println(S3_angle);
                Serial.print("progress ");
                Serial.println(progress);
                Serial.print("MOVING_TIME ");
                Serial.println(MOVING_TIME);

                Servo1.write(S1_angle);
                Servo2.write(S2_angle);
                Servo3.write(S3_angle);
              }
              if (millis() >= t_next) {
                Servo1.write(S1angle_next);
                Servo2.write(S2angle_next);
                Servo3.write(S3angle_next);
                Serial.print("Times up, proceeding...");
                startMillis = millis();
                i++;
              }
            } else {
              Serial.println("No more events in sequence");
              i++;
            }
          }
        } if (t == 0) {
          break;
        }
      }
      Serial.println("Done!");
      CapturedAll = 0;
    }
  }
}
