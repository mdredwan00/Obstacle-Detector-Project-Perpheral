#include <Servo.h>

// Pins
#define trigPin 9
#define echoPin 10
#define servoPin 6
#define ledPin 7          // Main LED
#define buzzerPin 8
#define ledPin2 2         // Additional LED 1
#define ledPin3 4         // Additional LED 2

// Objects and variables
Servo myServo;
long duration;
int distance;

// Servo sweeping variables for 360° rotation
int servoAngle = 0;   // Start at 0° for 360° rotation
int step = 2;         // Increased step for faster 360° rotation
                     // Direction: positive=forward, negative=backward

// SHORTENED DISTANCE THRESHOLDS
int alertThreshold = 50;    // Warning at 50cm
int stopThreshold = 20;     // Emergency stop at 20cm

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);    // Main LED
  pinMode(ledPin2, OUTPUT);   // Additional LED 1
  pinMode(ledPin3, OUTPUT);   // Additional LED 2
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize all LEDs to OFF
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  
  // Initialize servo for 360° continuous rotation
  myServo.attach(servoPin);
  myServo.write(90);  // Start stopped (90 = stop for 360° servos)
  
  // Initialize Serial
  Serial.begin(9600);
  Serial.println("=== 360° SERVO ROTATION TEST ===");
  Serial.println("Distance thresholds: 50cm(warn), 20cm(stop)");
  Serial.println("LEDs: Pin7=Main, Pin2=LED2, Pin4=LED3");
  Serial.println("Place hand close to sensor to stop rotation");
  Serial.println("=============================================");
  
  // LED test sequence
  testLEDs();
  delay(1000);
}

void testLEDs() {
  Serial.println("Testing LEDs...");
  
  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  
  digitalWrite(ledPin2, HIGH);
  delay(300);
  digitalWrite(ledPin2, LOW);
  
  digitalWrite(ledPin3, HIGH);
  delay(300);
  digitalWrite(ledPin3, LOW);
  
  // All LEDs on
  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  delay(500);
  
  // All LEDs off
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  
  Serial.println("LED test complete!");
}

void loop() {
  // Read distance from sensor
  distance = readDistance();
  
  // Print debug info
  Serial.print("Distance: ");
  if (distance == 400) {
    Serial.print("NO_ECHO");
  } else {
    Serial.print(distance);
    Serial.print("cm");
  }
  Serial.print(" | Servo Pos: ");
  Serial.print(servoAngle);
  Serial.print("° | Action: ");
  
  // SHORT DISTANCE LOGIC:
  // If obstacle detected within stopThreshold (20cm) - EMERGENCY STOP
  if (distance < stopThreshold && distance != 400) {
    Serial.println("EMERGENCY STOP (Too close!)");
    
    // 1. Stop servo completely (90 for 360° servos)
    myServo.write(90);
    
    // 2. Turn on ALL LEDs SOLID RED (for emergency)
    digitalWrite(ledPin, HIGH);    // Main LED on
    digitalWrite(ledPin2, HIGH);   // LED2 on
    digitalWrite(ledPin3, HIGH);   // LED3 on
    
    // 3. Continuous alarm
    tone(buzzerPin, 1500);  // Higher pitch for emergency
    
    // 4. Hold stopped state
    delay(200);
    
    // Don't update servoAngle while stopped
    
  } 
  // If obstacle detected within alertThreshold (50cm) - SLOW DOWN/PAUSE
  else if (distance < alertThreshold && distance != 400) {
    Serial.println("SLOW DOWN (Obstacle detected)");
    
    // 1. Slow rotation (for 360° servos, values closer to 90 slow it down)
    if (step > 0) {
      // If moving forward, slow down
      myServo.write(100);  // Slow clockwise
    } else {
      // If moving backward, slow down
      myServo.write(80);   // Slow counter-clockwise
    }
    
    // 2. BLINKING LED PATTERN for warning
    unsigned long currentTime = millis();
    int blinkPhase = (currentTime / 200) % 6;  // 6 phases for 3 LEDs
    
    switch(blinkPhase) {
      case 0:
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        break;
      case 1:
        digitalWrite(ledPin, LOW);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);
        break;
      case 2:
        digitalWrite(ledPin, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, HIGH);
        break;
      case 3:
        digitalWrite(ledPin, LOW);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);
        break;
      case 4:
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        break;
      case 5:
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        break;
    }
    
    // 3. Intermittent beeping
    tone(buzzerPin, 1000, 200);
    delay(300);
    
    // 4. Update angle slowly
    servoAngle += (step / 2);  // Half speed
    
  } 
  // No obstacle - FULL SPEED 360° ROTATION
  else {
    Serial.println("FULL SPEED ROTATION");
    
    // Turn off buzzer
    noTone(buzzerPin);
    
    // LED PATTERN for normal operation (chasing pattern)
    int ledPattern = servoAngle % 120;  // Pattern repeats every 120°
    
    if (ledPattern < 40) {
      // First third: LED1 on, others off
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    } else if (ledPattern < 80) {
      // Second third: LED2 on, others off
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, LOW);
    } else {
      // Third third: LED3 on, others off
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, HIGH);
    }
    
    // FULL SPEED 360° ROTATION
    // For 360° continuous rotation servos:
    // 0 = full speed CCW, 180 = full speed CW, 90 = stop
    
    // Update angle for 360° rotation
    servoAngle += step;
    
    // Wrap around at 360° (not 180°)
    if (servoAngle >= 360) {
      servoAngle = 0;
      Serial.println(">>> Completed 360°, restarting");
      
      // Flash all LEDs to indicate full rotation
      digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin2, HIGH);
      digitalWrite(ledPin3, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
    }
    if (servoAngle < 0) {
      servoAngle = 359;
      Serial.println(">>> Reversed to 359°");
    }
    
    // For 360° servo, map angle to servo speed
    int servoCommand;
    if (step > 0) {
      // Clockwise rotation
      servoCommand = map(servoAngle, 0, 360, 95, 180);  // CW speeds
    } else {
      // Counter-clockwise rotation
      servoCommand = map(servoAngle, 0, 360, 85, 0);    // CCW speeds
    }
    
    myServo.write(servoCommand);
    
    // Control rotation speed (smaller delay = faster)
    delay(50);
  }
  
  // Optional: Auto direction change every 5 rotations
  checkRotationDirection();
}

// Function to read distance from HC-SR04
int readDistance() {
  // Clear trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10us pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo pulse with shorter timeout for faster response
  duration = pulseIn(echoPin, HIGH, 20000); // Timeout after 20ms
  
  // If no echo received
  if (duration == 0) {
    return 400;  // Return large value to indicate no obstacle
  }
  
  // Calculate distance in cm
  int cm = duration * 0.034 / 2;
  
  // Validate reading
  if (cm > 400 || cm < 2) {
    return 400;  // Return error value
  }
  
  return cm;
}

// Optional: Change rotation direction automatically
void checkRotationDirection() {
  // Change direction every 5 full rotations for variety
  static int rotationCount = 0;
  
  if (servoAngle == 0 && step > 0) {
    rotationCount++;
    if (rotationCount >= 5) {
      step = -2;  // Reverse direction
      rotationCount = 0;
      Serial.println(">>> Changing to CCW rotation");
      
      // Flash LEDs to indicate direction change
      for(int i = 0; i < 3; i++) {
        digitalWrite(ledPin2, HIGH);
        delay(100);
        digitalWrite(ledPin2, LOW);
        delay(100);
      }
    }
  } else if (servoAngle == 359 && step < 0) {
    rotationCount++;
    if (rotationCount >= 5) {
      step = 2;   // Reverse direction
      rotationCount = 0;
      Serial.println(">>> Changing to CW rotation");
      
      // Flash LEDs to indicate direction change
      for(int i = 0; i < 3; i++) {
        digitalWrite(ledPin3, HIGH);
        delay(100);
        digitalWrite(ledPin3, LOW);
        delay(100);
      }
    }
  }
}
