#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int flamePin = 8;
const int gasPin = A0;
const int irPin = 2;
const int trigPin = 3;
const int echoPin = 4;
const int buzzerPin = 9;
const int fanRelayPin = 10;
const int ledPin = 11;  // New LED (room light)

// Threshold values
const int smokeThreshold = 130;     // Adjust based on MQ2
const int distanceThreshold = 50;   // cm for intruder alert

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Smart Solar Home ");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing... ");
  
  pinMode(flamePin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(fanRelayPin, LOW);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  delay(2000);
  lcd.clear();
}

void loop() {
  // --- Sensor readings ---
  int flameStatus = digitalRead(flamePin);
  int irStatus = digitalRead(irPin);
  int gasValue = analogRead(gasPin);

  // --- Measure Distance (ultrasonic) ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  // --- Debug info ---
  Serial.print("Flame: "); Serial.print(flameStatus == LOW ? "YES" : "NO");
  Serial.print(" | Gas: "); Serial.print(gasValue);
  Serial.print(" | IR: "); Serial.print(irStatus == HIGH ? "MOTION" : "NONE");
  Serial.print(" | Distance: "); Serial.print(distance);
  Serial.println(" cm");

  // --- Determine conditions ---
  bool alert = false;
  bool motionDetected = (irStatus == HIGH);
  bool fanOn = false;
  String statusLine = "";

  if (flameStatus == LOW) {
    statusLine = "Fire ALERT!";
    tone(buzzerPin, 1000);
    alert = true;
  } 
  else if (gasValue > smokeThreshold + 100) {
    statusLine = "Gas Leak!";
    tone(buzzerPin, 900);
    alert = true;
  } 
  else if (gasValue > smokeThreshold) {
    statusLine = " Smoke Detected";
    tone(buzzerPin, 800);
    alert = true;
  } 
  else if (distance < distanceThreshold && distance > 0) {
    statusLine = "Intruder Alert!";
    tone(buzzerPin, 1200);
    alert = true;
  } 
  else if (motionDetected) {
    statusLine = "All Safe ";
    noTone(buzzerPin);
  } 
  else {
    statusLine = "All Safe        ";
    noTone(buzzerPin);
  }

  // --- Fan and LED Control ---
  if (motionDetected || alert) {
    digitalWrite(fanRelayPin, HIGH);  // Fan ON
    digitalWrite(ledPin, HIGH);       // Light ON
    fanOn = true;
  } else {
    digitalWrite(fanRelayPin, LOW);   // Fan OFF
    digitalWrite(ledPin, LOW);        // Light OFF
    fanOn = false;
  }

  // --- LCD display ---
  lcd.setCursor(0, 0);
  lcd.print("F:");
  lcd.print(flameStatus == LOW ? "Y" : "N");
  lcd.print(" G:");
  lcd.print(gasValue);
  lcd.print(" D:");
  lcd.print(distance);
  lcd.print("  ");

  lcd.setCursor(0, 1);
  lcd.print(statusLine);
  int remaining = 16 - statusLine.length();
  for (int i = 0; i < remaining; i++) lcd.print(" ");

  // Fan indicator
  lcd.setCursor(14, 0);


  delay(700);
}
