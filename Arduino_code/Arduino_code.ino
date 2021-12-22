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

//Demo values
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

//Pins:
const int buttonPin = 7; //Pin of the Button
const int ledPin = 0; //Pin of the LED
const int Servo1Pin = 1; //Pin of the Servo 1
const int Servo2Pin = 2; //Pin of the Servo 2
const int Servo3Pin = 3; //Pin of the Servo 3
const int Servo4Pin = 4; //Pin of the Servo 4

//Variables:
String data; //Data from PC
char d1; //Selects device
int buttonState = 0; //Variable for the pushbutton status
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 50; // Period = 0,05s

//Time variables
unsigned long time_now = 0;

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

  startMillis = millis();
}

// Online:
// Tarvitaan aikaleima (Time), Tieto moottorista (Servo), Kulma tietyllä ajanhetkellä (Angle/AngularVel)
// Tehdään lista tehtävistä -> saapuvat matlabilta kronologisessa järjestyksessä
// Tallennetaan sekvenssi (struktina) listaan -> käydään listaa läpi -> tarkistetaan seuraavan aikaleiman tiedot 

// For(listan alkio)
//  Robot_state[i]
//  foreach Robot_staten servo
//    Command
//  Sleep = Next i time check.

// ServoState -> tallennetaan kulmat structiin -> tallennetaan structi vectoriin; Käydään läpi vektoria elementeitteiteittäin
// tarkistetaan seuraavan ServoStaten aikaleima -> toimitaan oikein (odotetaan, käännytään) 

// Trejectory planning: Alkupiste - loppupiste alukis, katsotaana myöhemmin rajoitteet

// EI FIKSATTUA PERIODIA -> TEHDÄÄN AIKAPERUSTEINEN FUNKTIO
// Haetaan tietorakenteesta kronologisesti aina seuraava tehtävä
// Odotetaan jos ei tarvitse tehdä mitään

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

  if(currentMillis - startMillis >= period){ //Period has passed

    startMillis = currentMillis;
  }

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

}

/* Expect inputed text: "move time;s1 angle;s2 angle;---s5 angle"
 * Parses the text properly and calls the right function accordingly.
 */
void parse_text(String text){
  int S1;
  int S2;
  int S3;
  int S4;
  
  int index1;
  int index2;
  int index3;
  
  char c = text.read();
   
  if(c == '*'){
    Serial.println();
    Serial.print("caputred String is : ");
    Serial.println(readString);

    index1 = readString.indexOf(';');
    Time = readString.substring(0,index1);
    index2 = readString.indexOf(';', index1+1);
    S1 = readString.substring(index1+1, index2-1);
    index3 = readString.indexOf(';', index2+1);
    S2 = readString.substring(index2+1,index3-1);
    index4 = readString.indexOf(';', index3+1);
    S3 = readString.substring(index3+1,index4-1);
    index5 = readString.indexOf(';', index4+1);
    S4 = readString.substring(index4+1,index5-1);

    Serial.print("Time = ");
    Serial.println(Time); 
    Serial.print("Servo number = ");
    Serial.println(ServoNmbr);
    Serial.print("angle = ");
    Serial.println(angle);
    Serial.println();
    Serial.println();

    //Do something with the parsed inputs;
    
    
    readString=""; //clear variables for new inputs
    Time = "";
    ServoNmbr = "";
    Angle = "";
  } else {
    readString += c;
  }
}


void EaseMvmnt(int servo, int goal, int speedphase){ 
  
  int start = servo_position(servo);
  unsigned int difference = goal - start;
  float tread = difference/8;
  float endpart = (goal - tread);

  // Rajoitetaan nopeutta hidastamalla kulmanopeutta
  // -> Riippuu servosta mikä max nopeus (automaattinen)
}

void testServos(){
  
}

void initializeServo(Servo servo){
  servo.write(0);
}
