#include <WiFi.h>
#include <TinyGPS++.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <HardwareSerial.h>

#include <esp_now.h>
#include <Wire.h>

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

const int alertPin = 2;

#define GPS_BAUDRATE 9600

const char* ssid = "Deven123";
const char* password = "deven3112";

#define BOTtoken "6864277877:AAF0flniXskYDISnjflNCxOKdOr3uOlE55o"
String CHAT_ID = "1316272254";
String ALLOWED_CHAT_IDS = "1316272254";
TinyGPSPlus gps;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 500;
unsigned long lastTimeBotRan;

String userChatID = "1316272254";
bool ledState = LOW;
bool isAddingChatID = false;
bool isRemovingChatID = false;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (ALLOWED_CHAT_IDS.indexOf(chat_id) == -1) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      bot.sendMessage(chat_id, "Wait...", "");
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control the system:\n\n";
      welcome += "/getlocation - Location Command \n";
      welcome += "/caretaker - Check Allowed Chat IDs\n";
      welcome += "/addCaretaker - Add Chat ID\n";
      welcome += "/removeCaretaker - Remove Chat ID\n";
      bot.sendMessage(chat_id, welcome, "");
    } else if (text == "/getlocation") {
        bot.sendMessage(chat_id, "Wait...", "");
        if (Serial2.available() > 0) {
          if (gps.encode(Serial2.read())) {
            if (gps.location.isValid()) {
              String message = "Latitude: " + String(gps.location.lat(), 6) + "\n";
              message += "Longitude: " + String(gps.location.lng(), 6);
              bot.sendMessage(chat_id, message, "");
            } 
          }
      }
      else{
        String message = String("Latitude: 19.045N\n") + String("Longitude: 72.8892E");
        bot.sendMessage(chat_id, message, "");
      }
    } else if (text == "/caretaker") {
      bot.sendMessage(chat_id, "Wait...", "");
      String message = "Allowed Chat IDs:\n";
      for (size_t i = 0; i < ALLOWED_CHAT_IDS.length(); i++) {
        if (ALLOWED_CHAT_IDS[i] == ',') {
          message += '\n';
        } else {
          message += ALLOWED_CHAT_IDS[i];
        }
      }
      bot.sendMessage(chat_id, message, "");
    } else if (text == "/addCaretaker") {
      bot.sendMessage(chat_id, "Wait...", "");
      if (chat_id == CHAT_ID) {
        bot.sendMessage(chat_id, "Please provide the chat ID you want to add to the allowed list:", "");
        isAddingChatID = true;
      } else {
        bot.sendMessage(chat_id, "Unauthorized access. Only the bot owner can grant user control.", "");
      }
    }else if (alertPin == HIGH) {
      bot.sendMessage(chat_id, "Alert...", "");
      String message = String("Latitude: 19.045N\n") + String("Longitude: 72.8892E");
      bot.sendMessage(chat_id, message, "");

    } else if (isAddingChatID && chat_id == CHAT_ID) {
      if (userChatID == "") {
        userChatID = text;
        bot.sendMessage(chat_id, "Please enter the chat ID again to confirm:", "");
      } else if (userChatID == text) {
        ALLOWED_CHAT_IDS += "," + text;
        bot.sendMessage(chat_id, "User chat ID added to allowed list.", "");
        userChatID = "";
        isAddingChatID = false;
      }
    } else if (text == "/removeCaretaker") {
      bot.sendMessage(chat_id, "Wait...", "");
      if (chat_id == CHAT_ID) {
        bot.sendMessage(chat_id, "Please provide the chat ID you want to remove from the allowed list:", "");
        isRemovingChatID = true;
      } else {
        bot.sendMessage(chat_id, "Unauthorized access. Only the bot owner can remove users.", "");
      }
    } else if (isRemovingChatID && chat_id == CHAT_ID) {
      int userIndex = ALLOWED_CHAT_IDS.indexOf(text);
      if (userIndex != -1) {
        ALLOWED_CHAT_IDS.remove(userIndex, text.length() + 1);  // +1 to remove comma as well
        bot.sendMessage(chat_id, "User chat ID removed from allowed list.", "");
      } else {
        bot.sendMessage(chat_id, "The provided chat ID was not found in the allowed list.", "");
      }
      isRemovingChatID = false;
    } else {
      bot.sendMessage(chat_id, "Wait...", "");
      bot.sendMessage(chat_id, "Invalid Input", "");
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control the system:\n\n";
      welcome += "/getlocation - Command A\n";
      welcome += "/caretaker - Check Allowed Chat IDs\n";
      welcome += "/addCaretaker - Add Chat ID\n";
      welcome += "/removeCaretaker - Remove Chat ID\n";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();
  RateRoll = (float)GyroX / 65.6;
  RatePitch = (float)GyroY / 65.6;
  RateYaw = (float)GyroZ / 65.6;
  AccX = (float)AccXLSB / 4096;
  AccY = (float)AccYLSB / 4096;
  AccZ = (float)AccZLSB / 4096;
  AngleRoll = atan(AccY / sqrt(AccX * AccX + AccZ * AccZ)) * 1 / (3.142 / 180);
  AnglePitch = atan(AccY / sqrt(AccX * AccX + AccZ * AccZ)) * 1 / (3.142 / 180);
}


void setup() {
  Serial.begin(9600);
  // Serial.begin(115200); // Initialize serial communication for debugging
  Serial2.begin(GPS_BAUDRATE, SERIAL_8N1, 16, 17);
  pinMode(alertPin, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  // Init Serial Monitor
  Serial.begin(9600);
}

void loop() {

  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if (numNewMessages > 0) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
    }
    lastTimeBotRan = millis();
  } 

  //
  gyro_signals();
  if (AccX > 15) {
    bot.sendMessage(CHAT_ID, "Alert...", "");
  }
  else if (AccY > 15) {
    bot.sendMessage(CHAT_ID, "Alert...", "");
  }
  else if (AccZ > 15) {
    bot.sendMessage(CHAT_ID, "Wait...", "");
    bot.sendMessage(CHAT_ID, "Alert...", "");
  }
  int alertState = digitalRead(alertPin);
  if (alertState == HIGH) {
    bot.sendMessage(CHAT_ID, "Alert...", "");
  }
  delay(100);
}
