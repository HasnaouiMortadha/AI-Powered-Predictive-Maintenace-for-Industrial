#include <Wire.h>
#include <Adafruit_ADXL345_U.h>    // For ADXL345
#include <DHT.h>                   // For DHT22
#include <Adafruit_INA219.h>       // For INA219
#include <WiFi.h>                  // For ESP32 Wi-Fi
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_WIFI_password";

// InfluxDB server URL and authentication
const char* serverURL = "http://127.0.0.1:8086/api/v2/write?org=AI_Powered_Predective&bucket=Industrial_DATA&precision=s";
const char* token = "DATA_Sensor";

// Sensor setup
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
DHT dht(2, DHT22);                 // DHT22 sensor on pin 2
Adafruit_INA219 ina219;
const int soundPin = 36;           // KY-038 sound sensor on analog pin
const int pressurePin = 39;        // MPX5700AP on analog pin

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize sensors
  if (!accel.begin()) {
    Serial.println("Could not find a valid ADXL345 sensor, check wiring!");
    while (1);
  }
  dht.begin();
  ina219.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Authorization", String("Token ") + token);
    http.addHeader("Content-Type", "text/plain");

    // Read sensor data
    sensors_event_t event;
    accel.getEvent(&event);
    float accelX = event.acceleration.x;
    float accelY = event.acceleration.y;
    float accelZ = event.acceleration.z;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    float pressureVoltage = analogRead(pressurePin) * (3.3 / 4095.0); // Convert ADC reading for ESP32 (0-3.3V range)
    float pressure = (pressureVoltage - 0.2) * (700 / 4.5);           // Calculate pressure in kPa

    float current_mA = ina219.getCurrent_mA();
    float voltage = ina219.getBusVoltage_V();
    
    int soundLevel = analogRead(soundPin);  // KY-038

    // Create InfluxDB line protocol data string
    String data = "sensor_data,device=esp32 ";
    data += "accelX=" + String(accelX) + ",";
    data += "accelY=" + String(accelY) + ",";
    data += "accelZ=" + String(accelZ) + ",";
    data += "temperature=" + String(temperature) + ",";
    data += "humidity=" + String(humidity) + ",";
    data += "pressure=" + String(pressure) + ",";
    data += "voltage=" + String(voltage) + ",";
    data += "current_mA=" + String(current_mA) + ",";
    data += "soundLevel=" + String(soundLevel);

    // Send HTTP POST request with data to InfluxDB
    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
      Serial.print("Data sent, response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected, trying to reconnect...");
    WiFi.reconnect();
  }

  delay(10000); // Send data every 10 seconds
}
