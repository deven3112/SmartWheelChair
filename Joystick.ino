#include <WiFi.h>

#define Rled 32

int enA = 13;
int LM_IN1 = 12;
int LM_IN2 = 14;

int enB = 25;
int RM_IN3 = 26;
int RM_IN4 = 27;

int joyX = 34, joyY = 35;
bool isConnected = false;




typedef struct struct_message {
  int a;
  int b;
} struct_message;


struct_message myData;

void setup() 
{
  Serial.begin(115200);
  pinMode(Rled, OUTPUT);
  digitalWrite(Rled, LOW);
  WiFi.mode(WIFI_STA);
   
   pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(LM_IN1, OUTPUT);
  pinMode(LM_IN2, OUTPUT);
  pinMode(RM_IN3, OUTPUT);
  pinMode(RM_IN4, OUTPUT);


  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4,LOW);
  
  // WiFi.mode(WIFI_STA);

}

void loop() 
{
  int xVal = analogRead(joyX);
  int yVal = analogRead(joyY);

  myData.a = xVal;
  myData.b = yVal;
  // Serial.println(myData.a);
  // Serial.println(myData.b);
   if (yVal < 100) 
  {
    movement_Inst_Fwd();
    Serial.println("R send");   
  }

  else if (yVal > 3500) 
  {
    movement_Inst_Bwd();
    Serial.println("B send");

  } 
  else if (xVal < 100) 
  {
    movement_Inst_Rgt();
    Serial.println("F send");

  } 
  else if (xVal > 3000) 
  {
    movement_Inst_Lft();
    Serial.println("L send");
  } 
  else 
  {
    movement_Inst_Stp();
    Serial.println("S send");
  }

}

void movement_Inst_Fwd(void) 
{
  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
  int xVal = myData.a;
  int yVal=myData.b;

  // if(xVal > 3000)
  // {
  //   Serial.println("RS send");

  //   digitalWrite(LM_IN1, HIGH);
  //   digitalWrite(LM_IN2, LOW);
  //   digitalWrite(RM_IN3, LOW);
  //   digitalWrite(RM_IN4, LOW);
  // }
  
  // else if(xVal < 100)
  // {
  //   Serial.println("LS send");

  //   digitalWrite(LM_IN1, LOW);
  //   digitalWrite(LM_IN2, LOW);
  //   digitalWrite(RM_IN3, HIGH);
  //   digitalWrite(RM_IN4, LOW);
  // }
}

void movement_Inst_Rgt(void)
{
  
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, HIGH);
  digitalWrite(RM_IN4, LOW);
 
}

void movement_Inst_Lft(void) 
{

  digitalWrite(LM_IN1, HIGH);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);  

}

void movement_Inst_Bwd(void) 
{
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, HIGH);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4, HIGH);
}

void movement_Inst_Stp(void) 
{
  digitalWrite(LM_IN1, LOW);
  digitalWrite(LM_IN2, LOW);
  digitalWrite(RM_IN3, LOW);
  digitalWrite(RM_IN4,LOW);
}
