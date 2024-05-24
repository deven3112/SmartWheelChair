//transmitter flex
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x7B, 0x35, 0x88};//E4:65:B8:7B:35:88


int flexPin2 = 5;
int flexPin3 = 4;
int flexPin4 = 32;
int flexPin5 = 33;

//define variable values
int flexVal2;
int flexVal3;
int flexVal4;
int flexVal5;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
  int b;
  int c;
  int d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  flexVal2 = analogRead(flexPin2);
  flexVal3 = analogRead(flexPin3);
  flexVal4 = analogRead(flexPin4);
  flexVal5 = analogRead(flexPin5);
  myData.a = flexVal2;
  myData.b = flexVal3;
  myData.c = flexVal4;
  myData.d = flexVal5;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
