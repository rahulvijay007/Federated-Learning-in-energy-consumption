# Federated Learning for Energy Consumption Forecasting

## Overview

This project implements a system for real-time energy consumption monitoring and forecasting using Arduino, ESP32, and suitable sensors. The project leverages federated learning to update the prediction model locally while preserving data privacy and ensuring efficient communication between edge devices and a central server.

## Features

- **Real-time Energy Monitoring**: Uses voltage and current sensors (ZMPT101B and ZMCT103C) to measure real-time energy consumption.
- **Federated Learning**: Local model updates are performed on edge devices and aggregated on a central server.
- **Wireless Communication**: Data transmission between edge devices and the server via WiFi.
- **User Interface**: A web-based dashboard for data visualization and analysis.

## Hardware Requirements

- Arduino Uno
- ESP32
- Voltage Sensor (ZMPT101B)
- Current Sensor (ZMCT103C)
- Breadboard and Jumper Wires
- Power Supply
- 10W LED Bulb (for testing)

## Software Requirements

- Arduino IDE
- Python 3.x
- Flask
- SQLite
- Pandas
- Matplotlib or Plotly

## Circuit Connections

### Arduino to Breadboard

- **Voltage Sensor (ZMPT101B)**:
  - VCC to Arduino 5V
  - GND to Arduino GND
  - OUT to Arduino A0

- **Current Sensor (ZMCT103C)**:
  - VCC to Arduino 5V
  - GND to Arduino GND
  - OUT to Arduino A1

### Arduino to ESP32

- **I2C Communication**:
  - Arduino SDA (A4) to ESP32 SDA (D21)
  - Arduino SCL (A5) to ESP32 SCL (D22)
  - GND to GND

## Setup and Installation

### Arduino

1. **Install Arduino Libraries**:
   - WiFi
   - Wire

2. **Upload Code to Arduino**:
   - Load the provided Arduino sketch onto the Arduino Uno.

### ESP32

1. **Install ESP32 Libraries**:
   - WiFi
   - HTTPClient
   - Wire

2. **Upload Code to ESP32**:
   - Load the provided ESP32 sketch onto the ESP32.

### Server

1. **Set Up Server**:
   - Install Flask and SQLite:
     ```sh
     pip install Flask sqlite3 pandas matplotlib plotly
     ```

2. **Run Server**:
   - Start the Flask server:
     ```sh
     python server.py
     ```

### Web Dashboard

1. **Run the Dashboard**:
   - Ensure the server is running and accessible.
   - Open the dashboard URL in a web browser:
     ```
     http://127.0.0.1:8080
     ```

## Usage

1. **Power On Devices**:
   - Ensure the Arduino and ESP32 are powered on and connected to the WiFi network.

2. **Monitor Data**:
   - View real-time data and aggregated results on the web dashboard.

3. **Visualize Data**:
   - Use the provided Python scripts to retrieve and visualize data from the SQLite database.

## Future Improvements

- **Enhanced Sensor Integration**: Adding more sensors for comprehensive monitoring.
- **Advanced Machine Learning Models**: Implementing sophisticated models for better accuracy.
- **Edge Computing Optimization**: Optimizing algorithms for faster processing.
- **Smart Home Integration**: Extending to control smart home devices.
- **Security Enhancements**: Implementing advanced encryption and authentication.
- **UI Refinements**: Improving the web dashboard for better user experience.
- **Cloud Integration**: Exploring cloud options for scalability.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgements

- Inspired by various open-source projects and research on federated learning and energy monitoring.

## Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss your ideas.

## Contact

For any inquiries or support, please contact.

---

**Note**: Ensure to replace placeholder texts like `"your-ssid"`, `"your-password"`, and server details with actual values relevant to your setup.
