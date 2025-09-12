#include <Servo.h>

// Pin Assignments
#define LED_PIN 13         // Status LED
#define SERVO_PIN 2        // Servo signal pin (Braille wheel)

// New LED pins for movement instructions
#define RED_LED_PIN 4      // Red LED: translator does NOT need to be moved
#define UNUSED_PIN 5       // Pin 5 (currently unused)
#define GREEN_LED_PIN 6    // Green LED: translator needs to be moved

// Stepper Motor Control Pins for DRV8833
#define AIN1 8      // Stepper Coil A+
#define AIN2 9      // Stepper Coil A-
#define BIN1 10     // Stepper Coil B+
#define BIN2 11     // Stepper Coil B-

// Full-step sequence (4-step mode)
const int stepSequence[4][4] = {
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 0, 1}
};

int stepIndex = 0; // Step position tracker

// Create Servo object
Servo brailleServo;

// Servo home position
const int homePosition = 0;

// Braille dot positions (adjust angles as needed)
int dotPositions[7];

void setup() {
    Serial.begin(9600);  // Initialize Serial Communication
    pinMode(LED_PIN, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);

    // Setup new LED pins for movement instructions
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(UNUSED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    // Default state: red on (translator not moving), green off.
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);

    brailleServo.attach(SERVO_PIN);

    dotPositions[1] = 20;
    dotPositions[2] = 40;
    dotPositions[3] = 60;
    dotPositions[4] = 80;
    dotPositions[5] = 100;
    dotPositions[6] = 120;

    brailleServo.write(homePosition);
    
    Serial.println("Arduino Ready. Waiting for Braille instructions...");
}

void loop() {
    // **Check if full data is available**
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');  // Read until newline ('\n')
        input.trim();  // Remove any accidental whitespace
        
        Serial.print("✅ Received: ");
        Serial.println(input);

        // Blink status LED to confirm data receipt
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);

        // Process each character in the received string
        for (int i = 0; i < input.length(); i++) {
            char incomingByte = input[i];

            if (incomingByte >= '1' && incomingByte <= '6') {
                int dot = incomingByte - '0';  // Convert char '1'-'6' to int
                setDotPosition(dot);
                pressProng();
            } 
            else if (incomingByte == 'M') {
                moveToNextLetter();
            } 
            else if (incomingByte == 'N') {
                newlineAction();
            }
        }
        
        Serial.flush();  // **Clear buffer to avoid stale data**
    }
}

// Moves the servo to the correct dot position
void setDotPosition(int dot) { 
    if (dot < 1 || dot > 6) return; // Safety check
    int pos = dotPositions[dot];
    Serial.print("Setting servo to dot ");
    Serial.print(dot);
    Serial.print(" at position ");
    Serial.println(pos);

    brailleServo.write(pos);
    delay(500);
}

// Press the prong by rotating the stepper motor
void pressProng() {
    Serial.println("Pressing prong...");
    stepMotor(5, -75);  // Perform step motion
    delay(500);
    releaseProng();
}

// Release the prong by rotating the stepper motor in reverse
void releaseProng() {
    Serial.println("Releasing prong...");
    stepMotor(5, 75);
}

// Stops the motor by setting all stepper inputs LOW
void stopMotor() {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    Serial.println("Motor stopped.");
}

// Move the stepper motor
void stepMotor(int stepDelay, int steps) {
    int direction = (steps > 0) ? 1 : -1;
    steps = abs(steps);

    for (int i = 0; i < steps; i++) { 
        digitalWrite(AIN1, stepSequence[stepIndex][0]);
        digitalWrite(AIN2, stepSequence[stepIndex][1]);
        digitalWrite(BIN1, stepSequence[stepIndex][2]);
        digitalWrite(BIN2, stepSequence[stepIndex][3]);

        stepIndex = (stepIndex + direction + 4) % 4;
        delay(stepDelay);
    }
    stopMotor();
}

// Move to next letter by resetting the wheel position
void moveToNextLetter() {
    // Signal that the translator needs to be moved:
    // Turn green LED on and red LED off.
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    
    Serial.println("Green light: Please move the translator to the next letter spot.");
    delay(1500);  // Delay for user to move the device

    // Perform the movement
    Serial.println("Moving to next letter.");
    brailleServo.write(homePosition);
    delay(500);
    
    // Return to default state: red light on.
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
}

// Handle newline (reset position or move paper)
void newlineAction() {
    Serial.println("Newline received. Resetting position.");
    brailleServo.write(homePosition);
    delay(500);
}
