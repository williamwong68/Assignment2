#include "DHT.h"
//#include <SoftwareSerial.h>
#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "Adafruit_PM25AQI.h"

#define DHTPIN 21      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define LEDPIN 11

// If your PM2.5 is UART only, for UNO and others (without hardware serial)
// we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(2, 3);

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SGP30 sgp;
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

// Teensy 5V <--> HC-05 Vcc
// Teensy Ground <--> HC-05 GND
#define rxPin 7 // Teensy pin 7 <--> HC-05 Tx
#define txPin 8 // Teensy pin 8 <--> HC-05 Rx
//SoftwareSerial BTSerial =  SoftwareSerial(rxPin, txPin);

void setup() {
  // Setup serial for monitor
  //pinMode(ledPin, OUTPUT);
  //Serial.begin(38400);
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  // Setup DHT Sensor
  pinMode(DHTPIN, INPUT);
  dht.begin();

  // Setup Serial1 for BlueTooth
  //pinMode(ledPin, OUTPUT);
  Serial1.begin(9600); // Default communication rate of the Bluetooth module

  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // Wait for serial console to open!
  }

  Serial.println("SGP30 test");

  if (! sgp.begin()) {
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!


  // Wait for serial monitor to open
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  
  // Wait one second for sensor to boot up!
  delay(1000);
  
  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
  //  Serial1.begin(9600);
  pmSerial.begin(9600);
  
  // There are 3 options for connectivity!
  //if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
  //if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
  
  Serial.println("PM25 found!");

}
int counter = 0;
void loop() {
  if (Serial1.available() > 0) { // Checks whether data is comming from the serial port
    delay(2000);
    

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F(" Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("C "));
    Serial.print(f);
    Serial.print(F("F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("C "));
    Serial.print(hif);
    Serial.println(F("F"));

    Serial1.print(F(" Humidity: "));
    Serial1.print(h);
    Serial1.print(F("%  Temperature: "));
    Serial1.print(t);
    Serial1.print(F(" C "));
    Serial1.print(f);
    Serial1.print(F("F  Heat index: "));
    Serial1.print(hic);
    Serial1.print(F("C "));
    Serial1.print(hif);
    Serial1.print(F("F "));

    if (! sgp.IAQmeasure() && sgp.IAQmeasureRaw()) {
      Serial.println("Measurement failed");
      return;
    }
    Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print("  ppb\t");
    Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.print("  ppm");
    Serial1.print("TVOC: "); Serial1.print(sgp.TVOC); Serial1.print("      ppb \t");
    Serial1.print("eCO2: "); Serial1.print(sgp.eCO2); Serial1.print("  ppm ");

    if (! sgp.IAQmeasureRaw()) {
      Serial.println("Raw Measurement failed");
      return;
    }
    Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
    Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");
    Serial1.print("Raw H2: "); Serial1.print(sgp.rawH2); Serial1.print(" \t");
    Serial1.print("Raw Ethanol: "); Serial1.print(sgp.rawEthanol); Serial1.print("");

    //delay(1000);

    counter++;
    if (counter == 30) {
      counter = 0;

      uint16_t TVOC_base, eCO2_base;
      if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
        Serial.println("Failed to get baseline readings");
        return;
      }
      Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
      Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    }

    PM25_AQI_Data data;

    if (! aqi.read(&data)) {
      Serial.println("Could not read from AQI");
      Serial1.println("Could not read from AQI");
      //delay(500);  // try again in a bit!
      return;
    }
    Serial.println("AQI reading success");

    Serial.println();
    Serial.println(F("---------------------------------------"));
    Serial.println(F("Concentration Units (standard)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
    Serial.println(F("Concentration Units (environmental)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
    Serial.println(F("---------------------------------------"));
    Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
    Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
    Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
    Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
    Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
    Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
    Serial.println(F("---------------------------------------"));

    Serial1.print(F("PM 1.0: ")); Serial1.print(data.pm10_standard);
    Serial1.print(F("\t\tPM 2.5: ")); Serial1.print(data.pm25_standard);
    Serial1.print(F("\t\tPM 10: ")); Serial1.println(data.pm100_standard);
    //Serial1.print(F("---------------------------------------"));
    //Serial1.print(F("Particles > 0.3um / 0.1L air:")); Serial1.print(data.particles_03um);
    //Serial1.print(F("Particles > 0.5um / 0.1L air:")); Serial1.print(data.particles_05um);
    //Serial1.print(F("Particles > 1.0um / 0.1L air:")); Serial1.print(data.particles_10um);
    //Serial1.print(F("Particles > 2.5um / 0.1L air:")); Serial1.print(data.particles_25um);
    //Serial1.print(F("Particles > 5.0um / 0.1L air:")); Serial1.print(data.particles_50um);
    //Serial1.print(F("Particles > 10 um / 0.1L air:")); Serial1.print(data.particles_100um);
    //Serial1.println(F("---------------------------------------"));

    delay(1000);
  }
  
}
