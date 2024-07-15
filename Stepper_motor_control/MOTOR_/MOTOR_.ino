#include<ESP8266WiFi.h>
#include <EEPROM.h>
#include<espnow.h>
#define MY_ROLE         ESP_NOW_ROLE_COMBO              // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_COMBO              // set the role of the receiver
#define WIFI_CHANNEL    1
#define MY_NAME         "SENSOR NODE" // DC:4F:22:66:CC:BC
uint8_t receiverAddress[] = {0xDC, 0x4F, 0x22, 0x66, 0xCE, 0x32};     // MAC address of Remote Pcb ESP12E
struct __attribute__((packed)) dataPacket {
  int switch1;
  int switch2;
};

#include <Servo.h>
#define s1 12 // Servo motor pin
#define s2 13  // Servo motor pin
Servo servo1;  // Servo motor define
Servo servo2;  // Servo motor define
void setup() {
  servo1.attach(s1); // servo motor pin initialize
  servo2.attach(s2); //servo motor pin initialize

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("This is my address: ");
  Serial.println(WiFi.macAddress());
  Serial.print(MY_NAME);
  Serial.println("...initializing...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_set_self_role(MY_ROLE);
  esp_now_register_send_cb(transmissionComplete);         // this function will get called once all data is sent
  esp_now_register_recv_cb(dataReceived);               // this function will get called whenever we receive data
  esp_now_add_peer(receiverAddress, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);

  Serial.println("Initialized.");
}

void loop() {
  // put your main code here, to run repeatedly:

}

void transmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) {
  if (transmissionStatus == 0) {
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error code: ");
    Serial.println(transmissionStatus);
  }
}

void dataReceived(uint8_t *senderMac, uint8_t *data123, uint8_t dataLength) {
  char macStr[18];
  dataPacket packet;
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);
  Serial.println();
  Serial.print("Received data from: ");
  Serial.println(macStr);
  memcpy(&packet, data123, sizeof(packet));

  int data = packet.switch1;
  int data1 = packet.switch2;
//  EEPROM.update(0, data1);
  runMotor(data, data1);
}

void runMotor(int data, int data1) {
  data1 = EEPROM.read(0);
  if (data1 == 1) {    
    servo1.write(100);
    servo2.write(100);
  }

  if (data1 == 2) {
    servo1.write(100);
    servo2.write(0);
  }
  if (data1 == 3) {
    
    servo2.write(0);
    servo1.write(0);
  }
}
