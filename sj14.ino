#include <WiFi.h>
#include <WebServer.h>
#include <Stepper.h>
#include <Arduino.h>
#include <Wire.h>

// Replace with your network credentials
const char* ssid = "stinger";
const char* password = "honeybee4U";

WebServer server(80);

#define TRIG_PIN 19
#define ECHO_PIN 4
#define TRIG_PIN1 5
#define ECHO_PIN1 15
#define TRIG_PIN2 2
#define ECHO_PIN2 23
#define TRIG_PIN3 18
#define ECHO_PIN3 21

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
int lastInput = -1; // Add a variable to store the last input

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

void rotate_anticlockwise() {
    ledcWrite(pwm_channel, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}


void handleRoot() {
    server.send(200, "text/plain", "Hello from ESP32!"); // Send a response to the client
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
  Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting to WiFi...");


}

void loop() {

    server.handleClient();
    delay(10);

    int direction, rotationAngle;
    char input;
    int v;
    int ca = currentAngle;
    int ra;

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

 String data = server.arg("plain"); // Get the POST data
 Serial.println("Received data: " + data);

 if (data.length() > 0) {
        char input = data[0];

        if (input == 'P') {
            // Handle 'P' command - perform action for 'P'
            calculateRotation(currentAngle, 0, direction, rotationAngle);
            // You can add your custom code here for the 'P' command.
            server.send(200, "text/plain", "P CMD: Action for 'P' command");
        } else if (input == 'B') {
            // Handle 'B' command - perform action for 'B'
            calculateRotation(currentAngle, 90, direction, rotationAngle);
            // You can add your custom code here for the 'B' command.
            server.send(200, "text/plain", "B CMD: Action for 'B' command");
        } else if (input == 'M') {
            // Handle 'M' command - perform action for 'M'
            calculateRotation(currentAngle, 180, direction, rotationAngle);
            // You can add your custom code here for the 'M' command.
            server.send(200, "text/plain", "M CMD: Action for 'M' command");
        } else if (input == 'O') {
            // Handle 'O' command - perform action for 'O'
            calculateRotation(currentAngle, 270, direction, rotationAngle);
            // You can add your custom code here for the 'O' command.
            server.send(200, "text/plain", "O CMD: Action for 'O' command");
        } else {
            // If the received command is not recognized, you can send an error response.
            server.send(400, "text/plain", "Unknown command");
        }

    lastInput = input;
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

    if (v != 0) {
        // Rotate the motor based on the value of v
        myStepper.step(stepsPerRevolution * (v / 90));
      
        delay(2000);
        rotate_clockwise(); // Rotate the DC motor clockwise
        delay(2000);
        rotate_anticlockwise(); // Rotate the DC motor anticlockwise
    }
  }
}
