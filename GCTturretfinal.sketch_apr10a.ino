#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Servo objects
Servo servoPan;
Servo servoTilt;

#define PAN_PIN   18
#define TILT_PIN  19
#define FIRE_PIN  23   // Relay / LED / MOSFET trigger

// Structure MUST match transmitter exactly
typedef struct DataPacket {
  int panAngle;
  int tiltAngle;
  bool fire;
} DataPacket;

DataPacket receivedData;

// Callback function when data is received
void onDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Debug print
  Serial.print("Pan: ");
  Serial.print(receivedData.panAngle);
  Serial.print(" | Tilt: ");
  Serial.print(receivedData.tiltAngle);
  Serial.print(" | Fire: ");
  Serial.println(receivedData.fire);

  // Apply constraints (safety)
  int pan = constrain(receivedData.panAngle, 0, 180);
  int tilt = constrain(receivedData.tiltAngle, 60, 120);

  // Move servos
  servoPan.write(pan);
  servoTilt.write(tilt);

  // Fire control
  digitalWrite(FIRE_PIN, receivedData.fire ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  // Servo setup
  servoPan.attach(PAN_PIN);
  servoTilt.attach(TILT_PIN);

  pinMode(FIRE_PIN, OUTPUT);
  digitalWrite(FIRE_PIN, LOW);

  // WiFi in station mode (required for ESP-NOW)
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Turret Ready. Waiting for data...");
}
