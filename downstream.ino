#include <SoftwareSerial.h>
//#define GRNLEDPIN 18
//#define REDLEDPIN 20
//#define YELLEDPIN 16
//#define BUZLEDPIN 14
int GRNLED = 18;
int REDLED = 20;
int YELLED = 16;

//Teensy 5V <--> HC 05 Vcc
//Teensy Ground <--> HC-05 GND
#define rxPin 7 //<--> HC-05Tx
#define txPin 8 //<--> HC-05Rx


void setup() {
  // put your setup code here, to run once:
  // Setup serial for monitor and Setup Serial1 for Bluetooth communication
  pinMode(GRNLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(YELLED, OUTPUT);
  
  Serial.begin(9600);
  Serial1.begin(9600);
  //Serial.println("Actuators");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available () > 0){
    String str = Serial1.readStringUntil('\n');
    Serial.println(str);
    if(str == "GRNLED_ON"){
      digitalWrite(GRNLED, HIGH);
      Serial.println("GRNLED ON");
    }
    else if(str == "GRNLED_OFF"){
      digitalWrite(GRNLED, LOW);
      Serial.println("GRNLED OFF");
    }
    else if(str == "REDLED_ON"){
      digitalWrite(REDLED, HIGH);
      Serial.println("REDLED ON");
    }
    else if(str == "REDLED_OFF"){
      digitalWrite(REDLED, LOW);
      Serial.println("REDLED OFF");
    }
    else if(str == "YELLED_ON"){
      digitalWrite(YELLED, HIGH);
      Serial.println("YELLED ON");
    }
    else if(str == "YELLED_OFF"){
      digitalWrite(YELLED, LOW);
      Serial.println("YELLED OFF");
    }
  }
}
