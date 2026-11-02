# Smart-Dustbin-Using-NodeMCU-ESP8266

### An Automatic Waste Bin with Smart Lid Control and Real-Time Fill-Level Monitoring

![Platform](https://img.shields.io/badge/Platform-NodeMCU%20ESP8266-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B-orange)
![IDE](https://img.shields.io/badge/IDE-Arduino-green)
![Sensors](https://img.shields.io/badge/Sensors-2%20x%20HC--SR04-yellow)
![Status](https://img.shields.io/badge/Project-Prototype-purple)

## Project Overview

The **Smart Dustbin Using NodeMCU ESP8266** is an automated waste management system designed to improve hygiene, convenience, and waste collection efficiency.

The system uses two ultrasonic sensors for separate purposes. The first sensor measures the distance between the top of the dustbin and the waste to estimate the bin's fill percentage. The second sensor detects a person's hand or an object approaching the bin and triggers a servo motor to open the lid automatically.

When the estimated fill level reaches 80%, the system displays a servicing alert on the Serial Monitor. When an object is detected within 20 cm, the lid opens and automatically closes after 3.5 seconds.

The project demonstrates how embedded systems and sensor-based automation can be used to develop a practical smart waste management solution.

## Features

- **Automatic Lid Opening:** Detects a nearby hand or object and opens the dustbin lid using a servo motor.
- **Contactless Waste Disposal:** Allows users to operate the lid without touching it.
- **Waste Fill-Level Monitoring:** Measures the distance between the sensor and the waste.
- **Fill Percentage Calculation:** Estimates bin occupancy on a scale from 0% to 100%.
- **80% Full Alert:** Displays a servicing warning when the calculated fill level reaches or exceeds 80%.
- **Automatic Lid Closing:** Closes the lid 3.5 seconds after it opens.
- **Real-Time Serial Monitoring:** Displays sensor readings, fill percentage, and system status.
- **Dual-Sensor Architecture:** Separates waste-level monitoring from person detection.

## 🛠️ Hardware Requirements

| Component | Quantity | Purpose |
|---|---:|---|
| NodeMCU ESP8266 ESP-12E | 1 | Main microcontroller |
| HC-SR04 ultrasonic sensor | 2 | Waste-level and proximity detection |
| SG90 servo motor | 1 | Opens and closes the lid |
| External regulated 5V supply | 1 | Powers the servo motor |
| Resistors for voltage dividers | As required | Reduces Echo signal voltage |
| Jumper wires | As required | Circuit connections |
| Dustbin and lid mechanism | 1 | Physical enclosure |
| Breadboard or PCB | Optional | Circuit assembly |

## Software Requirements

- Arduino IDE
- ESP8266 board support package
- Servo library compatible with the ESP8266
- USB cable for programming the NodeMCU
- Serial Monitor configured to **115200 baud**

## 🔌 Circuit Connections

### 1. Ultrasonic Sensor 1 — Waste-Level Detection

Position this sensor at the top of the dustbin, facing downward toward the waste.

| HC-SR04 Pin | NodeMCU ESP8266 |
|---|---|
| VCC | VIN / suitable regulated 5V supply |
| GND | GND |
| TRIG | D1 (GPIO5) |
| ECHO | D2 (GPIO4), through a voltage divider |

### 2. Ultrasonic Sensor 2 — Person Detection

Position this sensor facing outward so it can detect a hand or object approaching the dustbin.

| HC-SR04 Pin | NodeMCU ESP8266 |
|---|---|
| VCC | VIN / suitable regulated 5V supply |
| GND | GND |
| TRIG | D5 (GPIO14) |
| ECHO | D6 (GPIO12), through a voltage divider |

### 3. SG90 Servo Motor — Lid Control

| Servo Wire | Connection |
|---|---|
| Signal (orange/yellow) | D7 (GPIO13) |
| VCC (red) | External regulated 5V |
| GND (brown/black) | Common GND |

### Important Electrical Precautions

- The ESP8266 GPIO pins operate at 3.3V logic and are not 5V tolerant.
- **Both HC-SR04 Echo outputs must be reduced to approximately 3.3V using voltage dividers** before connecting to the NodeMCU.
- Use an appropriately rated external 5V supply for the servo if required by its current demand.
- Connect the external power supply ground, sensor grounds, and NodeMCU ground together.
- Verify the board's VIN power requirements before connecting a 5V supply.
- Avoid powering a servo from a GPIO pin.

## Working Principle

The Smart Dustbin operates through two independent sensing functions managed by the NodeMCU ESP8266.

### Waste-Level Monitoring

1. The first ultrasonic sensor measures the distance to the waste surface.
2. The NodeMCU converts the measured distance into an estimated fill percentage.
3. The percentage is constrained between 0% and 100%.
4. The fill percentage is displayed on the Serial Monitor.
5. When the estimated fill level reaches 80%, a servicing alert is printed.

The calculation uses these configured reference distances:

- Empty-bin distance: 25 cm
- Full-bin distance: 5 cm

The fill percentage is calculated as:

```text
Fill Percentage =
((Empty Distance - Measured Distance) /
 (Empty Distance - Full Distance)) × 100
```

The calculated result is limited to the range of 0–100%.

### Automatic Lid Control

1. The second ultrasonic sensor continuously checks the distance to a nearby object.
2. If an object is detected within 20 cm, the NodeMCU activates the servo.
3. The servo moves to the configured open position of 0°.
4. The lid remains open for 3.5 seconds.
5. The servo moves to the configured closed position of 180°.

The actual opening and closing angles may need adjustment according to the mechanical arrangement of the lid.

## System Logic

| Condition | System Response |
|---|---|
| Valid waste-level reading | Calculate and display fill percentage |
| Fill level below 80% | Continue normal monitoring |
| Fill level reaches 80% | Display servicing alert |
| Invalid ultrasonic reading | Display an error/status message |
| Object detected within 20 cm | Open the lid |
| 3.5 seconds elapsed | Close the lid |

## 🚀 Installation and Setup

### Step 1: Install Arduino IDE

Download and install the Arduino IDE from the official website:

https://www.arduino.cc/en/software

### Step 2: Configure the ESP8266 Board

In Arduino IDE, open Preferences and add the ESP8266 Boards Manager URL if it is not already configured.

```text
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Open Boards Manager, search for `esp8266`, and install the ESP8266 platform.

Select the appropriate NodeMCU ESP8266 board from the Tools menu.

### Step 3: Install the Servo Library

Install an ESP8266-compatible Servo library through Library Manager if the selected board package does not already provide the required library.

The source code uses:

```cpp
#include <Servo.h>
```

### Step 4: Connect the Hardware

Wire both ultrasonic sensors and the servo motor according to the pin tables above.

Check the Echo voltage dividers and common ground before powering the circuit.

### Step 5: Upload the Code

1. Connect the NodeMCU to your computer using USB.
2. Open the Smart Dustbin `.ino` file.
3. Select the correct board and COM port.
4. Compile and upload the program.

### Step 6: Open Serial Monitor

Set the Serial Monitor baud rate to **115200**.

You should see startup messages followed by distance measurements, bin fill percentage, proximity readings, and servicing alerts when their configured conditions are met.

## Example Serial Monitor Output

The following is an illustrative example of the expected output, not a captured test result.

```text
SMART DUSTBIN STARTED
----------------------
Trash Distance: 18.0 cm | Bin Fill: 35%
Person Distance: 35.2 cm
----------------------
Trash Distance: 8.0 cm | Bin Fill: 85%
ALERT: BIN NEEDS SERVICING!
Person Distance: 15.4 cm
PERSON DETECTED!
OPENING DUSTBIN LID
DUSTBIN LID CLOSED
----------------------
```

Actual readings depend on the waste surface, sensor placement, power supply, and surrounding environment.

## Testing and Validation

The following tests can be performed to verify system behavior:

| Test | Expected Behavior |
|---|---|
| Measure an empty bin | Fill estimate approaches 0% when the distance is 25 cm |
| Measure a nearly full bin | Fill estimate increases as the measured distance decreases |
| Simulate 80% fill | Servicing alert is displayed |
| Place a hand within 20 cm | Servo moves to the open position |
| Wait 3.5 seconds | Servo moves to the closed position |
| Disconnect or obstruct a sensor | Invalid readings are handled by the code |
| Repeat proximity detection | Lid responds to subsequent detection events |

The test results should be recorded after testing the physical prototype.

## Limitations

- The fill percentage is an estimate based on fixed reference distances and assumes the sensor is mounted appropriately.
- The 80% alert is printed repeatedly while the measured fill percentage remains at or above the threshold.
- The lid closes after 3.5 seconds even if a hand or object is still present.
- Ultrasonic measurements may be affected by irregular waste surfaces, soft materials, sensor alignment, and echoes.
- The two sensors may occasionally interfere with each other; physical separation and measurement timing help reduce this risk.
- The servo angles may require mechanical calibration.
- The current implementation reports status through the Serial Monitor. It does not include a mobile app, cloud dashboard, or wireless notification system.
- The project should be tested for reliable operation before practical deployment.

## Future Improvements

Potential upgrades include:

- **IoT Monitoring:** Send fill-level readings to an online dashboard using the ESP8266's Wi-Fi capabilities.
- **Mobile Notifications:** Notify users when the bin needs emptying.
- **Improved Lid Safety:** Keep the lid open while a person or object remains within the detection range.
- **Fill-Level Notifications:** Trigger a one-time alert when the bin crosses the 80% threshold instead of repeatedly printing it.
- **Data Logging:** Track fill-level changes over time.
- **Power Optimization:** Improve energy efficiency for battery-powered operation.
- **Enhanced Calibration:** Calibrate the empty and full reference distances for the actual dustbin.

## Applications

- Homes and apartments
- Schools and colleges
- Offices and workplaces
- Smart campus projects
- Automated waste disposal prototypes
- Educational IoT and embedded-systems demonstrations

## Learning Outcomes

This project provides practical experience with:

- ESP8266 microcontroller programming
- Ultrasonic distance measurement
- Sensor-based automation
- Servo motor control
- Analog voltage-divider principles for digital input protection
- Embedded C++ programming
- Real-time debugging through Serial Monitor
- Basic smart waste management system design

## Project Information

- **Project Name:** Smart Dustbin Using NodeMCU ESP8266
- **Controller:** NodeMCU ESP8266 ESP-12E
- **Sensors:** Two HC-SR04 ultrasonic sensors
- **Actuator:** SG90 servo motor
- **Programming Language:** Embedded C++
- **Development Environment:** Arduino IDE
- **Project Category:** Embedded Systems and Smart Waste Management

### ♻️ Smart Dustbin — Making Waste Disposal Smarter, Cleaner, and More Convenient.
