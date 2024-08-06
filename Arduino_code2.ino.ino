#include <Wire.h>

#define VOLTAGE_SENSOR_PIN A0
#define CURRENT_SENSOR_PIN A1
#define SAMPLING_INTERVAL 5000 // Adjust sampling interval as needed

void setup() {
  Serial.begin(115200);
  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
}

void loop() {
  // Read sensor data
  float voltage = readVoltage(VOLTAGE_SENSOR_PIN);
  float current = readCurrent(CURRENT_SENSOR_PIN);

  // Perform feature extraction
  float feature1 = voltage * current; // Example feature extraction
  
  // Perform lightweight model update (Federated Learning)
  float updatedWeight = performFederatedLearning(feature1);

  // Transmit model update to ESP32
  sendModelUpdateToESP32(updatedWeight);

  delay(SAMPLING_INTERVAL);
}

float readVoltage(int pin) {
  int sensorValue = analogRead(pin);
  float voltage = (sensorValue / 1023.0) * 5.0; // Convert to voltage
  return voltage;
}

float readCurrent(int pin) {
  int sensorValue = analogRead(pin);
  float current = (sensorValue / 1023.0) * 5.0; // Convert to current
  return current;
}

float performFederatedLearning(float feature) {
  // Example lightweight model update (Federated Learning)
  // In a real scenario, this would involve updating model parameters based on local data
  float updatedWeight = feature * 0.1; // Update weight based on extracted feature
  return updatedWeight;
}

void sendModelUpdateToESP32(float weight) {
  Wire.beginTransmission(8); // Address of ESP32
  Wire.write((byte*)&weight, sizeof(weight)); // Send weight
  Wire.endTransmission();
}
