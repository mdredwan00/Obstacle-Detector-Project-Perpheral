🚧 Obstacle Detection System Using Ultrasonic Sensor & 360° Servo Motor
📌 Project Overview

This project is an Arduino-based obstacle detection and safety system that uses an HC-SR04 ultrasonic sensor to detect nearby objects and control a 360-degree continuous rotation servo motor.
Based on the distance of the obstacle, the system provides visual alerts using LEDs and audio alerts using a buzzer, and automatically controls the motor speed and stop behavior.

🎯 Objectives

Detect obstacles using an ultrasonic sensor

Control a 360° servo motor based on distance

Provide warning and emergency alerts

Improve safety using automation

🧩 Components Used

Arduino Uno

HC-SR04 Ultrasonic Sensor

360° Continuous Rotation Servo Motor

LEDs (3)

Buzzer

Resistors

Breadboard

Jumper Wires

⚙️ Working Principle

The ultrasonic sensor sends sound waves and receives the echo.

Distance is calculated using the time delay of the echo.

Based on the measured distance:

Normal Mode (> 50 cm):

Servo rotates at full speed

LEDs show a running pattern

Buzzer remains OFF

Warning Mode (20–50 cm):

Servo slows down

LEDs blink in sequence

Buzzer beeps intermittently

Emergency Mode (< 20 cm):

Servo stops immediately

All LEDs turn ON

Buzzer sounds continuously

The servo automatically changes direction after completing multiple full rotations.

📏 Distance Thresholds
Distance	System Action
> 50 cm	Full speed rotation
20–50 cm	Slow rotation + warning
< 20 cm	Emergency stop
🔌 Pin Configuration
Component	Arduino Pin
Ultrasonic Trigger	9
Ultrasonic Echo	10
Servo Motor	6
LED 1	7
LED 2	2
LED 3	4
Buzzer	8
🖼️ Circuit Diagram

🛠️ Hardware Setup

📂 Project Structure
Obstacle_Detection_360_Servo/
│
├── Obstacle_Detection_360_Servo.ino
├── README.md
└── images/
    ├── circuit.jpg
    └── setup.png

🚀 Applications

Obstacle-avoiding robots

Smart vehicles

Industrial safety systems

Automated monitoring systems

✅ Advantages

Real-time obstacle detection

Improved safety

Simple and cost-effective

Easy to extend and modify

🧪 Future Improvements

Add LCD display for distance

Wireless monitoring using Bluetooth/Wi-Fi

Integration with mobile app

Use of AI for intelligent decision-making

🏁 Conclusion

This project demonstrates how sensors, actuators, and alert systems can be combined to create an automated safety solution. It provides hands-on experience with embedded systems, real-time control, and hardware interfacing
