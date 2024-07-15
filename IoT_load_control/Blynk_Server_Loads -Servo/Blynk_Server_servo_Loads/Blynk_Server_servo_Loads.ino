#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// LCD configuration
LiquidCrystal_I2C lcd1(0x27, 16, 2);

// Pin definitions
#define RELAY_PIN_1 1
#define RELAY_PIN_2 2
#define RELAY_PIN_3 3
#define RELAY_PIN_4 5
#define RELAY_PIN_5 16

#define SERVO_PIN_1 22
#define SERVO_PIN_2 23
#define SERVO_PIN_3 21

#define SWITCH_PIN_1 12
#define SWITCH_PIN_2 13
#define SWITCH_PIN_3 14
#define SWITCH_PIN_4 15
#define SWITCH_PIN_5 27

#define SERVO1_SWITCH_PIN 26
#define SERVO2_SWITCH_PIN 4
#define SERVO3_SWITCH_PIN 33

#define WIFI_LED_PIN 11

// Virtual pin definitions
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4
#define VPIN_BUTTON_5 V5
#define SERVO1_SLIDER V8
#define SERVO2_SLIDER V9
#define SERVO3_SLIDER V10

// Initial states
int relayState[5] = {0, 0, 0, 0, 0};
int servoState[3] = {0, 0, 0};

// Servo objects
Servo servo1;
Servo servo2;
Servo servo3;

// WiFi and Blynk configuration
#define AUTH_TOKEN "jlCzj0r7QU92EIU93sdKk_tEsqodzjiq"
#define WIFI_SSID "SSID"
#define WIFI_PASS "PASS"

BlynkTimer timer;
bool wifiFlag = false;

void toggleRelay(int relayIndex) {
  int pin;
  switch (relayIndex) {
    case 0: pin = RELAY_PIN_1; break;
    case 1: pin = RELAY_PIN_2; break;
    case 2: pin = RELAY_PIN_3; break;
    case 3: pin = RELAY_PIN_4; break;
    case 4: pin = RELAY_PIN_5; break;
    default: return;
  }

  relayState[relayIndex] = !relayState[relayIndex];
  digitalWrite(pin, relayState[relayIndex] ? LOW : HIGH);
  Serial.printf("Device %d %s\n", relayIndex + 1, relayState[relayIndex] ? "ON" : "OFF");
}

void toggleServo(int servoIndex) {
  Servo* servo;
  int pin;

  switch (servoIndex) {
    case 0: servo = &servo1; pin = SERVO_PIN_1; break;
    case 1: servo = &servo2; pin = SERVO_PIN_2; break;
    case 2: servo = &servo3; pin = SERVO_PIN_3; break;
    default: return;
  }

  servoState[servoIndex] = (servoState[servoIndex] == 180) ? 0 : 180;
  servo->write(servoState[servoIndex]);
  Serial.printf("Servo %d %s\n", servoIndex + 1, servoState[servoIndex] == 180 ? "ON" : "OFF");
}

void handleInternetCommands() {
  if (digitalRead(SWITCH_PIN_1) == LOW) { delay(100); toggleRelay(0); Blynk.virtualWrite(VPIN_BUTTON_1, relayState[0]); }
  else if (digitalRead(SWITCH_PIN_2) == LOW) { delay(100); toggleRelay(1); Blynk.virtualWrite(VPIN_BUTTON_2, relayState[1]); }
  else if (digitalRead(SWITCH_PIN_3) == LOW) { delay(100); toggleRelay(2); Blynk.virtualWrite(VPIN_BUTTON_3, relayState[2]); }
  else if (digitalRead(SWITCH_PIN_4) == LOW) { delay(100); toggleRelay(3); Blynk.virtualWrite(VPIN_BUTTON_4, relayState[3]); }
  else if (digitalRead(SWITCH_PIN_5) == LOW) { delay(100); toggleRelay(4); Blynk.virtualWrite(VPIN_BUTTON_5, relayState[4]); }
  else if (digitalRead(SERVO1_SWITCH_PIN) == LOW) { delay(100); toggleServo(0); Blynk.virtualWrite(SERVO1_SLIDER, servoState[0]); }
  else if (digitalRead(SERVO2_SWITCH_PIN) == LOW) { delay(100); toggleServo(1); Blynk.virtualWrite(SERVO2_SLIDER, servoState[1]); }
  else if (digitalRead(SERVO3_SWITCH_PIN) == LOW) { delay(100); toggleServo(2); Blynk.virtualWrite(SERVO3_SLIDER, servoState[2]); }
}

void handleOfflineCommands() {
  if (digitalRead(SWITCH_PIN_1) == LOW) { delay(200); toggleRelay(0); }
  else if (digitalRead(SWITCH_PIN_2) == LOW) { delay(200); toggleRelay(1); }
  else if (digitalRead(SWITCH_PIN_3) == LOW) { delay(200); toggleRelay(2); }
  else if (digitalRead(SWITCH_PIN_4) == LOW) { delay(200); toggleRelay(3); }
  else if (digitalRead(SWITCH_PIN_5) == LOW) { delay(200); toggleRelay(4); }
  else if (digitalRead(SERVO1_SWITCH_PIN) == LOW) { delay(200); toggleServo(0); }
  else if (digitalRead(SERVO2_SWITCH_PIN) == LOW) { delay(200); toggleServo(1); }
  else if (digitalRead(SERVO3_SWITCH_PIN) == LOW) { delay(200); toggleServo(2); }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1, VPIN_BUTTON_2, VPIN_BUTTON_3, VPIN_BUTTON_4, VPIN_BUTTON_5, SERVO1_SLIDER, SERVO2_SLIDER, SERVO3_SLIDER);
}

BLYNK_WRITE(VPIN_BUTTON_1) { relayState[0] = param.asInt(); digitalWrite(RELAY_PIN_1, relayState[0]); Serial.println("Button 1"); }
BLYNK_WRITE(VPIN_BUTTON_2) { relayState[1] = param.asInt(); digitalWrite(RELAY_PIN_2, relayState[1]); Serial.println("Button 2"); }
BLYNK_WRITE(VPIN_BUTTON_3) { relayState[2] = param.asInt(); digitalWrite(RELAY_PIN_3, relayState[2]); Serial.println("Button 3"); }
BLYNK_WRITE(VPIN_BUTTON_4) { relayState[3] = param.asInt(); digitalWrite(RELAY_PIN_4, relayState[3]); Serial.println("Button 4"); }
BLYNK_WRITE(VPIN_BUTTON_5) { relayState[4] = param.asInt(); digitalWrite(RELAY_PIN_5, relayState[4]); Serial.println("Button 5"); }
BLYNK_WRITE(SERVO1_SLIDER) { servo1.write(param.asInt()); Serial.println("Servo 1"); }
BLYNK_WRITE(SERVO2_SLIDER) { servo2.write(param.asInt()); Serial.println("Servo 2"); }
BLYNK_WRITE(SERVO3_SLIDER) { servo3.write(param.asInt()); Serial.println("Servo 3"); }

void checkBlynkStatus() {
  bool isConnected = Blynk.connected();
  if (!isConnected) {
    wifiFlag = true;
    digitalWrite(WIFI_LED_PIN, HIGH);
  } else {
    wifiFlag = false;
    digitalWrite(WIFI_LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  // Attach servos to respective pins
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo3.attach(SERVO_PIN_3);

  // Set relay pins as outputs
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(RELAY_PIN_5, OUTPUT);

  // Set WiFi LED pin as output
  pinMode(WIFI_LED_PIN, OUTPUT);

  // Set switch pins as inputs with pullup resistors
  pinMode(SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(SWITCH_PIN_2, INPUT_PULLUP);
  pinMode(SWITCH_PIN_3, INPUT_PULLUP);
  pinMode(SWITCH_PIN_4, INPUT_PULLUP);
  pinMode(SWITCH_PIN_5, INPUT_PULLUP);

  // Set servo switch pins as inputs with pullup resistors
  pinMode(SERVO1_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SERVO2_SWITCH_PIN, INPUT_PULLUP);
  pinMode(SERVO3_SWITCH_PIN, INPUT_PULLUP);

  // Ensure all relays are initially off
  digitalWrite(RELAY_PIN_1, relayState[0]);
  digitalWrite(RELAY_PIN_2, relayState[1]);
  digitalWrite(RELAY_PIN_3, relayState[2]);
  digitalWrite(RELAY_PIN_4, relayState[3]);
  digitalWrite(RELAY_PIN_5, relayState[4]);

  // Initialize WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Set timer to check Blynk status every 3 seconds
  timer.setInterval(3000L, checkBlynkStatus);

  // Configure Blynk
  Blynk.config(AUTH_TOKEN);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Not Connected");
  } else {
    Blynk.run();
  }

  timer.run();

  if (!wifiFlag) {
    handleInternetCommands();
  } else {
    handleOfflineCommands();
  }
}
