#include <Servo.h>
//Servos:
Servo Servo1; //Rotating bottom servo
Servo Servo2; //Shoulder servo
Servo Servo3; //Elbow servo
Servo Servo4; //Wrist servo

//Pins:
const int buttonPin = 9; //Pin of the Button
const int ledPin = 12; //Pin of the LED
const int ServoPin = 3; //Pin of the Servo



//Variables:
String data; //Data from PC
char d1; //Selects device
String x;
int buttonState = 0; //Variable for the pushbutton status

//Time variables
unsigned long time_now = 0;
int period = 1000; // interval of 1 second

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

  testServo(Servo1);
  testServo(Servo2);
  testServo(Servo3);
  testServo(Servo4);
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

void EaseMvmnt(int spd, Servo servo, int goal){ //How fast, what, where
  int period = 1000; // interval of 1 seconds
  
  int start = servo_position(servo);
  unsigned int difference = goal - start;
  float tread = difference/8;
  unsigned float endpart = (goal - tread);

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
  
  if(servo_position(servo) >= endpart {
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
  EaseMvmnt(2, servo, 45);
  Serial.print("done");

  Serial.print("Testing ease movement to 120 deg...");
  EaseMvmnt(2, servo, 120);
  Serial.print("done");
}

void initializeServo(Servo servo){
  servo.write(0);
}
