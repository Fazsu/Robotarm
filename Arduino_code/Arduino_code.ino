#include <Servo.h>

<<<<<<< Updated upstream
Servo Servo1;
const int buttonPin = 9; //Pin of the Button
const int ledPin = 12; //Pin of the LED
const int ServoPin = 3; //Pin of the Servo
=======
/*
 ******************AUTHOR***********************
 *     CREATED BY: MISKA ROMPPAINEN            *
 *     EMAIL: miska.romppainen@hotmail.com     *
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
>>>>>>> Stashed changes

String data; //Data from PC
char d1; //Selects device
String x;

int buttonState = 0; //Variable for the pushbutton status

int servoval = 1;

void setup() { 
  Serial.begin(9600);
  
  //Initialize the input/output
  pinMode(ServoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Servo1.attach(ServoPin); 

}

void loop(){ 
  buttonState = digitalRead(buttonPin);
  
  
  if(Serial.available()){
    data = Serial.readString();
    d1 = data.charAt(0);

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
    }
  }
}

  //ohjelmistorakenne:
  //hiearkkia, servojen rinnakkaistoiminta (riippumatta liikuttamistavasta), asynkroninen toiminta,

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

void servo_position(){
  //robotille oma rajapinta -> kysytään moottorien asentoa
  //säätöalgoritmille -> feedback (PID)
}
  //dv = a < liian kovaa
  //hidas lähtö tai rajoitettu nopeus tms.

  //EHKÄ vaihoehtoinen rajapinta paikkaan liittyen, mahdollisesti optio: softstart softstop (ease in ja ease out)
  //päälle älykkäämpi koneenohjausfunktio joka on siirrettävä
