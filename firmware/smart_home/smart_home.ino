#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
const int flamePin = 8;
const int gasPin = A0;
const int buzzerPin = 9;
const int irPin = 2;
const int ledPin = 11;
const int fanRelayPin = 10;
const int switchLight = 5;
const int switchFan = 6;
const int trigPin = 3;
const int echoPin = 4;

// Thresholds
int smokeThreshold = 450;      // calibrate for your MQ2
int distanceThreshold = 50;    // cm for HC-SR04

bool manualLight = false;
bool manualFan = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print("Smart Home Sys");
  lcd.setCursor(0,1); lcd.print("Initializing...");
  delay(1500); lcd.clear();

  pinMode(flamePin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(fanRelayPin, OUTPUT);
  pinMode(switchLight, INPUT_PULLUP);
  pinMode(switchFan, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  long distance = duration * 0.034 / 2;
  if(distance == 0 || distance > 400) distance = 999; // invalid => far
  return distance;
}

void loop() {
  int flameStatus = digitalRead(flamePin);    // LOW = flame (module dependent)
  int gasValue = analogRead(gasPin);          // MQ2
  int irStatus = digitalRead(irPin);          // HIGH or LOW depending on module
  long distance = readDistanceCM();

  // Debounce manual switches
  static unsigned long lastSwitchTime = 0;
  unsigned long now = millis();
  if(digitalRead(switchLight) == LOW && now - lastSwitchTime > 200) {
    manualLight = !manualLight; lastSwitchTime = now;
  }
  if(digitalRead(switchFan) == LOW && now - lastSwitchTime > 200) {
    manualFan = !manualFan; lastSwitchTime = now;
  }

  // Determine states: manual OR automatic (IR or proximity)
  bool lightState = manualLight;
  bool fanState = manualFan;
  if(irStatus == HIGH || distance < distanceThreshold) {
    lightState = true; fanState = true;
  }

  // Fire & smoke detection
  bool flameDetected = (flameStatus == LOW);              // adjust if needed
  bool smokeDetected = (gasValue > smokeThreshold);

  lcd.setCursor(0,0);
  if(flameDetected) {
    tone(buzzerPin, 1000);
    lcd.print("!! FLAME ALERT !!");
  } else if(smokeDetected) {
    tone(buzzerPin, 900);
    lcd.print("!! SMOKE ALERT !!");
  } else if(distance < distanceThreshold) {
    tone(buzzerPin, 1200);
    lcd.print("!! INTRUDER !!   ");
  } else {
    noTone(buzzerPin);
    lcd.print("Environment OK  ");
  }

  // Apply outputs
  digitalWrite(ledPin, lightState ? HIGH : LOW);
  digitalWrite(fanRelayPin, fanState ? HIGH : LOW);

  // LCD row 2 status
  lcd.setCursor(0,1);
  if(lightState && fanState) lcd.print("LIGHT+FAN ON   ");
  else if(lightState) lcd.print("LIGHT ON       ");
  else if(fanState) lcd.print("FAN ON         ");
  else lcd.print("ALL OFF        ");

  // Serial debug
  Serial.print("Flame:"); Serial.print(flameStatus);
  Serial.print(" Gas:"); Serial.print(gasValue);
  Serial.print(" IR:"); Serial.print(irStatus);
  Serial.print(" Dist:"); Serial.print(distance);
  Serial.print(" Light:"); Serial.print(lightState);
  Serial.print(" Fan:"); Serial.println(fanState);

  delay(300);
}
