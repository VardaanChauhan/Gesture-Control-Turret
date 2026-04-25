#include <Wire.h>
#include <Adafruit_BMP085.h>      // BMP180
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <MAX6675.h>
#include <TinyGPS++.h>

// ---- Pin Definitions ----
#define MQ2_PIN 32
#define MQ7_PIN 33
#define MQ131_PIN 34

#define DHT_PIN 23
#define DHT_TYPE DHT22

#define MAX6675_SCK 14
#define MAX6675_CS  12
#define MAX6675_SO  13

// GPS UART pins
#define NEO6M_RX 16
#define NEO6M_TX 17
#define NEOM8M_RX 4
#define NEOM8M_TX 0

// ---- Sensor Objects ----
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;               // BMP180
Adafruit_MPU6050 mpu;
MAX6675 thermocouple(MAX6675_SCK, MAX6675_CS, MAX6675_SO);
TinyGPSPlus gps6;
TinyGPSPlus gps8;
HardwareSerial gps6Serial(2);      // UART2 for NEO-6M
HardwareSerial gps8Serial(1);      // UART1 for NEO-M8M

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // I2C Pins

  // Init sensors
  dht.begin();
  bmp.begin();
  mpu.begin();

  // Init GPS
  gps6Serial.begin(9600, SERIAL_8N1, NEO6M_RX, NEO6M_TX);
  gps8Serial.begin(9600, SERIAL_8N1, NEOM8M_RX, NEOM8M_TX);

  Serial.println("✅ All Sensors Initialized!");
}

void loop() {
  // Gas Sensors
  int mq2 = analogRead(MQ2_PIN);
  int mq7 = analogRead(MQ7_PIN);
  int mq131 = analogRead(MQ131_PIN);

  // DHT22
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  // BMP180
  float pressure = bmp.readPressure() / 100.0;
  float altitude = bmp.readAltitude();

  // MPU6050
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  // MAX6675
  float thermoTemp = thermocouple.getCelsius();

  // GPS 6M read
  while (gps6Serial.available()) {
    gps6.encode(gps6Serial.read());
  }

  // GPS M8M read
  while (gps8Serial.available()) {
    gps8.encode(gps8Serial.read());
  }

  // Serial Output
  Serial.println("\n------- Sensor Readings -------");

  Serial.print("MQ2: "); Serial.println(mq2);
  Serial.print("MQ7: "); Serial.println(mq7);
  Serial.print("MQ131: "); Serial.println(mq131);

  Serial.print("DHT22 Temp: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(hum);

  Serial.print("BMP180 Pressure (hPa): "); Serial.println(pressure);
  Serial.print("Altitude (m): "); Serial.println(altitude);

  Serial.print("Accel X: "); Serial.println(a.acceleration.x);
  Serial.print("Accel Y: "); Serial.println(a.acceleration.y);
  Serial.print("Accel Z: "); Serial.println(a.acceleration.z);

  Serial.print("Gyro X: "); Serial.println(g.gyro.x);
  Serial.print("Gyro Y: "); Serial.println(g.gyro.y);
  Serial.print("Gyro Z: "); Serial.println(g.gyro.z);

  Serial.print("Thermocouple °C: "); Serial.println(thermoTemp);

  // GPS 6M Output
  Serial.println("--- NEO 6M GPS ---");
  if (gps6.location.isValid()) {
    Serial.print("Lat: "); Serial.println(gps6.location.lat(), 6);
    Serial.print("Lng: "); Serial.println(gps6.location.lng(), 6);
    Serial.print("Sats: "); Serial.println(gps6.satellites.value());
  } else {
    Serial.println("Waiting for fix...");
  }

  // GPS M8M Output
  Serial.println("--- NEO M8M GPS ---");
  if (gps8.location.isValid()) {
    Serial.print("Lat: "); Serial.println(gps8.location.lat(), 6);
    Serial.print("Lng: "); Serial.println(gps8.location.lng(), 6);
    Serial.print("Sats: "); Serial.println(gps8.satellites.value());
  } else {
    Serial.println("Waiting for fix...");
  }

  Serial.println("-----------------------------\n");

  delay(1000);
}