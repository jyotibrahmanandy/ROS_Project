#include <HTTPClient.h>
#include <WiFi.h>

#define SOUND_PIN 34
#define LED_PIN 25
#define BUZZER_PIN 26

// WIFI
const char *ssid = "moto edge 60 pro_7079";
const char *password = "motojyoti";

// ✅ PORT CHANGED TO 5001 (to avoid conflict with macOS AirPlay)
const char *serverUrl = "http://10.96.137.149:5001/data";

int threshold = 50; // User likes this sensitivity

int currentHour = 10;
int currentMinute = 15;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("================================");
  Serial.println("System Booting...");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("================================");
}

void loop() {
  Serial.println("----------------------------------");
  Serial.print("Current Time: ");
  Serial.print(currentHour); Serial.print(":"); Serial.println(currentMinute);
  Serial.println("System Mode: ACTIVE (Class Time)");

  int soundValue = analogRead(SOUND_PIN);
  Serial.print("Sound Value: ");
  Serial.println(soundValue);

  if (soundValue > threshold) {
    Serial.println("ALERT: Noise Detected!");

    // Buzzer and Light Action
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);

    sendData(soundValue, "NOISE");

    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("Environment Quiet");
  }

  Serial.println("----------------------------------");
  delay(2000);
}

void sendData(int sound, String status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"sound\":" + String(sound) +
                  ",\"status\":\"" + status +
                  "\",\"time\":\"" + String(currentHour) + ":" + String(currentMinute) + "\"}";

    int response = http.POST(json);
    if (response > 0) {
      Serial.print("Server Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error sending POST: ");
      Serial.println(response);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected. Cannot send data.");
  }
}