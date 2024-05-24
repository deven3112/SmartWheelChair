// Define pins for Hall effect sensors
const int hallSensor1 = 23;  // Connect to the first sensor
const int hallSensor2 = 33;  // Connect to the second sensor
const int hallSensor3 = 34;  // Connect to the third sensor

// Define pins for L298N motor driver
const int motor1A = 12;
const int motor1B = 14;
const int motor2A = 27;
const int motor2B = 26;

// Define PWM values for speed control
const int motorSpeed = 150;  // Adjust this value based on your setup

void setup() {
  Serial.begin(115200);
  // Initialize Hall effect sensor pins
  pinMode(hallSensor1, INPUT);
  pinMode(hallSensor2, INPUT);
  pinMode(hallSensor3, INPUT);

  // Initialize motor driver pins
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
}

void loop() {
  // Read the status of each Hall effect sensor
  int sensor1Value = digitalRead(hallSensor1);
  int sensor2Value = digitalRead(hallSensor2);
  int sensor3Value = digitalRead(hallSensor3);

  // Motor control based on sensor values
  if (sensor1Value == HIGH) {
    // Magnet detected near sensor 1, turn left
    analogWrite(motor1A, motorSpeed);
    analogWrite(motor1B, 0);
    analogWrite(motor2A, motorSpeed);
    analogWrite(motor2B, 0);
    Serial.println("left");
  } else if (sensor2Value == HIGH) {
    // Magnet detected near sensor 2, go forward
     analogWrite(motor1A, 0);
    analogWrite(motor1B, 0);
    analogWrite(motor2A, 0);
    analogWrite(motor2B, motorSpeed);
        Serial.println("Right");

  } else if (sensor3Value == LOW) {
    // Magnet detected near sensor 3, turn right
   analogWrite(motor1A, motorSpeed);
    analogWrite(motor1B, 0);
    analogWrite(motor2A, 0);
    analogWrite(motor2B, motorSpeed);
        Serial.println("forward");

  } else {
    // No magnet detected, stop the motors
    analogWrite(motor1A, 0);
    analogWrite(motor1B, 0);
    analogWrite(motor2A, 0);
    analogWrite(motor2B, 0);
    Serial.println("stop");
  }
}
