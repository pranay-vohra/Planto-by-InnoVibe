#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
#define SOIL_PIN 34      // Soil moisture sensor
#define DHT_PIN 27       // DHT22 temperature/humidity sensor
#define LDR_PIN 35       // Light sensor
#define MQ135_PIN 32     // Air quality sensor
#define RELAY_PIN 25     // Water pump relay control

// DHT setup
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// WiFi and Server Settings
const char* ssid = "Pranay";
const char* password = "jcuq2765";
const char* serverUrl = "http://192.168.48.138:5050/predict";  // Your server address

// Plant state mapping  
const char* emoticon_map[] = {
  ":)", // Happy
  ":|", // Needs Water
  ":(" // Warning/Unhealthy
};

const char* message_map[] = {
  "I'm happy!",
  "I need water!",
  "I'm not well!"
};

// Function declarations
void displayResults(int soil, int light, int air, float temp, float humid, int state);
void displayError(String message);
void displayNetworkStatus(String status);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Smart Plant System Starting...");
  
  // Initialize sensors
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  
  // Show boot message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Smart Plant System"));
  display.println(F("Initializing..."));
  display.display();
  
  // Connect to WiFi
  displayNetworkStatus("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected to WiFi, IP: ");
    Serial.println(WiFi.localIP());
    displayNetworkStatus("WiFi Connected!");
  } else {
    Serial.println("Failed to connect to WiFi");
    displayNetworkStatus("WiFi Failed!");
    delay(3000);
  }
  
  delay(2000);
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost, reconnecting...");
    displayNetworkStatus("WiFi Reconnecting...");
    WiFi.reconnect();
    delay(5000);
    return;
  }
  
  // Read all sensors
  int soilValue = analogRead(SOIL_PIN);   // Higher value = drier soil
  int lightValue = analogRead(LDR_PIN);   // Higher value = more light
  int airQuality = analogRead(MQ135_PIN); // Higher value = worse air quality
  float temp = dht.readTemperature();     // Temperature in Celsius
  float humid = dht.readHumidity();       // Relative humidity

  // Check for valid readings
  if (isnan(temp) || isnan(humid)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    temp = 25.0;  // Default values if sensor fails
    humid = 50.0;
  }
  
  // Debug output
  Serial.println(F("Current Sensor Readings:"));
  Serial.printf("Soil: %d | Light: %d | Air: %d | Temp: %.1f°C | Humid: %.1f%%\n", 
                soilValue, lightValue, airQuality, temp, humid);
  
  // Create JSON payload for server
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["soil_moisture"] = soilValue;
  jsonDoc["light"] = lightValue;
  jsonDoc["air_quality"] = airQuality;
  jsonDoc["temperature"] = temp;
  jsonDoc["humidity"] = humid;
  
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);
  
  // Send data to server and get prediction
  int prediction = sendDataToServer(jsonPayload);

// Only update display and take actions if server responded correctly
 if (prediction == -1) {
   Serial.println("Server unreachable, skipping display and action.");
   return; // <<<< Do nothing else in this loop
} 

displayResults(soilValue, lightValue, airQuality, temp, humid, prediction);

// Take action based on prediction
if (prediction == 1) {
  Serial.println(F("Plant needs water - Activating pump"));
  digitalWrite(RELAY_PIN, LOW);
  delay(3000);
  digitalWrite(RELAY_PIN, HIGH);
}
  // Display results on OLED
  displayResults(soilValue, lightValue, airQuality, temp, humid, prediction);
  
  // Take action based on prediction
  if (prediction == 1) { // Needs water
  Serial.println(F("Plant needs water - Activating pump"));
  digitalWrite(RELAY_PIN, LOW);  // Relay ON if active LOW
  delay(3000);                   // Run pump for 3 seconds
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF if active LOW
}
  
  // Wait before next reading
  delay(5000);
}

int sendDataToServer(String jsonPayload) {
  HTTPClient http;
  int plantState = 0; // Default: happy
  
  Serial.println("Sending data to server...");
  Serial.println(jsonPayload);
  
  // Begin HTTP connection
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  // Send POST request
  int httpResponseCode = http.POST(jsonPayload);
  
  // Check response
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
    
    // Parse JSON response
    StaticJsonDocument<200> responseDoc;
    DeserializationError error = deserializeJson(responseDoc, response);
    
    if (!error) {
      plantState = responseDoc["prediction"];
      Serial.printf("Prediction from server: %d (%s)\n", plantState, message_map[plantState]);
    } else {
      Serial.println("Failed to parse response");
      displayError("JSON parse error");
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    displayError("Server error: " + String(httpResponseCode));
    
    // Fall back to local simple rule-based prediction if server fails
    int soilValue = analogRead(SOIL_PIN);
    if (soilValue > 2500) { // Threshold for dry soil
      plantState = 1; // Needs water
      Serial.println("Falling back to local rule: Needs water");
    }
  }
  
  http.end();
  return plantState;
}

// Function to display sensor readings and plant state
void displayResults(int soil, int light, int air, float temp, float humid, int state) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Soil: ")); display.println(soil);
  display.print(F("Light: ")); display.println(light);
  display.print(F("Air: ")); display.println(air);
  
  display.setCursor(64, 0);
  display.print(F("T: ")); display.print(temp, 1); display.println(F("C"));
  display.setCursor(64, 8);
  display.print(F("H: ")); display.print(humid, 1); display.println(F("%"));
  
  // Show plant emotion (larger text)
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println(emoticon_map[state]);
  
  // Show message
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(message_map[state]);
  
  display.display();
}

// Function to display error messages
void displayError(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("ERROR:"));
  display.println(message);
  display.display();
}

// Function to display network status
void displayNetworkStatus(String status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Network Status:"));
  display.println(status);
  display.display();
}