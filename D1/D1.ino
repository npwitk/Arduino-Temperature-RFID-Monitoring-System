#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>
#include <SoftwareSerial.h>

// Configuration
struct Config {
    const char* WIFI_SSID = "WIFI_SSID";
    const char* WIFI_PASSWORD = "WIFI_PASSWORD";
    const char* FIREBASE_HOST = "FIREBASE_HOST";
    const char* FIREBASE_KEY = "FIREBASE_KEY";
    const char* LINE_TOKEN = "LINE_TOKEN";
    const float TEMP_THRESHOLD = 37.5;
    const int SERIAL_BAUD = 9600;
    const int NODESERIAL_BAUD = 57600;
};

// Student data structure
struct Student {
    int id;
    const char* name;
};

// Global variables
const Config config;
const Student STUDENTS[] = {
    {39056, "Hug"},
    {39861, "Prom"},
    {43012, "Danny"},
    {36939, "Beau"},
    {43013, "Mei-mei"},
    {37105, "Google"},
    {36954, "Aum"},
    {44872, "Minnie"},
    {42437, "Cake"}
};

// Initialize components
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "th.pool.ntp.org", 25200, 60000);
FirebaseData firebaseData;
SoftwareSerial NodeSerial(D8, D2); // RX | TX

void setupWiFi() {
    WiFi.begin(config.WIFI_SSID, config.WIFI_PASSWORD);
    Serial.printf("WiFi connecting to %s\n", config.WIFI_SSID);
    
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(400);
    }
    
    Serial.printf("\nWiFi connected\nIP : ");
    Serial.println(WiFi.localIP());
}

void setupPins() {
    pinMode(D8, INPUT);
    pinMode(D2, OUTPUT);
}

const char* findStudentName(int studentId) {
    for (const auto& student : STUDENTS) {
        if (student.id == studentId) {
            return student.name;
        }
    }
    return "Unknown";
}

void notifyTemperature(const char* owner, float temp) {
    String message;
    if (temp >= config.TEMP_THRESHOLD) {
        message = "⚠️ " + String(owner) + "'s Body Temperature is high (" + String(temp) + "°C) ⚠️";
    } else {
        message = String(owner) + "'s Body Temperature is normal (" + String(temp) + "°C)";
    }
    
    Serial.println(message);
    LINE.notify(message);
}

void sendToFirebase(const char* owner, int studentId, float temp) {
    FirebaseJson data;
    data.set("time", timeClient.getFormattedTime());
    data.set("student_id", studentId);
    data.set("name", owner);
    data.set("body_temp", temp);

    if (Firebase.pushJSON(firebaseData, "/entrydata", data)) {
        Serial.println("Pushed: " + firebaseData.pushName());
    } else {
        Serial.println("Error: " + firebaseData.errorReason());
    }
}

void processSerialData() {
    if (NodeSerial.available() > 0) {
        float temp = NodeSerial.parseFloat();
        int studentId = NodeSerial.parseInt();
        
        const char* owner = findStudentName(studentId);
        
        Serial.printf("Owner: %s, Temperature: %.1f, ID: %d\n", owner, temp, studentId);
        
        timeClient.update();
        notifyTemperature(owner, temp);
        sendToFirebase(owner, studentId, temp);
        
        delay(100);
    }
}

void setup() {
    Serial.begin(config.SERIAL_BAUD);
    NodeSerial.begin(config.NODESERIAL_BAUD);
    
    setupPins();
    setupWiFi();
    
    LINE.setToken(config.LINE_TOKEN);
    Firebase.begin(config.FIREBASE_HOST, config.FIREBASE_KEY);
    timeClient.begin();
    
    Serial.println("NodeMCU/ESP8266 Ready");
}

void loop() {
    processSerialData();
}