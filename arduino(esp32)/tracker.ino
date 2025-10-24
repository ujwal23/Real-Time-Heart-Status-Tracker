#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>

// WiFi Credentials
const char* ssid = "vinod_eero";
const char* password = "4082145850";

// Flask server details
const char* flaskServerURL = "http://192.168.4.144:5000/update-data"; // Update with your computer's IP

// Sensor Pins
const int tempPin = 34;
const int ecgPin = 35;
const int pulsePin = 32; // Add pulse sensor pin

// Web server on port 80 (optional - for local debugging)
WebServer server(80);

// Timing variables
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 2000; // Send data every 2 seconds

// Read LM35 Temperature
float readTemperature() {
  int raw = analogRead(tempPin);
  float voltage = (raw / 4095.0) * 3.3;
  return voltage * 100.0; // Convert to Celsius
}

// Read Pulse (simplified - you might need to adjust based on your actual pulse sensor)
int readPulse() {
  // This is a simplified pulse reading - adjust based on your sensor
  int pulseValue = analogRead(pulsePin);
  // Convert analog reading to BPM (you'll need to calibrate this)
  int bpm = map(pulseValue, 0, 4095, 60, 120); // Map to typical heart rate range
  return bpm;
}

// Read ECG
int readECG() {
  return analogRead(ecgPin);
}

// Send data to Flask server
void sendDataToFlask() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(flaskServerURL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Read sensor values
    float temp = readTemperature();
    int pulse = readPulse();
    int ecg = readECG();
    
    // Create POST data
    String postData = "temp=" + String(temp, 2) + 
                     "&pulse=" + String(pulse) + 
                     "&ecg=" + String(ecg);
    
    // Send POST request
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Data sent successfully: " + response);
      Serial.println("Temp: " + String(temp) + "°C, Pulse: " + String(pulse) + " BPM, ECG: " + String(ecg));
    } else {
      Serial.println("Error sending data: " + String(httpResponseCode));
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

// Optional: Local web server for debugging
void handleRoot() {
  float temp = readTemperature();
  int pulse = readPulse();
  int ecg = readECG();
  
  String html = "<html><body>";
  html += "<h2>ESP32 Sensor Readings</h2>";
  html += "<p>Temperature: " + String(temp, 2) + " °C</p>";
  html += "<p>Pulse: " + String(pulse) + " BPM</p>";
  html += "<p>ECG: " + String(ecg) + "</p>";
  html += "<p>Sending to Flask server: " + String(flaskServerURL) + "</p>";
  html += "<script>setTimeout(function(){location.reload();}, 3000);</script>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleSensorData() {
  float temp = readTemperature();
  int pulse = readPulse();
  int ecg = readECG();
  
  String json = "{\"temp\":" + String(temp, 2) +
                ",\"pulse\":" + String(pulse) +
                ",\"ecg\":" + String(ecg) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  
  // Initialize sensor pins
  pinMode(tempPin, INPUT);
  pinMode(ecgPin, INPUT);
  pinMode(pulsePin, INPUT);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Sending data to Flask server: ");
  Serial.println(flaskServerURL);
  
  // Optional: Start local web server for debugging
  server.on("/", handleRoot);
  server.on("/sensor-data", handleSensorData);
  server.begin();
  Serial.println("Local web server started (optional for debugging)");
  
  Serial.println("Setup complete. Starting sensor readings...");
}

void loop() {
  // Handle local web server requests (optional)
  server.handleClient();
  
  // Send sensor data to Flask server at regular intervals
  unsigned long currentTime = millis();
  if (currentTime - lastSensorRead >= sensorInterval) {
    sendDataToFlask();
    lastSensorRead = currentTime;
  }
  
  delay(100); // Small delay to prevent excessive CPU usage
}