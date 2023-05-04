#include <Wire.h>
#include <MPU9250.h>

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

void pinModeMotors()
{
    pinMode(right_motor_front1, OUTPUT);
    pinMode(right_motor_front2, OUTPUT);
    pinMode(right_motor_back1, OUTPUT);
    pinMode(right_motor_back2, OUTPUT);
    pinMode(left_motor_front1, OUTPUT);
    pinMode(left_motor_front2, OUTPUT);
    pinMode(left_motor_back1, OUTPUT);
    pinMode(left_motor_back2, OUTPUT);
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

MPU9250 IMU(Wire, 0x68); // MPU9250 is a class and "IMU" is a object, we need to pass parameter to the object "IMU". wire is used for I2C communication,
                         // second parameter is for I2C address, we left the ADO pin unconnected so its set to low, 0x68 is address,
                         // if it was high then the address is 0x69

// PID parameters
const float Kp = 1;
const float Ki = 0.1;
const float Kd = 0.1;
const int maxSpeed = 255;
// Variables for IMU
float ax, ay, az;
float vx = 0, vy = 0, vz = 0;
float px = 0, py = 0, pz = 0;

void MoveForward(float targetDistance)
{
    // Variables for PID algorithm
    float lastError = 0;
    float totalError = 0;
    unsigned long lastTime = 0;

    // ========== ================= ================
    // const float targetDistance = 300;
    float distanceTraveled = 0;
    while (distanceTraveled < targetDistance)
    {
        // Read accelerometer data from MPU9250
        ax = IMU.getAccelX_mss(); // m/sec*sec
        ay = IMU.getAccelY_mss();
        az = IMU.getAccelZ_mss();
        // mpu9250.readAccelData(&ax, &ay, &az);

        // Calculate velocity
        unsigned long now = millis();
        float dt = (now - lastTime) / 1000.0;
        vx += ax * dt;
        vy += ay * dt;
        vz += az * dt;

        // Calculate position
        px += vx * dt;
        py += vy * dt;
        // pz += vz * dt;

        // Calculate distance traveled
        distanceTraveled = sqrt(px * px + py * py /*+ pz * pz*/);

        // PID control to adjust motor speed based on distance error
        float error = targetDistance - distanceTraveled;
        // Calculate PID components
        float P = Kp * error;
        totalError += error * dt;
        float I = Ki * totalError;
        float D = Kd * (error - lastError) / dt;
        lastError = error;

        // Calculate motor speed using PID components
        int leftSpeed = max(min(P + I + D, maxSpeed), -maxSpeed);
        int rightSpeed = max(min(P + I + D, maxSpeed), -maxSpeed);

        // Set motor speed
        Forward(leftSpeed, rightSpeed);

        // Set motor speed
        /* if (speed > maxSpeed)
            speed = maxSpeed;
        else if (speed < -maxSpeed)
            speed = -maxSpeed;

        if (error > 0)
            Forward(speed);
        else
            Forward(-speed); */

        // Print debug info
        Serial.print("Distance: ");
        Serial.print(targetDistance);
        /* Serial.print(" cm, Speed: ");
        Serial.print(speed); */
        Serial.print(", Error: ");
        Serial.println(error);
        // Delay for stability
        delay(10);
        // Update last time
        lastTime = now;
    }
}
void takeTurn(float radius, float angle, int RL) // RL = 1 for right | -1 for left
{
    // Calculate distance and motor speed for quarter circle turn
    float targetDistance = RL * radius * M_PI / 2;

    // Initialize variables for IMU
    float gx, gy, gz;
    float heading = 0;

    // Initialize variables for PID algorithm

    float lastError = 0;
    float totalError = 0;
    unsigned long lastTime = 0;

    // Wait for the robot to stop moving
    while (abs(vx) > 0.1 || abs(vy) > 0.1 || abs(vz) > 0.1)
    {
        gx = IMU.getGyroX_rads();
        gy = IMU.getGyroY_rads();
        gz = IMU.getGyroZ_rads();
        vx = vy = vz = 0;
        delay(10);
    }

    // Turn right in quarter circle
    while (heading < angle)
    {
        // Read gyroscope data from MPU9250
        gx = IMU.getGyroX_rads();
        gy = IMU.getGyroY_rads();
        gz = IMU.getGyroZ_rads();

        // Calculate angular velocity and heading
        float dt = (millis() - lastTime) / 1000.0;
        float wx = gx * DEG_TO_RAD;
        float wy = gy * DEG_TO_RAD;
        float wz = gz * DEG_TO_RAD;
        heading += wz * dt;

        // PID control to adjust motor speed based on heading error
        float error = RL * (heading - targetDistance);
        totalError += error;
        float dError = error - lastError;
        float speed = RL * (Kp * error + Ki * totalError + Kd * dError);
        lastError = error;

        // Set motor speed
        if (speed > maxSpeed)
            speed = maxSpeed;
        else if (speed < -maxSpeed)
            speed = -maxSpeed;

        if (RL == 1)
            Forward_The_Right(speed);
        else
            Forward_The_Left(speed);
        // Print debug info
        Serial.print("Heading: ");
        Serial.print(heading);
        Serial.print(", Speed: ");
        Serial.print(speed);
        Serial.print(", Error: ");
        Serial.println(error);

        // Update last time
        lastTime = millis();
    }

    // Stop the robot
    Stop();
}

void setup()
{
    // Initialize serial communication
    Serial.begin(31250);

    // Initialize motor driver pins
    pinModeMotors();

    // Initialize MPU9250
    IMU.begin(); // Initialize the IMU object
}

void loop()
{

    // Move forward 3 meters
    MoveForward(3);

    // Turn right in a quarter circle with 15cm radius
    // turnRight(0.15, 90);
    takeTurn(0.15, 90, 1);

    // Move forward 3 meters
    MoveForward(3);

    // Turn right in a quarter circle with 15cm radius
    // turnRight(0.15, 90);
    takeTurn(0.15, 90, 1);
}