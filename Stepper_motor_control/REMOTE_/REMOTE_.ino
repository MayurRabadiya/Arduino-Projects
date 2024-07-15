#include<ESP8266WiFi.h>
#include<espnow.h>
#define MY_ROLE         ESP_NOW_ROLE_COMBO              // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_COMBO              // set the role of the receiver
#define WIFI_CHANNEL    1
#define MY_NAME         "PC NODE" //DC:4F:22:66:C8:92
uint8_t receiverAddress[] = {0xDC, 0x4F, 0x22, 0x66, 0xC8, 0xB4};  //  MAC address of CONSOLE Pcb ESP12E
struct __attribute__((packed)) dataPacket {
  int switch1;
  int switch2;
};

#define switch_pin 0
#define switch_pin2 12
#define switch_pin3 13
int a = 0;
int switchStatusLast = LOW;  // last status switch
boolean stop1 = true;
boolean stop2 = true;
boolean stop3 = true;


void setup() {
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

  pinMode(switch_pin, INPUT);
  pinMode(switch_pin2, INPUT);
  pinMode(switch_pin3, INPUT);

}

void loop() {
  button1();
  button2();
  button3();
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
  Serial.print("data[0] : ");
  Serial.println(packet.switch1);
  Serial.print("data[1] : ");
  Serial.println(packet.switch2);
}

void button1() {
  if (digitalRead(switch_pin) == HIGH) {
    if (stop1 == false) {
      delay(300);
      dataPacket packet;
      packet.switch1 = 1;
      packet.switch2 = 1;
      esp_now_send(receiverAddress, (uint8_t *) &packet, sizeof(packet));
      stop1 = true;
    }
  }
  else {
    stop1 = false;
  }
}


void button2() {
  if (digitalRead(switch_pin2) == HIGH) {
    if (stop2 == false) {
      delay(300);
      dataPacket packet;
      packet.switch1 = 2;
      packet.switch2 = 2;
      esp_now_send(receiverAddress, (uint8_t *) &packet, sizeof(packet));
      stop2 = true;
    }
  }
  else {
    stop2 = false;
  }
}


void button3() {
  if (digitalRead(switch_pin3) == HIGH) {
    if (stop3 == false) {
      delay(300);
      dataPacket packet;
      packet.switch1 = 3;
      packet.switch2 = 3;
      esp_now_send(receiverAddress, (uint8_t *) &packet, sizeof(packet));
      stop3 = true;
    }
  }
  else {
    stop3 = false;
  }
}
