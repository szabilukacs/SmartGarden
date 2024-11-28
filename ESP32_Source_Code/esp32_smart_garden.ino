
// Includes

// Wifi
#include <WiFi.h>
#include <HTTPClient.h>

// Temperature
#include <OneWire.h>
#include <DallasTemperature.h>

// Humidity
#include <DHT.h>
#include <DHT_U.h>

// Wifi config settings
#include "wifi_config.h"

// Defines

#define ONE_WIRE_BUS 4    // 1 temperature sensor data pin
#define DHTPIN_IN 13      // humidity sensor pin
#define DHTPIN_OUT 17     // humidity sensor pin
#define DHTTYPE DHT11     // DHT 11
#define POWER_PIN 16      // POWER PIN IN
#define POWER_PIN_TEMP 18 // POWER PIN TEMP
#define POWER_PIN_RAIN 32 // POWER PIN TEMP
#define AN_PIN_RAIN 33    // ANALOG INPUT
#define AN_PIN_LIGTH 34   // ANALOG INPUT
#define D_PIN_DOOR 19
#define uS_TO_S_FACTOR 1000000ULL // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 590ULL      // Time ESP32 will go to sleep (in seconds) 10 second execution time
// 10 minutes deep sleep (minus 6s execution time)
#define SEALEVELPRESSURE_HPA (1022.5) // TGM
#define RAIN_OFFSET 4095              // 1024 mean dry
#define MAX_ERROR 3

// Declare measurments variables
float in_temp = 0.0;
float in_hum = 0.0;
float out_temp = 0.0;
float out_hum = 0.0;
int rain_value = 0;
int lux = 0;
bool door_open = 0;
float in_heat_index = 0.0;
float out_heat_index = 0.0;

// Current error for pressure measurment => TODO: later counter it for other measurments also
int error_counter = 0;

// Temperature setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// Number of temperature devices found
int numberOfDevices;

// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

// Humidity
DHT dht_in(DHTPIN_IN, DHTTYPE);
DHT dht_out(DHTPIN_OUT, DHTTYPE);

void setup()
{

  Serial.begin(9600);

  delay(500); // Take some time to open up the Serial Monitor

  // Initialize the POWER pins as an output
  pinMode(POWER_PIN, OUTPUT);
  pinMode(POWER_PIN_TEMP, OUTPUT);
  pinMode(POWER_PIN_RAIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);      // Turn the LED on (HIGH)
  digitalWrite(POWER_PIN_TEMP, LOW); // Turn the LED on (HIGH)
  digitalWrite(POWER_PIN_RAIN, LOW); // Turn the LED on (HIGH)

  pinMode(AN_PIN_RAIN, INPUT);
  pinMode(AN_PIN_LIGTH, INPUT);
  pinMode(D_PIN_DOOR, INPUT_PULLUP);

  // Error counter
  error_counter = 0;

  connectWiFi();

  // Deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                 " Seconds");

  // Power ON TEMP
  digitalWrite(POWER_PIN_TEMP, HIGH);
  delay(50);

  // Measurments temps
  measure_temperature();
  delay(50);
  digitalWrite(POWER_PIN_TEMP, LOW);

  // Measurments other
  digitalWrite(POWER_PIN, HIGH);
  delay(500);

  measure_humidity();
  delay(50);
  digitalWrite(POWER_PIN, LOW);

  digitalWrite(POWER_PIN_RAIN, HIGH);
  delay(10);
  measure_rain();
  digitalWrite(POWER_PIN_RAIN, LOW);

  delay(50);
  lux = measure_ligth();

  door_open = measure_door();

  if (door_open == true)
    Serial.println("nyitva");
  else
    Serial.println("csukva");
  Serial.println(door_open);

  send_data_to_website();
  delay(50);

  Serial.println("Going to sleep now");

  // Power OFF
  digitalWrite(POWER_PIN, LOW);      // Turn the LED off (LOW)
  digitalWrite(POWER_PIN_TEMP, LOW); // Turn the LED off (LOW)
  digitalWrite(POWER_PIN_RAIN, LOW);
  delay(50);

  esp_deep_sleep_start(); // Go to deep sleep
}

void loop()
{
}
