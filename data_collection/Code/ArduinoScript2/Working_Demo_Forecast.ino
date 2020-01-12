
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
AdafruitIO_Feed *outsidetemperatureforecast = io.feed("outside-temperature-forecast");
AdafruitIO_Feed *outsidehumidityforecast = io.feed("outside-humidity-forecast");
AdafruitIO_Feed *outsidepressureforecast = io.feed("outside-pressure-forecast");
AdafruitIO_Feed *uvIndexforecast = io.feed("uv-index-forecast");
AdafruitIO_Feed *precipIntensityforecast = io.feed("precipitation-intensity-forecast");
AdafruitIO_Feed *indoorhumidityprediction = io.feed("indoor-humidity-prediction");

// set up the feeds for the BME280
AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed = io.feed("humidity");
AdafruitIO_Feed *pressureFeed = io.feed("pressure");


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
    HTTPClient https;  //Object of class HTTPClient
    https.begin("https://io.adafruit.com/api/v2/gabrieledamone/integrations/weather/2302?x-aio-key=5b2580302b1d408d84be87e9756b5c2c"); // Rome Weather
    int httpCode = https.GET();
    Serial.println("phase 8 done");
    //Check the returning code                                                                  
    if (httpCode > 0) {
    String payload = https.getString();   //Get the request response payload
    Serial.println("phase 9 done");
    const size_t capacity = 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(11) + 5*JSON_OBJECT_SIZE(17) + JSON_OBJECT_SIZE(18) + JSON_OBJECT_SIZE(19) + JSON_OBJECT_SIZE(38) + 2*JSON_OBJECT_SIZE(39);
    Serial.println("phase 10 done");
    DynamicJsonDocument doc(capacity);

    Serial.println("phase 1 done");

    deserializeJson(doc, payload);

    Serial.println("phase 2 done");

    const char* username = doc["username"]; // "gabrieledamone"

    Serial.println("phase 3 done");

    JsonObject forecast_hours_1 = doc["forecast_hours_1"];
    int forecast_hours_1_precipIntensity = forecast_hours_1["precipIntensity"];
    float forecast_hours_1_temperature = forecast_hours_1["temperature"]; 
    float forecast_hours_1_humidity = forecast_hours_1["humidity"]; 
    int forecast_hours_1_pressure = forecast_hours_1["pressure"]; 
    int forecast_hours_1_uvIndex = forecast_hours_1["uvIndex"]; 

    Serial.println("phase 4 done");


    // Send data from Sensors to Adafruit IO
    temperatureFeed->save(temperatureReading);
    humidityFeed->save(humidityReading);
    pressureFeed->save(pressureReading);
    // Send data from API to Adafruit IO
    //outsidetemperatureforecast->save(forecast_hours_1_temperature);
    //outsidehumidityforecast->save(forecast_hours_1_humidity);
    //outsidepressureforecast->save(forecast_hours_1_pressure);
    //uvIndexforecast->save(forecast_hours_1_uvIndex);
    //precipIntensityforecast->save(forecast_hours_1_precipIntensity);
    indoorhumidityprediction->save(42.1125 + 0.024074*forecast_hours_1_precipIntensity -0.10937*forecast_hours_1_uvIndex + 0.90715*forecast_hours_1_temperature + 0.067339*forecast_hours_1_humidity);

    if (forecast_hours_1_temperature > 25) {
      if (indoorhumidityprediction > 50) {
        Dehumidifier = ON
      }
      else {
        Dehumidifier = OFF
    }
    if (forecast_hours_1_temperature < 25) {
      if (indoorhumidityprediction > 40) {
        Dehumidifier = ON
      }
      else {
        Dehumidifier = OFF
      }
    }
    while (Dehumidifier = ON) {
      if (indoorhumidityprediction > 40)
    }
    
    Serial.println(42.1125 + 0.024074*forecast_hours_1_precipIntensity -0.10937*forecast_hours_1_uvIndex + 0.90715*forecast_hours_1_temperature + 0.067339*forecast_hours_1_humidity);
    // delay the polled loop
    delay(READ_DELAY * 1000);
    }
    https.end();   //Close connection
  }
  // Delay
  delay(15000);
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
