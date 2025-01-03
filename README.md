# RFID Temperature Monitoring System with Arduino

This project was originally developed in November 2021 as a school project at *The Prince Royal's College, Chiang Mai, Thailand*. The system combines RFID authentication with temperature monitoring to create an automated attendance and health screening solution.

> [!IMPORTANT]
> - This code base is from 2021 and may require updates for current library versions
> - LINE Notify service used in this project is being discontinued
> - API keys and credentials in code samples are non-functional examples
> - If implementing a similar system today, consider modern alternatives for notifications

## Features
- Contactless temperature measurement using MLX90614 IR sensor
- Student identification via RFID cards
- Real-time temperature alerts via LINE Notify
- Temperature data logging to Firebase
- Automatic time synchronization via NTP
- Buzzer alerts for high temperature readings

## Hardware Components
- Arduino UNO (RFID + Temperature sensor control)
- ESP8266 NodeMCU (WiFi + Cloud connectivity)
- RC522 RFID reader
- MLX90614 IR temperature sensor
- Passive buzzer
- Breadboard and jumper wires

## Software Architecture
1. **Arduino (`UNO.ino`)**
   - RFID card reading
   - Temperature measurement
   - Buzzer control
   - Serial communication with ESP8266

2. **ESP8266 (`D1.ino`)**
   - WiFi connectivity
   - Firebase integration
   - LINE notifications
   - NTP time synchronization

## Data Flow
1. Student presents RFID card
2. System authenticates student ID
3. Temperature measurement taken
4. Data sent to Firebase
5. LINE notification if temperature exceeds threshold
6. Results displayed on web interface

## Screenshots


<div align="center">
   Web dashboard showing real-time temperature logs
   <div align="center">
<img src="https://raw.githubusercontent.com/npwitk/Arduino-Temperature-RFID-Monitoring-System/refs/heads/main/Images/web-interface.png" width="600" alt="Web Interface"/>
</div>
</div>



<div align="center">
   Firebase JSON structure for data storage
   <div align="center">
<img src="https://raw.githubusercontent.com/npwitk/Arduino-Temperature-RFID-Monitoring-System/refs/heads/main/Images/firebase-json.png" width="600" alt="Firebase Database"/>
</div>
</div>


<div align="center">
   Temperature alert notifications via LINE
   <div align="center">
<img src="https://raw.githubusercontent.com/npwitk/Arduino-Temperature-RFID-Monitoring-System/refs/heads/main/Images/line-notification.png" width="400" alt="LINE Notifications"/>
</div>
</div>

## Setup Requirements
1. Arduino IDE
2. Required Libraries:
   - Adafruit_MLX90614
   - MFRC522
   - FirebaseESP8266
   - TridentTD_LineNotify
   - NTPClient

## Historical Context
This system was developed during the COVID-19 pandemic to automate temperature screening in schools. While some services (like LINE Notify) are being discontinued, the core functionality remains relevant for automated health screening systems.

## License
This project is shared for educational purposes. Feel free to modify and improve upon it while maintaining appropriate attribution.
