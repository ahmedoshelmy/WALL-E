#include "communication.h"
#include <Servo.h>

int commands = 0;
Command c;

#define RIGHT_CAMERA_SERVO_PIN 16
#define LEFT_CAMERA_SERVO_PIN 15

// TODO: Set Ultrasonic pin
#define ULTRA_SONIC_PIN_T 9
#define ULTRA_SONIC_PIN_E 8
int rotationOfCamera = 45;

int game_mode = 0;  // for switch between game modes

Servo camera_right;  // for camera
Servo camera_left;   // for camera


int ENR = 3;
int ENL = 4;  // Enable Left
int right_motor_front1 = 36;
int right_motor_front2 = 37;
int right_motor_back1 = 39;
int right_motor_back2 = 38;
int left_motor_front1 = 26;
int left_motor_front2 = 27;
int left_motor_back1 = 29;
int left_motor_back2 = 28;
int rollerA = 22;
int rollerB = 23;

// ====== Ball Collection Variables
//* action_bc:
//* 0 => mean nothing
//* 1 => mean take forward
//* 2 => mean right
//* 3 => mean left
//* 4 => avoid forward
//* 6 => camera down

//===================== END: Variables =====================
//===================== Start: Methods =====================

void Forward(float l_speed, float r_speed) {

  digitalWrite(right_motor_front1, HIGH);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, HIGH);
  digitalWrite(right_motor_back2, LOW);
  digitalWrite(left_motor_front1, HIGH);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, HIGH);
  digitalWrite(left_motor_back2, LOW);

  analogWrite(ENL, l_speed);
  analogWrite(ENR, r_speed);
  // for(int i = 15 ; i <= 255;i+=20){
  //   analogWrite(ENL, i);
  //   analogWrite(ENR, i);
  //   delay(100) ;
  // }
}


void onRoller() {
  digitalWrite(rollerA, HIGH);
  digitalWrite(rollerB, LOW);
}

void offRoller() {
  digitalWrite(rollerB, LOW);
  digitalWrite(rollerA, LOW);
}





void Back(float speed) {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, HIGH);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, HIGH);
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, HIGH);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, HIGH);
  analogWrite(ENL, speed);
  analogWrite(ENR, speed);
}

void Left(float speed) {
  print("Arduino : move left");
  digitalWrite(right_motor_front1, HIGH);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, HIGH);
  digitalWrite(right_motor_back2, LOW);

  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, HIGH);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, HIGH);
  analogWrite(ENL, speed);
  analogWrite(ENR, speed);
}



void Right(float speed) {
  print("Arduino : move right");
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, HIGH);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, HIGH);

  digitalWrite(left_motor_front1, HIGH);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, HIGH);
  digitalWrite(left_motor_back2, LOW);

  analogWrite(ENL, speed);
  analogWrite(ENR, speed);
}



void Stop() {
  digitalWrite(right_motor_front1, LOW);
  digitalWrite(right_motor_front2, LOW);
  digitalWrite(right_motor_back1, LOW);
  digitalWrite(right_motor_back2, LOW);
  digitalWrite(left_motor_front1, LOW);
  digitalWrite(left_motor_front2, LOW);
  digitalWrite(left_motor_back1, LOW);
  digitalWrite(left_motor_back2, LOW);
  analogWrite(ENL, 0);
  analogWrite(ENR, 0);
}

// I Don't What it do But I found it in defending code

void Camera_up(int angle) {
  camera_right.write(90 + angle);  // go up
}


//when take nothing and ultrasoic does not take action the camera must take up action from ardrunio only

int ReadUS() {
  // to read ultrasonic sensor value
  // Clears the trigPin

  digitalWrite(ULTRA_SONIC_PIN_T, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ULTRA_SONIC_PIN_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_SONIC_PIN_T, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(ULTRA_SONIC_PIN_E, HIGH);
  // Calculating the distance
  float distance = duration * 0.034 / 2;

  Serial.print("dist : ");
  Serial.println(distance);
  return distance;
}

void CameraAngle(int ch_angle) {
}

int default_angle = 45;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(31250);

  pinMode(right_motor_front1, OUTPUT);
  pinMode(right_motor_front2, OUTPUT);
  pinMode(right_motor_back1, OUTPUT);
  pinMode(right_motor_back2, OUTPUT);
  pinMode(left_motor_front1, OUTPUT);
  pinMode(left_motor_front2, OUTPUT);
  pinMode(left_motor_back1, OUTPUT);
  pinMode(left_motor_back2, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(ENL, OUTPUT);
  pinMode(rollerA, OUTPUT);
  pinMode(rollerB, OUTPUT);

  camera_right.attach(RIGHT_CAMERA_SERVO_PIN);

  camera_right.write(default_angle);
  camera_left.write(default_angle);


  pinMode(ULTRA_SONIC_PIN_T, OUTPUT);  // Sets the trigPin as an Output
  pinMode(ULTRA_SONIC_PIN_E, INPUT);   // Sets the echoPin as an Input
  print("========== Start ball collecting ==========\n");
  //print("$start{m=2,d=90}"); //right
  //print("$start{m=2,d=-90}"); //left
}

void loop() {
  // Serial.print(ReadUS());
  //nothing, TK_Forward, TK_right, TK_left, AVD_forward, CAMERA_DOWN
  //0         1           2         3        4        5
  // put your main code here, to run repeatedly:
  if (c.read()) {
    if (c == "set_d") {
      int d = c["d"].get<int>();
      if (d == 0) {
        print("Arduino : Do Nothing => camera up \n");
        //here we should make the camera up of the robot at the state
        Camera_up(rotationOfCamera);

        Stop();


      } else if (d == 1) {

        int forward_speed = 255;
        print("Arduino : Take Forward \n");
        Forward(forward_speed, forward_speed);
        
      } else if (d == 2) {
        print("Arduino : Take right \n");

        int take_right_around_speed = 255;
        int take_right_forward_speed = 255;
        bool option = 0;


        if (option)
          Forward(take_right_forward_speed, 0);
        else Right(take_right_around_speed);


        onRoller();
      } else if (d == 3) {
        print("Arduino : Take left \n");

        int take_left_around_speed = 255;
        int take_left_forward_speed = 255;
        bool option = 0;


        if (option)
          Forward(0, take_left_forward_speed);
        else Left(take_left_around_speed);


        onRoller();
      } else if (d == 4) {
        print("Arduino : avoid forward by taken left \n");

        int avoid_around_speed = 255;
        int avoid_forward_speed = 255;
        bool option = 0;


        if (option)
          Forward(0, avoid_forward_speed);
        else Left(avoid_around_speed);

        onRoller();
      } else if (d == 5) {
        print("Arduino : CAMERA_DOWN \n");
      }
    }
  }
}

