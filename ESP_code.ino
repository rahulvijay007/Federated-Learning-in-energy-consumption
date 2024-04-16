#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <SD.h>
#include <RTClib.h>

#define VOLTAGE_SENSOR_PIN A0
#define CURRENT_SENSOR_PIN A1
#define WIFI_SSID "your-ssid"
#define WIFI_PASSWORD "your-password"
#define SERVER_ADDRESS "your-server-address"
#define SERVER_PORT 80

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  while (!Serial) continue;

  // Initialize SD card
  if (!SD.begin(5)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card initialized.");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected.");
}

void loop() {
  // Read sensor data
  float voltage = analogRead(VOLTAGE_SENSOR_PIN) * (5.0 / 4095.0); // Convert ADC value to voltage
  float current = analogRead(CURRENT_SENSOR_PIN) * (5.0 / 4095.0); // Convert ADC value to voltage

  // Get current time from RTC
  DateTime now = rtc.now();

  // Write data to SD card
  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.timestamp());
    dataFile.print(",");
    dataFile.print(voltage);
    dataFile.print(",");
    dataFile.println(current);
    dataFile.close();
    Serial.println("Data written to SD card.");
  } else {
    Serial.println("Error opening data file.");
  }

  // Transmit data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin("http://" + String(SERVER_ADDRESS) + ":" + String(SERVER_PORT) + "/upload");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Construct POST data
    String data = "timestamp=" + String(now.timestamp()) + "&voltage=" + String(voltage) + "&current=" + String(current);

    // Send HTTP POST request
    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(5000); // Adjust sampling interval as needed
}
