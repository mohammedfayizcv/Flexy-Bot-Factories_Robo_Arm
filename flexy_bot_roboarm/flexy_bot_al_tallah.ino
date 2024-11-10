#include <Servo.h>

// Define pins
#define Joy_X A0
#define Joy_Y A1
#define BUTTON_GRIP 2      // Button to toggle grip and switch modes
#define BUTTON_MODE 5      // Button to toggle between manual and automatic modes
#define SERVO_X_PIN 9      // Pin for Servo_X
#define SERVO_Y_PIN 10     // Pin for Servo_Y
#define SERVO_GRIP_PIN 11  // Pin for Servo_Grip
#define FLAME_SENSOR_PIN A2 // Pin for Flame Sensor
#define LED_PIN 12       // Pin for Red LED

// Servo objects
Servo Servo_X;
Servo Servo_Y;
Servo Servo_Grip;

bool gripState = false;          // State for grip
bool automaticMode = false;      // Start in manual mode

// Current positions of the servos
int currentXPos = 90; // Start in the middle
int currentYPos = 90; // Start in the middle
unsigned long lastPressTime = 0; // Time of last button press
const unsigned long debounceDelay = 300; // Debounce delay in milliseconds
const int sensitivity = 2; // Adjust this value for sensitivity

void setup() {
  // Attach servos
  Servo_X.attach(SERVO_X_PIN);
  Servo_Y.attach(SERVO_Y_PIN);
  Servo_Grip.attach(SERVO_GRIP_PIN);

  // Set pin modes
  pinMode(BUTTON_GRIP, INPUT_PULLUP);
  pinMode(BUTTON_MODE, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  Serial.begin(9600);
}

void loop() {
  // Read flame sensor value
  int flameValue = analogRead(FLAME_SENSOR_PIN);
  if (flameValue < 200) { // Adjust this threshold based on your flame sensor
    digitalWrite(LED_PIN, HIGH); // Turn on the red LED
    delay(2000);
    Serial.println("Flame detected!"); // Send signal to Python
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off the red LED
  }

  // Check for mode switch button press
  if (digitalRead(BUTTON_MODE) == LOW) { // Button pressed
    if (millis() - lastPressTime > debounceDelay) { // Check for debounce
      automaticMode = !automaticMode; // Toggle automatic mode
      lastPressTime = millis(); // Update last press time
      Serial.print("Switched to ");
      Serial.println(automaticMode ? "Automatic Mode" : "Manual Mode");
    }
  }

  // Automatic mode logic
  if (automaticMode) {
    // Step 1: Move X-axis to 0 degrees smoothly
    moveServoSmoothly(Servo_X, currentXPos, 0);

    // Step 2: Open grip to grab the object
    if (!gripState) {
      gripState = true;
      Servo_Grip.write(0); // Open grip
      delay(1000);
    }

    // Step 3: Close grip to hold the object
    // (Add your logic here)

    // Step 4: Move Y-axis to 140 degrees smoothly
    moveServoSmoothly(Servo_Y, currentYPos, 140);
    delay(500);

    if (gripState) {
      gripState = false;
      Servo_Grip.write(90); // Close grip
      delay(1000);
    }

    // Step 5: Move Y-axis to 45 degrees smoothly
    moveServoSmoothly(Servo_Y, currentYPos, 45);
    delay(500);

    // Step 6: Move X-axis to 180 degrees smoothly
    moveServoSmoothly(Servo_X, currentXPos, 180);
    delay(500);

    // Step 7: Move Y-axis to 140 degrees smoothly to release object
    moveServoSmoothly(Servo_Y, currentYPos, 140);
    delay(500);

    // Step 8: Open grip to release the object
    Servo_Grip.write(0); // Open grip to drop the object
    delay(1000);

    // Step 9: Return Y-axis to original position smoothly
    moveServoSmoothly(Servo_Y, currentYPos, 90);
  } 
  else {
    // Manual control logic
    int current_Joy_X_Val = analogRead(Joy_X);
    int current_Joy_Y_Val = analogRead(Joy_Y);

    // Map joystick values to servo angles
    int targetXPos = map(current_Joy_X_Val, 0, 1023, 0, 180);
    int targetYPos = map(current_Joy_Y_Val, 0, 1023, 0, 180);

    // Move servos to joystick positions
    Servo_X.write(targetXPos);
    Servo_Y.write(targetYPos);
  }

  if (digitalRead(BUTTON_GRIP) == LOW) {
      if (millis() - lastPressTime > debounceDelay) {
        gripState = !gripState;
        Servo_Grip.write(gripState ? 0 : 90);
        lastPressTime = millis();
      }
    }
}

void moveServoSmoothly(Servo &servo, int currentPos, int targetPos) {
  int step = (targetPos > currentPos) ? 1 : -1;
  for (int pos = currentPos; pos != targetPos; pos += step) {
    servo.write(pos);
    delay(15); // Adjust delay for speed
  }
}