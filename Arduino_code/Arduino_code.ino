#include <Servo.h>

/*
 * Matlab – syötetään polku sekvenssi – pystyykö arduino suorittamaan. 
 *(Aika ; servo nro ; kulma) tai (Aika ; Servo nro ; nopeus)
 * Nopeudella eliminoidaan ongelma – huojuvuus, vastaa mekaniikka kysymykseen ja emuloidaan toista moottoria.
 */

//Servos:
Servo Servo1; //Rotating bottom servo
Servo Servo2; //Shoulder servo
Servo Servo3; //Elbow servo
Servo Servo4; //Wrist servo

//Pins:
const int buttonPin = 9; //Pin of the Button
const int ledPin = 12; //Pin of the LED
const int Servo1Pin = 3; //Pin of the Servo 1
const int Servo2Pin = 0; //Pin of the Servo 2
const int Servo3Pin = 0; //Pin of the Servo 3
const int Servo4Pin = 0; //Pin of the Servo 4

//Variables:
String data; //Data from PC
char d1; //Selects device
String x;
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

void echo(String text){
  char* cString = (char*) malloc(sizeof(char)*(text.length() + 1));
  text.toCharArray(cString, text.length() + 1);
  //Serial.println("Serial Print cString: " + cString);
  Serial.println("Serial Print text: " + text);
}

void loop(){ 
  buttonState = digitalRead(buttonPin);

  if(Serial.available()){ // if there is data to read
    data = Serial.readString();
    Serial.println("before echo: " + data);
    echo(data);
    
    


    /* GUI case switch
     * 
    switch(d1){       //Select action based on 1st char
      case 'L':       // L = Turn on LED
      digitalWrite(ledPin,HIGH);
      break;
      case 'l':       // l = Turn off LED
      digitalWrite(ledPin, LOW);
      break;
      case 'S':       // S = Move Servo
      x = data.substring(1);
      servoval = x.toInt();
      Servo1.write(servoval);
      delay(100);   // Wait for servo to move
      break;
      
    }*/
  }
/*
  testServo(Servo1);
  testServo(Servo2);
  testServo(Servo3);
  testServo(Servo4);
  */
}

  //ohjelmistorakenne:
  //hiearkkia, servojen rinnakkaistoiminta (riippumatta liikuttamistavasta), asynkroninen toiminta,


// KAIKU RAJAPINTA - ottaa ja lähettää saman viestin -testataan interface


/* Inputed text: "move time ; servo to move ; desired angle"
 * Parses the text properly and calls the right function accordingly.
 
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
*/
  

void algorithm(float x1, float y1){
  //rajapinta servon nopeuden säätelyyn
  //laskee reittipisteet x0 -> haluttuun asentoon x1
  //return pistejono -> syötetään servolle nopeus pisteen pääsyyn
  //anturitiedolla pysäytys tms.
  //Ei mahdollista siirtää servoa alle max nopeuden -> loop; servoa liikutetaan vain yksi askel; delay; -> ajanmittaus -> ohjelmistokellon kautta(?)
  //main loopissa jokaista servon funktioo
  //emuloidaan eri moottorityyppien toimintaa (nopeus ja asentotieto)

  //Tarkistetaan ollaanko jo pisteessä
  Serial.println("algorithm funktio");
  
  float R1 = 1;
  float value1 = (pow(x1,2)+pow(y1,2));
  float value2 = pow(R1,2);

  String a1 = String(value1);
  String a2 = String(value2);
  Serial.println(a1);
  Serial.println(a2);   
  
  if(a1 == a2){ //(x0-x1)^2+(y0-y1)^2=r^2 ympyrän radan pisteet
    //Ollaan ympyrän radan sisäpuolella, voidaan osua pisteeseen
    Serial.println("IF passed");
    float angle = atan2(y1,x1);
    Serial.print(angle);
    if(Servo1.read() != angle){
        Servo1.write(x1);
    }
  }

}

//robotille oma rajapinta -> kysytään moottorien asentoa
//Palauttaa viimeisen kirjoitetun kulman (ei välttämättä lopullinen kulma)
int servo_position(Servo checkServo){
  int lastinput = checkServo.read(); //Ainoa tapa tarkistaa kulma ennen pid säädintä
  
  return lastinput;
}

  //säätöalgoritmille -> feedback (PID)
  //dv = a < liian kovaa
  //hidas lähtö tai rajoitettu nopeus tms.

  //EHKÄ vaihoehtoinen rajapinta paikkaan liittyen, mahdollisesti optio: softstart softstop (ease in ja ease out)
  //päälle älykkäämpi koneenohjausfunktio joka on siirrettävä


// alatason funktioita
  //rajapinta 1 servojen asennot (kulma) 
  //rajapinta 2 servojen kulmanopeudelle kun kulmassa -> nopeus 0

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

void move_servos(int position_servo1, int position_servo2, int position_servo3, int position_servo4){
  Servo1.write(position_servo1);
  Servo2.write(position_servo2);
  Servo3.write(position_servo3);
  Servo4.write(position_servo4);
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
