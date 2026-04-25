#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;

Servo servoPan;
Servo servoTilt;

// Raw values
int16_t ax, ay, az;

// Servo angles
int panAngle = 90;
int tiltAngle = 90;

// Sensitivity tuning
float sensitivity = 0.05;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  servoPan.attach(9);   // Pan servo
  servoTilt.attach(10); // Tilt servo

  servoPan.write(panAngle);
  servoTilt.write(tiltAngle);
}

void loop() {
  mpu.getAcceleration(&ax, &ay, &az);

  // Normalize values
  float normX = ax / 16384.0;
  float normY = ay / 16384.0;

  // Map tilt (forward/backward movement)
  tiltAngle += normY * sensitivity * 100;

  // Map pan (left/right movement)
  panAngle += normX * sensitivity * 100;

  // Constrain angles
  panAngle = constrain(panAngle, 0, 180);
  tiltAngle = constrain(tiltAngle, 0, 180);

  // Move servos
  servoPan.write(panAngle);
  servoTilt.write(tiltAngle);

  // Debug
  Serial.print("Pan: ");
  Serial.print(panAngle);
  Serial.print(" | Tilt: ");
  Serial.println(tiltAngle);

  delay(50);
}
