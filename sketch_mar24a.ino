#define BLYNK_TEMPLATE_ID "TMPL6EL5kNzyR" // Replace with Template ID from Blynk
#define BLYNK_TEMPLATE_NAME "nhietdo" // Replace with Template Name
#define BLYNK_AUTH_TOKEN "HvBb5NCrcYv_P7N3m3vlW5cRZkDZRiCL" // Replace with Auth Token from Blynk

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi information
char ssid[] = "Batmangemdi";     // Replace with your WiFi name
char pass[] = "longvu10";        // Replace with your WiFi password

// DHT11 configuration
#define DHTPIN 4          // Pin connected to DHT11
#define DHTTYPE DHT11     // Sensor type
DHT dht(DHTPIN, DHTTYPE);

// LCD I2C configuration
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, change if necessary (0x27 or 0x3F)

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("WiFi & Blynk...");
  
  // Initialize DHT
  dht.begin();
  
  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Display successful connection status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected!");
  delay(2000);
  lcd.clear();
}

// Function to send data to Blynk (no need to handle in BLYNK_WRITE since we are only sending data)
void sendDataToBlynk(float temperature, float humidity) {
  Blynk.virtualWrite(V0, temperature); // Send temperature to Virtual Pin V0
  Blynk.virtualWrite(V1, humidity);    // Send humidity to Virtual Pin V1
}

void loop() {
  Blynk.run(); // Run Blynk to maintain connection
  
  // Read data from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check if reading fails
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }
  
  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(humidity);
  lcd.print(" %");
  
  // Send data to Blynk
  sendDataToBlynk(temperature, humidity);
  
  // Print data to Serial for debugging
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  delay(2000); // Update every 2 seconds
}