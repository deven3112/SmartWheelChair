//receiver flex
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure

//Motor A
const int RightMotorForward = 26;    // IN1
const int RightMotorBackward = 27;   // IN2
//Motor B
const int LeftMotorForward = 14;     // IN3
const int LeftMotorBackward = 12;    // IN4
 
typedef struct struct_message {
    int a;
    int b;
    int c;
    int d;
    
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
 
 
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("flex1: ");
  Serial.println(myData.a);
  Serial.print("flex2: ");
  Serial.println(myData.b);
  Serial.print("flex3: ");
  Serial.println(myData.c);
  Serial.print("flex4: ");
  Serial.println(myData.d);

  if(myData.a > 200){
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(RightMotorBackward, LOW);
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    Serial.println("FORWARD");
    }

    if(myData.b > 10){
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, HIGH);
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, HIGH);
    Serial.println("BACKWARD");
    }
    
    if( myData.c > 250 ){
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, HIGH);
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    Serial.println("LEFT");
    }

    if( myData.d > 200 ){
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(RightMotorBackward, LOW);
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, HIGH);
    Serial.println("RIGHT");
    }
    if( myData.d < 200 && myData.d > 200 && myData.b < 10 && myData.d > 200  ){
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(RightMotorBackward, LOW);
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(LeftMotorBackward, LOW);
    Serial.println("RIGHT");
    }

    
}

 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
 // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
}
 
void loop() {
}
