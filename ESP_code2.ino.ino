#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

#define WIFI_SSID "iQOO Neo6"
#define WIFI_PASSWORD "90999909@#"
#define SERVER_ADDRESS "http://127.0.0.1:8080"

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  Wire.begin(8); // Join I2C bus as master with address 8
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Nothing needed here, everything is handled by receiveEvent
  delay(1000); // Adjust delay as needed
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Attempting to connect to WPA SSID: " + String(WIFI_SSID));
  }
  Serial.println("Connected to WiFi");
}

void receiveEvent(int howMany) {
  float weight;
  Wire.readBytes((char*)&weight, sizeof(weight)); // Read weight from Arduino
  forwardDataToServer(weight);
}

void forwardDataToServer(float weight) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverAddress = String(SERVER_ADDRESS) + "/update";

    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "weight=" + String(weight);
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      Serial.println("Data forwarded to central server successfully. Response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error forwarding data to central server. Response code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to forward data to central server.");
  }
}
