#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "MFRC522.h"

// Pin definitions
const int BUZZER_PIN = 5;
const int RST_PIN = 9;
const int SS_PIN = 10;
const int RX_PIN = 2;
const int TX_PIN = 3;

// Configuration
const float TEMP_THRESHOLD = 37.5;
const int SERIAL_BAUD = 9600;
const int UNOSERIAL_BAUD = 57600;
const int BUZZER_DELAY = 200;

// RFID card structure
struct RFIDCard {
    const char* uid;
    const char* name;
    long studentId;
};

// RFID cards database
const RFIDCard CARDS[] = {
    {"CE 55 97 D9", "Hug", 39056},
    {"8E 2C 72 5B", "Prom", 39861},
    {"AE DE AD D9", "Danny", 43012},
    {"3E E8 5E 5B", "Beau", 36939},
    {"47 7F 3B 21", "Mei-mei", 43013},
    {"97 98 22 30", "Cake", 42437},
    {"67 85 B6 46", "Google", 37105},
    {"97 44 42 21", "Aum", 36954},
    {"67 01 22 30", "Minnie", 44872}
};

// Initialize components
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SoftwareSerial UnoSerial(RX_PIN, TX_PIN);
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Global variables
int state = 1;
long currentStudentId = 0;
float temperature = 0;

void setupPins() {
    pinMode(RX_PIN, INPUT);
    pinMode(TX_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void setupCommunication() {
    Serial.begin(SERIAL_BAUD);
    UnoSerial.begin(UNOSERIAL_BAUD);
    SPI.begin();
}

String readRFIDCard() {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    return content.substring(1);
}

const RFIDCard* findCard(const String& uid) {
    for (const auto& card : CARDS) {
        if (uid == card.uid) {
            return &card;
        }
    }
    return nullptr;
}

void playBuzzerPattern(int beeps) {
    for (int i = 0; i < beeps; i++) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(BUZZER_DELAY);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BUZZER_DELAY);
    }
}

void handleRFIDScan() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String rfid = readRFIDCard();
        const RFIDCard* card = findCard(rfid);
        
        if (card) {
            Serial.printf("Named to %s\n", card->name);
            currentStudentId = card->studentId;
            state = 2;
        } else {
            Serial.println("Not Registered");
        }
    }
    delay(100);
}

void measureTemperature() {
    playBuzzerPattern(1);
    
    temperature = mlx.readObjectTempC();
    
    UnoSerial.print("\n");
    UnoSerial.print(temperature);
    delay(100);
    UnoSerial.print("\r");
    UnoSerial.print(currentStudentId);
    
    Serial.println(currentStudentId);
    Serial.println(temperature);
    
    delay(500);
    state = 3;
}

void handleTemperatureAlert() {
    if (temperature >= TEMP_THRESHOLD) {
        playBuzzerPattern(3);
    }
    playBuzzerPattern(1);
    state = 1;
}

void setup() {
    setupPins();
    setupCommunication();
    mfrc522.PCD_Init();
    mlx.begin();
    
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("System Ready");
}

void loop() {
    switch (state) {
        case 1:
            handleRFIDScan();
            break;
            
        case 2:
            measureTemperature();
            break;
            
        case 3:
            handleTemperatureAlert();
            break;
    }
}