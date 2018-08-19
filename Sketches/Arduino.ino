// Main Program Arduino 
#include <AFMotor.h> 
#include <Servo.h>
#define MAX_SPEED 250 // Max Speed of Motor

AF_DCMotor leftMotor(1, MOTOR12_8KHZ); // Initialize Left motor
AF_DCMotor rightMotor(3, MOTOR12_8KHZ); // Initialize Right motor 

AF_DCMotor redLed(2, MOTOR12_8KHZ); // Initialize Red Led
AF_DCMotor blueLed(4, MOTOR12_8KHZ); // Initialize Blue Led

int speedSet = 0;
int a, b, c;
String dir = "NA";

Servo neck;
int neckPos = 0;
unsigned long StartTime = millis(), CurrentTime = 0, 
  RedLedStart=0, BlueLedStart=0;

void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(A5, INPUT);

  neck.attach(10);  
  delay(1000);
  rightMotor.setSpeed(MAX_SPEED);
  leftMotor.setSpeed(MAX_SPEED);
  redLed.setSpeed(MAX_SPEED);
  blueLed.setSpeed(MAX_SPEED);
}

void loop() {
  a = digitalRead(A1);
  b = digitalRead(A3);
  c = digitalRead(A5);
  Serial.print(a);
  Serial.print("  ");
  Serial.print(b);
  Serial.print("  ");
  Serial.print(c);
  Serial.println("  ");
  
  CurrentTime = millis();
  
  if(a==0 & b==0 & c==0) {
    stopHere();
    blueLed.run(RELEASE);
    blinkLed(RedLedStart, redLed, 0, 400, 300);
    blinkLed(BlueLedStart, blueLed, 1100, 400, 300);
  }

  if(a==1 & b==1 & c==1) {
    Go_Forward();
  }

  if(a==1 & b==1 & c==0) {
    Backward();
    redLed.run(FORWARD);
    blueLed.run(RELEASE);
  }

  if(a==1 & b==0 & c==0) {
    Go_Left();
  }

  if(a==0 & b==1 & c==0) {
    Go_Right();
  }

}

void stepBackward() {
  Backward();
  delay(500);
  stopHere();
}

void Go_Forward() {
  turn(90);
  Forward();
}

void Forward() {
  leftMotor.run(FORWARD); 
  rightMotor.run(FORWARD); 
  dir = "FW";
  redLed.run(RELEASE);
  blueLed.run(FORWARD);
}

void Backward() {                                                             // ---- Go Backward
  leftMotor.run(BACKWARD); 
  rightMotor.run(BACKWARD);
  dir = "BW";
}

void stopHere() {                                                                // ---- Stop Now
  if(dir=="FW") {
    Backward();
    delay(200);
  } else if(dir=="BW") {
    Forward();
    delay(200);
  }  
  dir = "NA";
  leftMotor.run(RELEASE); 
  rightMotor.run(RELEASE);
}

void Go_Right() {                                                                 // ---- Turn Right and Go Forward
  turn(180);
  delay(400);
  Forward();
}  

void Go_Left() {                                                                  // ---- Turn Left and Go Forward
  turn(0);
  delay(400);
  Forward();
}

void turn(int angle) {
  Serial.print("Turn : ");
  Serial.println(angle);
  neck.write(angle);
  neckPos = angle;
}

void blinkLed(unsigned long &myTimer, AF_DCMotor &myLed, int startTime, int glowTime, int offTime) {
    int elspsedTime = CurrentTime-myTimer;
    glowTime+=startTime;
    if(myTimer==0) {
      myTimer = CurrentTime;
    } else if(elspsedTime>startTime && elspsedTime<(glowTime)) {
      myLed.run(FORWARD);
      Serial.println("Glow");
    } else if(elspsedTime>glowTime && elspsedTime<(glowTime+offTime)) {
      myLed.run(RELEASE);
      Serial.println("Released");
    } else if(elspsedTime>(glowTime+offTime)) {
      myTimer = 0;
      Serial.println("Reset");
    }
}

 
