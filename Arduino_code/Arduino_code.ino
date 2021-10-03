#include <Servo.h>

Servo Servo1;
const int buttonPin = 9; //Pin of the Button
const int ledPin = 12; //Pin of the LED
const int ServoPin = 3; //Pin of the Servo

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

void algorithm(){
  if (buttonState == HIGH){
      digitalWrite(ledPin, HIGH);
      int pos = 0;
      while(pos < 270){
        Servo1.write(pos); 
        pos += 1; 
        }
     } else {
      // Make servo go to original position
      Servo1.write(0);
      delay(15); 
      digitalWrite(ledPin, LOW);
     }
  //rajapinta servon nopeuden säätelyyn
  //laskee reittipisteet x0 -> haluttuun asentoon x1
  //return pistejono -> syötetään servolle nopeus pisteen pääsyyn
  //anturitiedolla pysäytys tms.
  //Ei mahdollista siirtää servoa alle max nopeuden -> loop; servoa liikutetaan vain yksi askel; delay; -> ajanmittaus -> ohjelmistokellon kautta(?)
  //main loopissa jokaista servon funktioo
  //emuloidaan eri moottorityyppien toimintaa (nopeus ja asentotieto)

}

void servo_position(){
  //robotille oma rajapinta -> kysytään moottorien asentoa
  //säätöalgoritmille -> feedback (PID)
}
  //dv = a < liian kovaa
  //hidas lähtö tai rajoitettu nopeus tms.

  //EHKÄ vaihoehtoinen rajapinta paikkaan liittyen, mahdollisesti optio: softstart softstop (ease in ja ease out)
  //päälle älykkäämpi koneenohjausfunktio joka on siirrettävä
