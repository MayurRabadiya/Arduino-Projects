#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd1(0x27, 16, 2);

boolean stop1 = false;
boolean stop1_n = false;
boolean stop2 = false;
boolean stop2_n = false;

const uint8_t RelayPins[] = {17, 12, 3, 5, 16};
const uint8_t MotorPins[] = {23, 4, 19, 18};
const uint8_t SwitchPins[] = {12, 13, 14, 15, 25};
const uint8_t motorControlPins[] = {26, 27, 32, 33};
const uint8_t wifiLed = 11;

const uint8_t VPIN_BUTTONS[] = {V1, V2, V3, V4, V5};
const uint8_t MOTOR_BUTTONS[] = {V6, V7, V8, V9};

int toggleStates[] = {0, 0, 0, 0, 0};
int motorToggleStates[] = {0, 0, 0, 0};

int wifiFlag = 0;

#define AUTH "jlCzj0r7QU92EIU93sdKk_tEsqodzjiq" // You should get Auth Token in the Blynk App.
#define WIFI_SSID "SSID" // Enter Wifi Name
#define WIFI_PASS "PASS" // Enter wifi Password

BlynkTimer timer;

void relayOnOff(uint8_t relay) {
  if (toggleStates[relay] == 1) {
    digitalWrite(RelayPins[relay], LOW);
    toggleStates[relay] = 0;
    Serial.printf("Device%d ON\n", relay + 1);
  } else {
    digitalWrite(RelayPins[relay], HIGH);
    toggleStates[relay] = 1;
    Serial.printf("Device%d OFF\n", relay + 1);
  }
  delay(200);
}

void motorControl(uint8_t motor, uint8_t direction) {
  uint8_t motorIndex = motor * 2 + direction;
  if (motorToggleStates[motorIndex] == 0) {
    digitalWrite(MotorPins[motorIndex], HIGH);
    Serial.printf("Motor_%d %s ON\n", motor + 1, direction == 0 ? "D" : "R");
  } else {
    digitalWrite(MotorPins[motorIndex], LOW);
    Serial.printf("Motor_%d %s OFF\n", motor + 1, direction == 0 ? "D" : "R");
  }
  delay(200);
}

void withInternetControl() {
  for (uint8_t i = 0; i < 5; i++) {
    if (digitalRead(SwitchPins[i]) == LOW) {
      delay(100);
      relayOnOff(i);
      Blynk.virtualWrite(VPIN_BUTTONS[i], toggleStates[i]);
    }
  }

  for (uint8_t i = 0; i < 2; i++) {
    if (digitalRead(motorControlPins[i * 2]) == LOW) {
      delay(100);
      motorControl(i, 0);
    }
    if (digitalRead(motorControlPins[i * 2 + 1]) == LOW) {
      delay(100);
      motorControl(i, 1);
    }
  }
}

bool stopLoop(int motorIndex, int direction) {
  int pin = motorControlPins[motorIndex * 2 + direction];
  if (digitalRead(pin) == HIGH) {
    delay(100);
    motorControl(motorIndex, direction);
  }
  return false;
}

void withoutInternetControl() {
  for (uint8_t i = 0; i < 5; i++) {
    if (digitalRead(SwitchPins[i]) == LOW) {
      delay(200);
      relayOnOff(i);
    }
  }

  for (uint8_t i = 0; i < 2; i++) {
    if (digitalRead(motorControlPins[i * 2]) == LOW) {
      delay(200);
      motorControl(i, 0);
    }
    if (digitalRead(motorControlPins[i * 2 + 1]) == LOW) {
      delay(200);
      motorControl(i, 1);
    }
  }
}

BLYNK_CONNECTED() {
  for (uint8_t i = 0; i < 5; i++) {
    Blynk.syncVirtual(VPIN_BUTTONS[i]);
  }
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleStates[0] = param.asInt();
  digitalWrite(RelayPins[0], toggleStates[0]);
  Serial.println("Button_1");
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleStates[1] = param.asInt();
  digitalWrite(RelayPins[1], toggleStates[1]);
  Serial.println("Button_2");
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleStates[2] = param.asInt();
  digitalWrite(RelayPins[2], toggleStates[2]);
  Serial.println("Button_3");
}

BLYNK_WRITE(VPIN_BUTTON_4) {
  toggleStates[3] = param.asInt();
  digitalWrite(RelayPins[3], toggleStates[3]);
  Serial.println("Button_4");
}

BLYNK_WRITE(VPIN_BUTTON_5) {
  toggleStates[4] = param.asInt();
  digitalWrite(RelayPins[4], toggleStates[4]);
  Serial.println("Button_5");
}

BLYNK_WRITE(motor1_p__BUTTON) {
  int value = param.asInt();
  pinMode(motorControlPins[0], value == 1 ? INPUT : INPUT_PULLUP);
  digitalWrite(MotorPins[0], value);
  Serial.println(value == 1 ? "motor_1 ON_D" : "motor_1 OFF_D");
}

BLYNK_WRITE(motor1_n__BUTTON) {
  int value = param.asInt();
  pinMode(motorControlPins[1], value == 1 ? INPUT : INPUT_PULLUP);
  digitalWrite(MotorPins[1], value);
  Serial.println(value == 1 ? "motor_1 ON_R" : "motor_1 OFF_R");
}

BLYNK_WRITE(motor2_p__BUTTON) {
  int value = param.asInt();
  pinMode(motorControlPins[2], value == 1 ? INPUT : INPUT_PULLUP);
  digitalWrite(MotorPins[2], value);
  Serial.println(value == 1 ? "motor_2 ON_D" : "motor_2 OFF_D");
}

BLYNK_WRITE(motor2_n__BUTTON) {
  int value = param.asInt();
  pinMode(motorControlPins[3], value == 1 ? INPUT : INPUT_PULLUP);
  digitalWrite(MotorPins[3], value);
  Serial.println(value == 1 ? "motor_2 ON_R" : "motor_2 OFF_R");
}

void checkBlynkStatus() {
  bool isConnected = Blynk.connected();
  digitalWrite(wifiLed, isConnected ? LOW : HIGH);
  wifiFlag = isConnected ? 0 : 1;
}

void setup() {
  Serial.begin(115200);

  lcd1.begin();
  lcd1.backlight();
  lcd1.setCursor(2, 0);
  lcd1.print("PROJECT_");
  lcd1.setCursor(2, 1);
  lcd1.print("PROJECT_");

  for (uint8_t i = 0; i < 5; i++) {
    pinMode(RelayPins[i], OUTPUT);
    pinMode(SwitchPins[i], INPUT_PULLUP);
    digitalWrite(RelayPins[i], toggleStates[i]);
  }

  for (uint8_t i = 0; i < 4; i++) {
    pinMode(MotorPins[i], OUTPUT);
    pinMode(motorControlPins[i], INPUT_PULLUP);
    digitalWrite(MotorPins[i], motorToggleStates[i]);
  }

  pinMode(wifiLed, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  timer.setInterval(3000L, checkBlynkStatus);
  Blynk.config(AUTH);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Not Connected");
  } else {
    Blynk.run();
  }

  stop1 = stopLoop(0, 0);
  stop1_n = stopLoop(0, 1);
  stop2 = stopLoop(1, 0);
  stop2_n = stopLoop(1, 1);

  timer.run();
  if (wifiFlag == 0) {
    withInternetControl();
  } else {
    withoutInternetControl();
  }
}
