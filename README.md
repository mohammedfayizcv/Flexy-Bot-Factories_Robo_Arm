# Flexy Bot Factories

## Project Overview and Introduction

**Project Name:** Flexy Bot Factories

**Objective:**  
The Flexy Bot Factories project aims to create an automated robotic system that can detect flames and animals using sensors and cameras. This system can operate in both manual and automatic modes, providing flexibility for various tasks.

---

## Components Used

- **Arduino microcontroller**
- **Servo motors** for movement
- **Flame sensor**
- **Joystick** for manual control
- **Camera** for animal detection
- **Python** for image processing and communication with Arduino

The project combines hardware control (using Arduino) with software (using Python) to create an interactive and responsive robotic system.

---

## Methodology

### Hardware Setup
1. Connect the servo motors, flame sensor, and buttons to the Arduino.
2. Set up the camera for animal detection.

### Arduino Code
- Implement control logic for the servos based on joystick input and button presses.
- Use a flame sensor to detect flames and trigger actions accordingly.
- Communicate with the Python script via serial communication.

### Python Code
- Use OpenCV for object detection (specifically animals) using the YOLO model.
- Play a siren sound upon detection of animals or flames.
- Provide a GUI using Tkinter to display notifications and control the flow of the program.

### Testing
- Test the system in various environments to ensure reliability in detecting flames and animals.
- Verify the communication between Arduino and Python.

---

## Connections

### Arduino Connections
- **Servo Motors:**
  - Servo_X connected to pin 9
  - Servo_Y connected to pin 10
  - Servo_Grip connected to pin 11
- **Flame Sensor:**  
  Connected to pin A2
- **Buttons:**
  - Grip button connected to pin 2
  - Mode button connected to pin 5
- **LED Indicator:**  
  Red LED connected to pin 12
- **Joystick:**
  - X-axis connected to pin A0
  - Y-axis connected to pin A1

### Python Connections
- Serial communication established with the Arduino using the appropriate COM port.

---

## Library Installation and Usage

### Arduino Libraries
- **Servo Library:** Used to control servo motors.


##include <Servo.h>
Python Libraries
OpenCV: For image processing and animal detection.
bash
Copy code
pip install opencv-python
Tkinter: For creating a GUI.

pip install tk
NumPy: For numerical operations.

pip install numpy
Pygame: For playing sounds.

pip install pygame
PySerial: For serial communication with Arduino.

pip install pyserial


##Future Expansion
-Enhanced Object Detection
-Integrate additional machine learning models to detect more objects or specific animal breeds.
-Improved User Interface
-Develop a more sophisticated GUI with additional controls and features for better user interaction.
-Remote Monitoring
-Implement a web server to monitor the system remotely and receive alerts.

**Data Logging**
Add functionality to log events (e.g., flame detection, animal detection) for analysis and reporting.
Integration with IoT
Connect the system to IoT platforms to enable remote control and monitoring from mobile devices.
Battery Management
Explore battery management solutions for autonomous operation.
Advanced Automation
Develop more complex automated routines based on detected conditions (e.g., different actions based on the type of animal detected).
