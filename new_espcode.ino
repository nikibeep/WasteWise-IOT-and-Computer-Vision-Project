#include <Stepper.h>
#include <Wire.h>

#define TRIG_PIN 19
#define ECHO_PIN 4
#define TRIG_PIN1 5
#define ECHO_PIN1 15
#define TRIG_PIN2 2
#define ECHO_PIN2 23
#define TRIG_PIN3 18
#define ECHO_PIN3 21
int ra;
int direction, rotationAngle;
#define ENA 22 // Enable pin for motor A
#define IN1 13 // Input 1 for motor A
#define IN2 27 // Input 2 for motor A
const int frequency = 500;
const int pwm_channel = 0;
const int resolution = 8;

const int stepsPerRevolution = 200;
const int motorPin1 = 14;
const int motorPin2 = 12;

Stepper myStepper(stepsPerRevolution, motorPin1, motorPin2);

int currentAngle = 0;

void calculateRotation(int currentAngle, int targetAngle, int &direction, int &rotationAngle) {
    int clockwiseDistance = (targetAngle - currentAngle + 360) % 360;
    int counterclockwiseDistance = (currentAngle - targetAngle + 360) % 360;

    if (clockwiseDistance <= counterclockwiseDistance) {
        direction = 1; // 1 for clockwise
        rotationAngle = clockwiseDistance;
    } else {
        direction = -1; // -1 for counterclockwise
        rotationAngle = counterclockwiseDistance;
    }
    fn();
    rtr();
}

long measureDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration / 58;
}

void rotate_clockwise() {
    ledcWrite(pwm_channel, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void rtr(){
      delay(2000);
      rotate_clockwise(); // Rotate the DC motor clockwise
      delay(2000);
      rotate_anticlockwise();
}

void fn(){

  myStepper.step(stepsPerRevolution * (rotationAngle / 90));

}


void rotate_anticlockwise() {
    ledcWrite(pwm_channel, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void setup() {

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  myStepper.setSpeed(500); // Set the motor speed (60 RPM in this case)
  Serial.begin(9600);
}

void loop() {
   
    int input;
    int v;
    int ca = currentAngle;
    

  long distance = measureDistance(TRIG_PIN, ECHO_PIN);
  Serial.print("Bin 1: ");
  Serial.print(distance);
  long distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);
  Serial.print("Bin 2: ");
  Serial.print(distance1);
  long distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);
  Serial.print("Bin 3: ");
  Serial.print(distance2);
  long distance3 = measureDistance(TRIG_PIN3, ECHO_PIN3);
  Serial.print("Bin 4: ");
  Serial.print(distance3);

  Serial.println("Enter a target angle (1-4): ");

  input = Serial.parseInt();

  if (input == 1) {
      calculateRotation(currentAngle, 0, direction, rotationAngle);
      
  } else if (input == 2) {
      calculateRotation(currentAngle, 90, direction, rotationAngle);
      
  } else if (input == 3) {
      calculateRotation(currentAngle, 180, direction, rotationAngle);
      
  } else if (input == 4) {
      calculateRotation(currentAngle, 270, direction, rotationAngle);
      
  }

  ra = rotationAngle;

  Serial.print("Rotate ");
  Serial.print(ra);
  Serial.print(" degrees in a ");
  Serial.print((direction == 1) ? "clockwise" : "counterclockwise");
  Serial.println(" direction to reach the target angle.");

  currentAngle = (currentAngle + ra * direction + 360) % 360;

  if (direction != 1) {
      v = -ra;
  } else {
      v = ra;
  }

  // if (v != 0) {
  //     // Rotate the motor based on the value of v
  //     myStepper.step(stepsPerRevolution * (v / 90));
  //    // Rotate the DC motor anticlockwise

  // }
}
