#include <Arduino.h>
#include <WiFiManager.h>
#include <WiFi.h>

#include "OTAUpdate.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeSans18pt7b.h>

Adafruit_BME280 bme280; // I2C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println("Booting");

  //does not work
  WiFi.setHostname("Home-Commander");

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset saved settings - for debug purposes
  //wifiManager.resetSettings();

  //fetches ssid and pass from eeprom and tries to connetc
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Home Commander");
  //OTA Update initialization
  OTAUpdate.begin();

  //SCL       ->     Pin D4 (GPIO2)                                 *
  //SDA       ->     Pin D3 (GPIO0)
  //Wire.begin(D3, D4);


  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  if (!bme280.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  // weather monitoring
  Serial.println("-- Weather Station Scenario --");
  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");

  bme280.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );
           
  // suggested rate is 1/60Hz (1m)
  //delayTime = 60000; // in milliseconds

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
                 
  display.setTextColor(WHITE);
  display.setFont(&FreeSans18pt7b);
  display.setCursor(0,30); 
  display.println("Hi!");
  delay(5000);
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme280.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme280.readPressure() / 100.0F);
    Serial.println(" hPa");

    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme280.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme280.readHumidity());
    Serial.println(" %");

    Serial.println();
}


void loop() {
  //handle OTA update
  OTAUpdate.handle();

  // Only needed in forced mode! In normal mode, you can remove the next line.
  bme280.takeForcedMeasurement(); // has no effect in normal mode
  
  String temperature = String(bme280.readHumidity(), 2);
  
  display.clearDisplay();
  display.setCursor(0,50);
  display.print(temperature); display.print(" %");
  display.display();

  printValues();
  delay(6000);
}