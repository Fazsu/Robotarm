#include <Servo.h>

/*
 * Matlab – syötetään polku sekvenssi – pystyykö arduino suorittamaan. 
 *(Aika ; servo nro ; kulma) tai (Aika ; Servo nro ; nopeus)
 * Nopeudella eliminoidaan ongelma – huojuvuus, vastaa mekaniikka kysymykseen ja emuloidaan toista moottoria.
 */

//Servos:
Servo Servo1; //Base
Servo Servo2; //Left
Servo Servo3; //Right
Servo Servo4; //Claw

//Pins:
const int buttonPin = 9; //Pin of the Button
const int ledPin = 12; //Pin of the LED
const int Servo1Pin = 1; //Pin of the Servo 1
const int Servo2Pin = 2; //Pin of the Servo 2
const int Servo3Pin = 3; //Pin of the Servo 3
const int Servo4Pin = 4; //Pin of the Servo 4

//Variables:
String data; //Data from PC
char d1; //Selects device
int buttonState = 0; //Variable for the pushbutton status
int ServoNmbr;
time_t Time;

//Time variables
unsigned long time_now = 0;
int period = 1000; // interval of 1 second

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

//ohjelmistorakenne:
//hiearkkia, servojen rinnakkaistoiminta (riippumatta liikuttamistavasta), asynkroninen toiminta,

//säätöalgoritmille -> feedback (PID)
//dv = a < liian kovaa
//hidas lähtö tai rajoitettu nopeus tms.

//EHKÄ vaihoehtoinen rajapinta paikkaan liittyen, mahdollisesti optio: softstart softstop (ease in ja ease out)
//päälle älykkäämpi koneenohjausfunktio joka on siirrettävä


// alatason funktioita
  //rajapinta 1 servojen asennot (kulma) 
  //rajapinta 2 servojen kulmanopeudelle kun kulmassa -> nopeus 0


// KAIKU RAJAPINTA - ottaa ja lähettää saman viestin takaisin
void echo(String text){
  char* cString = (char*) malloc(sizeof(char)*(text.length() + 1));
  text.toCharArray(cString, text.length() + 1);
  Serial.println("Serial Print text: " + text);
}

//Robotille oma rajapinta -> kysytään moottorien asentoa
//Palauttaa viimeisen kirjoitetun kulman (ei välttämättä lopullinen kulma)
int servo_position(Servo checkServo){
  int lastinput = checkServo.read(); //Ainoa tapa tarkistaa kulma ennen pid säädintä
  Serial.println(lastinput);
}

void loop(){ 
  buttonState = digitalRead(buttonPin);

  if(Serial.available()){ // If there is data to read
    data = Serial.readString(); 

    //Komennon muoto? Viestipituus ongelma. Fixed length. Ei hyvä. 255 merkkiongelma.
    //Miten ilmoitetaan aika?
    //Käytetään Arduinon kelloa.
    //Parsitaan funktio delimiterin avulla.
    
    echo(data);
    d1 = data[0];

    if(data.length() > 1){
      char s = data[1];
      int servo = s.toInt();
      
      if(data.length() > 2){
        String x = data.substring(2);
        int servoval = x.toInt();
      }
    }
        
    switch(d1){       //Select action based on 1st char
      case 'L':       //Sequence list action
        
        break;
        
      case 'C':       //Check servo "C1" -> Check Servo1
        if(servo == 1){
          servo_position(Servo1);
        } if(servo == 2) {
          servo_position(Servo2);
        } if(servo == 3){
          servo_position(Servo3);
        } if(servo == 4){
          servo_position(Servo4);
        } else {
          Serial.println("Error: Check input.");
        }
        break;
        
      case 'S':   //Move individual servo "S1120" -> servo 1 to 120deg
        if(servo == 1){
          Servo1.write(servoval);
          delay(100); // Wait for servo to move
        } if(servo == 2) {
          Servo2.write(servoval);
          delay(100);
        } if(servo == 3){
          Servo3.write(servoval);
          delay(100);
        } if(servo == 4){
          Servo4.write(servoval);
          delay(100); 
        } else {
          Serial.println("Error: Check input.");
        }
        break;
    }
  }
/*
  testServo(Servo1);
  testServo(Servo2);
  testServo(Servo3);
  testServo(Servo4);
  */
}

/* Inputed text: "move time ; servo to move ; desired angle"
 * Parses the text properly and calls the right function accordingly.
 */
void parse_text(String text){
  int index1; // ;-locations
  int index2;
  int index3;
  String delim = '*';
  char c = text.read();
   
  if(c == delim){
    Serial.println();
    Serial.print("caputred String is : ");
    Serial.println(readString);

    index1 = readString.indexOf(';');
    Time = readString.substring(0,ind1);
    index2 = readString.indexOf(';', ind1+1);
    ServoNmbr = readString.substring(ind1+1, ind2+1);
    index3 = readString.indexOf(';', ind2+1);
    angle = readString.substring(ind2+1);

    Serial.print("Time = ");
    Serial.println(Time); 
    Serial.print("Servo number = ");
    Serial.println(ServoNmbr);
    Serial.print("angle = ");
    Serial.println(angle);
    Serial.println();
    Serial.println();

    //Do something with the parsed inputs;
    
    readString=""; //clears variable for new input
    Time = "";
    ServoNmbr = "";
    angle = "";
  } else {
    readString += c;
  }
}


void EaseMvmnt(Servo servo, int goal, int spd){ //How fast, what, where
  int period = 1000; // interval of 1 seconds
  
  int start = servo_position(servo);
  unsigned int difference = goal - start;
  float tread = difference/8;
  float endpart = (goal - tread);

  if(servo_position(servo) <= tread){
      for(int i=start; i<=tread; i+=spd){
        if(millis() >= time_now + period){
          time_now += period;
          servo.write(i);
        }
      }
    }
    
  if(servo_position(servo) > tread && servo_position(servo) < endpart){
    servo.write(endpart);
  }
  
  if(servo_position(servo) >= endpart) {
        for(int i=endpart; i<=goal; i+=spd){
          if(millis() >= time_now + period){
          time_now += period;
          servo.write(i);
        }
    }
  }
}

void testServo(Servo servo){
  Serial.print("Initializing servo:...");
  initializeServo(servo);
  Serial.print("done");
  
  Serial.print("Testing servo movement...");
  servo.write(15);
  Serial.print(servo_position(servo));
  Serial.print("done");

  Serial.print("Testing ease movement to 45 deg...");
  EaseMvmnt(servo, 2, 45);
  Serial.print("done");

  Serial.print("Testing ease movement to 120 deg...");
  EaseMvmnt(servo, 2, 120);
  Serial.print("done");
}

void initializeServo(Servo servo){
  servo.write(0);
}
