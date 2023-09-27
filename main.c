#include <Arduino.h>
#include <WiFiNINA.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

/************************* WiFi Setup **********************************/

#define WIFI_SSID       "wifi_ssid"
#define WIFI_PASS       "wifi_password"

/************************* AWS IoT Core Setup ***************************/

#define AIO_SERVER      "iot-endpoint.iot.us-west-2.amazonaws.com"  // Update with your IoT Core endpoint
#define AIO_SERVERPORT  8883
#define AIO_USERNAME    "iot-username"  // Update with your IoT Core username
#define AIO_KEY         "iot-key"       // Update with your IoT Core key
#define AIO_SECRET      "iot-secret"    // Update with your IoT Core secret

/************************* Sensor Pins *********************************/

const int humiditySensorPin = A0; // Analog pin for humidity sensor
const int temperatureSensorPin = A1; // Analog pin for temperature sensor

/************************* MQTT Setup *********************************/

WiFiSSLClient wifiClient;
Adafruit_MQTT_Client mqtt(&wifiClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY, AIO_SECRET);

/************************* MQTT Feeds ***************************************/

Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/humidity");
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/temperature");

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  mqtt.subscribe(&humidityFeed);
  mqtt.subscribe(&temperatureFeed);
}

void loop() {
  // Read data from sensors
  float humidity = readHumiditySensor();
  float temperature = readTemperatureSensor();

  // Publish humidity and temperature to AWS IoT Core
  publishData(humidity, temperature);

  delay(1000); // Publish every 1 seconds
}

float readHumiditySensor() {
  return random(30, 60); // Replace with actual sensor reading
}

float readTemperatureSensor() {
  return random(20, 30); // Replace with actual sensor reading
}

void publishData(float humidity, float temperature) {
  // Publish humidity and temperature to MQTT topics
  if (!humidityFeed.publish(humidity)) {
    Serial.println(F("Failed to publish humidity"));
  }

  if (!temperatureFeed.publish(temperature)) {
    Serial.println(F("Failed to publish temperature"));
  }
}
