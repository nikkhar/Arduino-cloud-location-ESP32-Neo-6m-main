#include "thingProperties.h"
#include <WiFi.h>
#include <Arduino_ConnectionHandler.h>
#include <ArduinoIoTCloud.h>
#include <TinyGPS++.h>

float latitude;
float longitude;
TinyGPSPlus gpsModule;
HardwareSerial SerialGPS(1);

const char* ssid = "YOUR WIFI"; //enter your wifi name
const char* password = "YOUR WIFI PASSWORD"; //enter your wifi password

unsigned long previousMillis = 0;
const unsigned long updateInterval = 10000; 

void setup() {
  SerialGPS.begin(9600, SERIAL_8N1, TX, RX); //connect TX RX
  delay(1000);
  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    while (SerialGPS.available() > 0) {
      if (gpsModule.encode(SerialGPS.read())) {
        if (gpsModule.location.isValid()) {
          latitude = gpsModule.location.lat();
          longitude = gpsModule.location.lng();
          
          logGPSData(latitude, longitude);
          ArduinoCloud.update();
        }
      }
    }
    coordinates = Location(latitude, longitude); //changes value in cloud
    onCoordinatesChange();  
  }
}

//coordinates log
void logGPSData(float lat, float lng) {
  // Log the GPS data to Serial Monitor
  Serial.print("Timestamp: ");
  Serial.print(gpsModule.time.hour());
  Serial.print(":");
  Serial.print(gpsModule.time.minute());
  Serial.print(":");
  Serial.print(gpsModule.time.second());
  Serial.print(", Latitude: ");
  Serial.print(lat, 6);
  Serial.print(", Longitude: ");
  Serial.println(lng, 6);
}

void onCoordinatesChange() {
  // This is where you can define what happens when the coordinates change.
  // You can add your custom logic here.
  // For example, you can update some other variables or perform actions.
  // This function will be called automatically when the coordinates change.
}
