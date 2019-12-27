
/**************************************************************************************
 
 Internet weather station using ESP8266 Wi-Fi module
 This is a free software with NO WARRANTY.
 http://simple-circuit.com/
 
***************************************************************************************/
 
#include "config.h" 
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  // http web access library
#include <ArduinoJson.h>        // JSON decoding library

// include sensors
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// BME280 Sensor Definitions
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

// Reed Switch Ventilation Sensor Definition
#define windowPin 15

// Instanciate the sensors
Adafruit_BME280 bme;

// Delay between sensor reads, in seconds
#define READ_DELAY 10

 // WiFI Parameters
const char *ssid     = "Telecom-61690181";
const char *password = "mWwtu6NFNoykKA9293LbSUYK";

// Data
int main_temp;

// DHT22 Data
int temperatureReading;
int pressureReading;

// BME280 Data
int altitudeReading = 0;
int humidityReading = 0;

// Reed Switch Data
int windowState = LOW;

// Set up the Feeds for external API
AdafruitIO_Feed *outsidetemperaturefeed = io.feed("outside-temperature");
AdafruitIO_Feed *outsidehumidityfeed = io.feed("outside-humidity");
AdafruitIO_Feed *outsidepressurefeed = io.feed("outside-pressure");

// set up the feeds for the BME280
AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed = io.feed("humidity");
AdafruitIO_Feed *pressureFeed = io.feed("pressure");

// set up feed for the Reed Switch
AdafruitIO_Feed *windowFeed = io.feed("window");

void setup() {
  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  while (!Serial);

  Serial.println("Adafruit IO Environmental Logger");

    // set up BME280
  setupBME280();

    // declare Reed Switch as input
  pinMode(windowPin, INPUT);
  
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  Serial.println("Reading Sensors...");
  Serial.println("* read window sensor...");
  readWindowSensor();

  // Read the temperature from the BME280
  temperatureReading = bme.readTemperature();

  Serial.print("Temperature = "); Serial.print(temperatureReading); Serial.println(" *F");

  // Read the pressure from the BME280
  pressureReading = bme.readPressure() / 100.0F;
  Serial.print("Pressure = "); Serial.print(pressureReading); Serial.println(" hPa");
  
  // Read the humidity from the BME280
  humidityReading = bme.readHumidity();
  Serial.print("Humidity = "); Serial.print(humidityReading); Serial.println("%");


  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://api.openweathermap.org/data/2.5/weather?lat=41.92&lon=12.52&APPID=7a3d3be289dc85b94e1430f35310fa20"); // Rome Weather
    // http.begin("http://api.openweathermap.org/data/2.5/weather?q=London,uk&APPID=7a3d3be289dc85b94e1430f35310fa20"); London Weather
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
    String payload = http.getString();   //Get the request response payload
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(13) + 250;
    DynamicJsonDocument doc(capacity);

    //const char* json = "{\"coord\":{\"lon\":-0.13,\"lat\":51.51},\"weather\":[{\"id\":741,\"main\":\"Fog\",\"description\":\"fog\",\"icon\":\"50d\"}],\"base\":\"stations\",\"main\":{\"temp\":278.24,\"pressure\":1020,\"humidity\":86,\"temp_min\":275.93,\"temp_max\":280.37},\"visibility\":10000,\"wind\":{\"speed\":2.1,\"deg\":70},\"clouds\":{\"all\":20},\"dt\":1575128095,\"sys\":{\"type\":1,\"id\":1412,\"country\":\"GB\",\"sunrise\":1575099726,\"sunset\":1575129384},\"timezone\":0,\"id\":2643743,\"name\":\"London\",\"cod\":200}";

    deserializeJson(doc, payload);

    const char* base = doc["base"]; // "stations"

    JsonObject main = doc["main"];
    float main_temp = main["temp"]; // 278.24
    int main_pressure = main["pressure"]; // 1020
    int main_humidity = main["humidity"]; // 86

    // send data to Adafruit IO feeds
    temperatureFeed->save(temperatureReading);
    humidityFeed->save(humidityReading);
    // altitudeFeed->save(altitudeReading);
    pressureFeed->save(pressureReading);
    Serial.print("Outside Temperature:");
    Serial.println(main_temp - 273.15);
    outsidetemperaturefeed->save(main_temp - 273.15);
    Serial.print("Outside Humidity:");
    Serial.println(main_humidity);
    outsidehumidityfeed->save(main_humidity);
    Serial.print("Outside Pressure:"); 
    Serial.println(main_pressure);
    outsidepressurefeed->save(main_pressure);
    // delay the polled loop
    delay(READ_DELAY * 1000);
    }
    http.end();   //Close connection
  }
  // Delay
  delay(15000);
}

// Set up the Reed Switch Ventilation sensor
void readWindowSensor() {
// reads the status of the front door and sends to adafruit io
  windowState = digitalRead(windowPin);
  if (windowState == LOW) {
    Serial.println("* Window Closed");
    windowFeed->save(1);
  } else {
    Serial.println("* Window Open");
    windowFeed->save(3);
  }
  delay(READ_DELAY);
}

// Set up the BME280 sensor
void setupBME280() {
  bool status;
  status = bme.begin();
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("BME Sensor is set up!");
}
