
// ==== motor driver pins
int ENR = 3;
int ENL = 4; // Enable Left
int right_motor_front1 = 36;
int right_motor_front2 = 37;
int right_motor_back1 = 38;
int right_motor_back2 = 39;
int left_motor_front1 = 26;
int left_motor_front2 = 27;
int left_motor_back1 = 28;
int left_motor_back2 = 29;
int rollerA = 22 ; 
int rollerB = 23 ; 
int DCMotorSoccerA = 24  ; 
int DCMotorSoccerB = 25  ; 


void pinModeMotors()
{
    // DC Motors to RUN 
    pinMode(right_motor_front1, OUTPUT);
    pinMode(right_motor_front2, OUTPUT);
    pinMode(right_motor_back1, OUTPUT);
    pinMode(right_motor_back2, OUTPUT);
    pinMode(left_motor_front1, OUTPUT);
    pinMode(left_motor_front2, OUTPUT);
    pinMode(left_motor_back1, OUTPUT);
    pinMode(left_motor_back2, OUTPUT);
  // BALL COLLECTING 
    pinMode(rollerA , OUTPUT) ; 
    pinMode(rollerB , OUTPUT) ; 
    // SOCCER
     pinMode(DCMotorSoccerA , OUTPUT) ; 
    pinMode(DCMotorSoccerB  , OUTPUT) ; 
}

void onRoller(){
   digitalWrite(rollerA, HIGH);
   digitalWrite(rollerB, LOW);
}
void offRoller(){
   digitalWrite(rollerB, LOW);
   digitalWrite(rollerA, LOW);
}
void DCMotorSoccerON(){
   digitalWrite(DCMotorSoccerA, HIGH);
   digitalWrite(DCMotorSoccerB, LOW);
}
void DCMotorSoccerOFF(){
   digitalWrite(DCMotorSoccerA, LOW);
   digitalWrite(DCMotorSoccerB, LOW);
}


void Forward(float l_speed, float r_speed)
{

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
}

void Back(float speed)
{
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

void Left(float speed)
{
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

void Forward_The_Right(float speed)
{
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}
void Forward_The_Left(float speed)
{
    digitalWrite(right_motor_front1, HIGH);
    digitalWrite(right_motor_front2, LOW);
    digitalWrite(right_motor_back1, HIGH);
    digitalWrite(right_motor_back2, LOW);
    analogWrite(ENL, speed);
    analogWrite(ENR, speed);
}

void Right(float speed)
{
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

void U_Turn(float speed)
{
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

    // delay(5000);
}

void Stop()
{
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

void MazeGame(){
  
}

void BallCollectingGame(){

  
}
void ShootingGame(){
  
}
void DefendingGame(){
  
}
void GunGame(){
  
}

void setup()
{
   Serial.begin(9600); 
}

void loop()
{
 DCMotorSoccerON(); 
 delay(500) ;
 DCMotorSoccerOFF();
  
}
