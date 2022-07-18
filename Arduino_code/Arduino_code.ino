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
Servo ServoBase; //Base
Servo ServoJoint1; //Left
Servo ServoJoint2; //Right
Servo ServoClaw; //Claw

//Initialization values
unsigned long Time_Init = 1;
long SBase_Init = 90;
long SJ1_Init = 60;
long SJ2_Init = 60;
long SClaw_Init = 0;
const long SJ2_SCALE = 180;

//Pins:
const int ServoBasePin = 1; //Pin of the Servo base
const int ServoJoint1Pin = 2; //Pin of the Servo 1st joint
const int ServoJoint2Pin = 3; //Pin of the Servo 2nd joint
const int ServoClawPin = 4; //Pin of the Servo claw

//Variables:
String readString; //captured String
String data; //Data from PC
char d1; //Selects device
int buttonState = 0; //Variable for the pushbutton status

int counter = 1; //Counter to track enumeration

//Time variables
unsigned long startMillis;
unsigned long Time;

bool CapturedAll = 0;

//Struct for event
typedef struct
{
  unsigned long Time;
  long SBaseAngle;
  long SJ1Angle;
  long SJ2Angle;
  long SClawAngle;
} Event;

Event EventArray[100]; // Array of 100 Events (Arduino does not have a dynamic data stroage -> Vector could not be used)

void setup() {
  Serial.begin(9600); //Baudrate of serialport (Matlab/GUI to Arduino)

  //Initialize the input/output
  pinMode(ServoBasePin, OUTPUT);
  pinMode(ServoJoint1Pin, OUTPUT);
  pinMode(ServoJoint2Pin, OUTPUT);
  pinMode(ServoClawPin, OUTPUT);

  ServoBase.attach(ServoBasePin);
  ServoJoint1.attach(ServoJoint1Pin);
  ServoJoint2.attach(ServoJoint2Pin);
  ServoClaw.attach(ServoClawPin);
}

void Initialize() {
  //Initial positions
  EventArray[0].Time = Time_Init;
  EventArray[0].SBaseAngle = SBase_Init;
  EventArray[0].SJ1Angle = SJ1_Init;
  EventArray[0].SJ2Angle = SJ2_Init;
  EventArray[0].SClawAngle = SClaw_Init;

  ServoBase.write(SBase_Init);
  ServoJoint1.write(SJ1_Init);
  ServoJoint2.write(SJ2_Init);
  ServoClaw.write(SClaw_Init);
}
void loop() {
  //Temporarly used variables
  String TimeStr;
  String SBaseStr;
  String SJ1Str;
  String SJ2Str;
  String SClawStr;

  long SBase;
  long SJ1;
  long SJ2;
  long SClaw;

  int index1;
  int index2;
  int index3;
  int index4;
  int index5;
  int Time_index1;
  int Time_index2;
  int Time_index3;

  if (Serial.available() > 0) { // Checks if there is data to read

    char c = Serial.read();

    /*  
        ----------------------------FUNCTION-----------------------------------
        Expect inputed text: "move time;SBase angle;---;SClaw angle*"
        Parses the text properly and calls the right function accordingly.
        Because of Arduinos limitations, own class could not be made.
        -----------------------------------------------------------------------
    */

    // CLEAR Event array
    if (c == 'c') {
      counter = 1; //Reset Counter
      for (int i = 1; i < 100; i++) {
        EventArray[i].Time = 0;
        EventArray[i].SBaseAngle = 0;
        EventArray[i].SJ1Angle = 0;
        EventArray[i].SJ2Angle = 0;
        EventArray[i].SClawAngle = 0;
      }
      Serial.print("Event Record cleared!");
      c = 0;
      
      // Signal arduino that every event has been sent and captured
    } if (c == 'k') {
      CapturedAll = 1;
      c = 0;
      Initialize();

      // Receive incoming events over serialport
    } if (c == '*') {
      Serial.println();
      Serial.print("caputred String is : ");
      Serial.println(readString);

      //Checks indexes of the delimiters ";", parses the inputs correctly
      index1 = readString.indexOf(';');
      TimeStr = readString.substring(0, index1);
      index2 = readString.indexOf(';', index1 + 1);
      SBaseStr = readString.substring(index1 + 1, index2);
      index3 = readString.indexOf(';', index2 + 1);
      SJ1Str = readString.substring(index2 + 1, index3);
      index4 = readString.indexOf(';', index3 + 1);
      SJ2Str = readString.substring(index3 + 1, index4);
      index5 = readString.indexOf(';', index4 + 1);
      SClawStr = readString.substring(index4 + 1, index5);

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
      SBase = SBaseStr.toInt();
      SJ1 = SJ1Str.toInt();
      SJ2 = SJ2Str.toInt() + SJ2_SCALE;
      SClaw = SClawStr.toInt();

      //Storing the captured values into the Event record
      EventArray[counter].Time = Time;
      EventArray[counter].SBaseAngle = (long)SBase;
      EventArray[counter].SJ1Angle = (long)SJ1;
      EventArray[counter].SJ2Angle = (long)SJ2;
      EventArray[counter].SClawAngle = (long)SClaw;

      //Clear variables for new inputs
      readString = "";
      TimeStr = "";
      SBaseStr = "";
      SJ1Str = "";
      SJ2Str = "";
      SClawStr = "";
      counter += 1;
    } else {
      //Adds the character to the string
      readString += c;
    }

    if (CapturedAll == 1) {
      startMillis = millis();
      
      //Checks each element of the record
      for (int i = 0; i < 100;) {
        unsigned long t = EventArray[i].Time;

        if (t > 0) {
          String EventMsg = "Initiating Event: " + String(i);
          Serial.println(EventMsg);

          long SBase = EventArray[i].SBaseAngle;
          long SJ1 = EventArray[i].SJ1Angle;
          long SJ2 = EventArray[i].SJ2Angle;
          long SClaw = EventArray[i].SClawAngle;

          // Since all the items are in chronological order -> check if time to act -> acts properly
          if (i < 99) {
            int next = i + 1;
            int t_next = EventArray[next].Time;
            if (t_next > 0) {
              unsigned long MOVING_TIME = t_next - t;
              unsigned long progress = millis() - startMillis;

              long SBaseangle_now = ServoBase.read();
              long SBaseangle_next = EventArray[next].SBaseAngle;

              long SJ1angle_now = ServoJoint1.read();
              long SJ1angle_next = EventArray[next].SJ1Angle;

              long SJ2angle_now = ServoJoint2.read();
              long SJ2angle_next = EventArray[next].SJ2Angle;

              if (progress <= MOVING_TIME) {
                long SBase_angle = map(progress, 0, MOVING_TIME, SBaseangle_now, SBaseangle_next);
                long SJ1_angle = map(progress, 0, MOVING_TIME, SJ1angle_now, SJ1angle_next);
                long SJ2_angle = map(progress, 0, MOVING_TIME, SJ2angle_now, SJ2angle_next);

                //Debugging prints
                Serial.print("SBase_angle ");
                Serial.println(SBase_angle);
                Serial.print("SJ1_angle ");
                Serial.println(SJ1_angle);
                Serial.print("SJ2_angle ");
                Serial.println(SJ2_angle);
                Serial.print("progress ");
                Serial.println(progress);
                Serial.print("MOVING_TIME ");
                Serial.println(MOVING_TIME);

                move_Servos(SBase_angle, SJ1_angle, SJ2_angle, SClaw);
              }
              if (millis() >= t_next) {
                Serial.println("Proceeding...");
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
  } else {
    Initialize();
  }
}

void move_Servos(long SBase, long SJ1, long SJ2, long SClaw){
  ServoBase.write(SBase);
  ServoJoint1.write(SJ1);
  ServoJoint2.write(SJ2);
  ServoClaw.write(SClaw);
}
