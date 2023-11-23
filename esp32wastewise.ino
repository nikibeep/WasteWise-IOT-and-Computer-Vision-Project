#include <WiFi.h>
#include <WebServer.h>
#include <Stepper.h>
#include <Arduino.h>
#include <Wire.h>// Include the AccelStepper library for controlling the stepper motor

// Replace with your network credentials
const char* ssid = "stinger";
const char* password = "honeybee4U";

WebServer server(80);  // Create a web server object

const int stepsPerRevolution = 200;
const int motorPin1 = 14;
const int motorPin2 = 12;

Stepper myStepper(stepsPerRevolution, motorPin1, motorPin2); // Initialize the stepper motor

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
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Connected to WiFi");

    server.on("/", handleRoot); // Handle HTTP GET requests
    Serial.println("Handling client request...");
    server.on("/data", handleData); // Handle HTTP POST requests
    server.begin(); // Start the server

    myStepper.setSpeed(500);
}

void loop() {
    server.handleClient();
    delay(10); // Handle client requests
}

void handleRoot() {
    server.send(200, "text/plain", "Hello from ESP32!"); // Send a response to the client
}

void handleData() {
    int direction, rotationAngle;
    int input;
    int v;
    int ca = currentAngle;
    int ra;

    String data = server.arg("plain"); // Get the POST data
    Serial.println("Received data: " + data);

    if (data.length() > 0) {
        char command = data[0];

        switch (command) {
            case 'P':
                // Handle 'P' command - perform action for 'P'
                calculateRotation(currentAngle, 0, direction, rotationAngle);
                // You can add your custom code here for the 'P' command.
                server.send(200, "text/plain", "P CMD: Action for 'P' command");
                break;

            case 'B':
                // Handle 'B' command - perform action for 'B'
                calculateRotation(currentAngle, 90, direction, rotationAngle);
                // You can add your custom code here for the 'B' command.
                server.send(200, "text/plain", "B CMD: Action for 'B' command");
                break;

            case 'M':
                // Handle 'M' command - perform action for 'M'
                calculateRotation(currentAngle, 180, direction, rotationAngle);
                // You can add your custom code here for the 'M' command.
                server.send(200, "text/plain", "M CMD: Action for 'M' command");
                break;

            case 'O':
                // Handle 'O' command - perform action for 'O'
                calculateRotation(currentAngle, 270, direction, rotationAngle);
                // You can add your custom code here for the 'O' command.
                server.send(200, "text/plain", "O CMD: Action for 'O' command");
                break;

            default:
                // If the received command is not recognized, you can send an error response.
                server.send(400, "text/plain", "Unknown command");
                break;
        }
    } else {
        // If no data was received, you can send an error response.
        server.send(400, "text/plain", "No command data received");
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

    if (v != 0) {
        // Rotate the motor based on the value of v
        myStepper.step(stepsPerRevolution * (v / 90)); // Move the stepper motor
        // Wait for the motor to complete the move
    }
}


