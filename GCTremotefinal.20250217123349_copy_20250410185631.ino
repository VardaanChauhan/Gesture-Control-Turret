#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <WiFi.h>
#include <esp_now.h>

#define BUTTON_PIN 5  // Tactile switch connected here

MPU9250_asukiaaa mySensor;

uint8_t receiverAddress[] = {0xF4, 0x65, 0x0B, 0xEA, 0xB1, 0xEC}; // Replace with turret ESP32 MAC

typedef struct DataPacket {
  int panAngle;   // 0 to 180
  int tiltAngle;  // 60 to 120
  bool fire;      // true when button is pressed
} DataPacket;

DataPacket dataToSend;

void initMPU() {
  Wire.begin();
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  delay(2000); // Allow sensor to settle
}

void initESPNow() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    while (true);
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    while (true);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  initMPU();
  initESPNow();
}

void loop() {
  mySensor.accelUpdate();
  float ax = mySensor.accelX() * 1000; // in milli-g
  float ay = mySensor.accelY() * 1000;

  // Map acceleration to servo angles
  int panAngle = map(ax, -2000, 2000, 0, 180);
  int tiltAngle = map(ay, -2000, 2000, 60, 120);

  // 🛑 Apply dead zone smoothing
  if (panAngle >= 94 && panAngle <= 105) {
    panAngle = 99;
  }
  if (tiltAngle >= 87 && tiltAngle <= 93) {
    tiltAngle = 90;
  }

  // Read fire button
  bool fire = digitalRead(BUTTON_PIN) == LOW;

  // Pack data
  dataToSend.panAngle = panAngle;
  dataToSend.tiltAngle = tiltAngle;
  dataToSend.fire = fire;

  // Send data
  esp_now_send(receiverAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));

  delay(100); // 10Hz update rate
}
