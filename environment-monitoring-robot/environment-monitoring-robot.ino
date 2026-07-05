/*
 * ============================================================================
 * Arduino Uno Smart Robot: Obstacle Avoidance, Flame Detection & Data Logging
 * ============================================================================
 * 
 * PIN CONFLICT RESOLUTION & REMAPPING:
 * The original request assigned ENB to D11 and CS to D12. 
 * However, D11 is hardware MOSI and D12 is hardware MISO for the SD Card SPI.
 * To prevent critical SPI communication failures, the following safe remaps 
 * were applied automatically in the code:
 * - ENB (Motor B Enable) moved from D11 to A2. 
 *   (Note: A2 is not a hardware PWM pin, so Motor B will run at full speed. 
 *   This is standard practice when Uno PWM pins are exhausted).
 * - SD Card CS (Chip Select) moved from D12 to A3.
 * 
 * Hardware SPI pins used for SD Card: MOSI=D11, MISO=D12, SCK=D13.
 * Hardware I2C pins used for OLED: SDA=A4, SCL=A5.
 * ============================================================================
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ==========================================
// 1. PIN DEFINITIONS
// ==========================================
#define LEFT_IR_PIN     2
#define RIGHT_IR_PIN    4
#define DHT_PIN         3
#define FLAME_PIN       5
#define BUZZER_PIN      6

// Motor Driver Pins (L293D)
#define MOTOR_IN1       7
#define MOTOR_IN2       8
#define MOTOR_IN3       9
#define MOTOR_IN4       A0
#define MOTOR_ENA       10  // PWM Pin
#define MOTOR_ENB       A2  // REMAPPED from D11 to avoid SPI MOSI conflict

// Sensors & Modules
#define LDR_PIN         A1
#define SD_CS_PIN       A3  // REMAPPED from D12 to avoid SPI MISO conflict

// OLED Display Settings
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDRESS    0x3C

// DHT Sensor Type
#define DHT_TYPE        DHT22

// ==========================================
// 2. GLOBAL OBJECTS & VARIABLES
// ==========================================
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
File sdFile;

// Sensor Data Variables
float temperature = 0.0;
float humidity = 0.0;
int lightLevel = 0;
bool flameDetected = false;
bool leftObstacle = false;
bool rightObstacle = false;

// Robot State Machine
enum RobotState {
  STATE_FORWARD,
  STATE_BACKWARD,
  STATE_TURN_LEFT,
  STATE_TURN_RIGHT,
  STATE_STOP_FLAME,
  STATE_STOP_OBSTACLE
};

RobotState currentRobotState = STATE_FORWARD;
unsigned long stateStartTime = 0;
unsigned long stateDuration = 0;

// Timing Variables (for non-blocking millis() operations)
unsigned long lastDHTRead = 0;
unsigned long lastOLEDUpdate = 0;
unsigned long lastSDLog = 0;
unsigned long lastBuzzerTime = 0;

// SD Card Status
bool sdCardInitialized = false;

// ==========================================
// 3. SETUP FUNCTION
// ==========================================
void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println(F("System Initializing..."));

  // Initialize Pins
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Motor Pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_ENB, OUTPUT);

  // Set initial motor speeds (ENA is PWM, ENB is digital HIGH due to remap)
  analogWrite(MOTOR_ENA, 180); // ~70% speed
  digitalWrite(MOTOR_ENB, HIGH); // Full speed (A2 is not hardware PWM)
  stopMotors();

  // Initialize DHT22
  dht.begin();
  Serial.println(F("DHT22 Initialized."));

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 OLED allocation failed!"));
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("OLED Ready!"));
    display.display();
    Serial.println(F("OLED Initialized."));
  }

  // Initialize SD Card
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("SD Card initialization failed!"));
    sdCardInitialized = false;
  } else {
    Serial.println(F("SD Card Ready."));
    sdCardInitialized = true;
    // Create/overwrite CSV header if file doesn't exist or just append
    sdFile = SD.open("datalog.csv", FILE_WRITE);
    if (sdFile) {
      sdFile.println(F("Millis,Temperature,Humidity,LightLevel,FlameDetected,RobotState"));
      sdFile.close();
    }
  }

  // Seed random number generator for obstacle avoidance
  randomSeed(analogRead(A1)); 

  Serial.println(F("System Ready!"));
  stateStartTime = millis();
}

// ==========================================
// 4. MAIN LOOP
// ==========================================
void loop() {
  unsigned long currentMillis = millis();

  // --- A. Read Sensors (Non-blocking) ---
  readSensors(currentMillis);

  // --- B. Check Flame (Highest Priority) ---
  if (flameDetected) {
    if (currentRobotState != STATE_STOP_FLAME) {
      currentRobotState = STATE_STOP_FLAME;
      tone(BUZZER_PIN, 1500); // Continuous alarm
      Serial.println(F("ALERT: Flame Detected! Stopping."));
    }
  } else {
    if (currentRobotState == STATE_STOP_FLAME) {
      noTone(BUZZER_PIN);
      currentRobotState = STATE_FORWARD; // Resume normal operation
      Serial.println(F("Flame cleared. Resuming."));
    }
  }

  // --- C. Obstacle Avoidance State Machine ---
  if (currentRobotState != STATE_STOP_FLAME) {
    handleObstacleAvoidance(currentMillis);
  }

  // --- D. Execute Motor Commands based on State ---
  executeMotorState();

  // --- E. Update OLED Display (Every 500ms to prevent flicker) ---
  if (currentMillis - lastOLEDUpdate >= 500) {
    lastOLEDUpdate = currentMillis;
    updateOLEDDisplay();
  }

  // --- F. Log Data to SD Card (Every 2 seconds) ---
  if (currentMillis - lastSDLog >= 2000) {
    lastSDLog = currentMillis;
    logDataToSD();
  }
}

// ==========================================
// 5. FUNCTION DEFINITIONS
// ==========================================

/**
 * Reads all sensors and updates global variables.
 * Uses millis() to read DHT22 only every 2 seconds (DHT22 is slow).
 */
void readSensors(unsigned long currentMillis) {
  // Read IR Sensors (Assuming Active LOW when obstacle is detected)
  leftObstacle = (digitalRead(LEFT_IR_PIN) == LOW);
  rightObstacle = (digitalRead(RIGHT_IR_PIN) == LOW);

  // Read Flame Sensor (Assuming Active LOW for KY-026 and generic digital modules)
  flameDetected = (digitalRead(FLAME_PIN) == LOW);

  // Read LDR and map to percentage (0-100%)
  int ldrRaw = analogRead(LDR_PIN);
  lightLevel = map(ldrRaw, 0, 1023, 0, 100);

  // Read DHT22 every 2 seconds
  if (currentMillis - lastDHTRead >= 2000) {
    lastDHTRead = currentMillis;
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    // Error handling for DHT
    if (!isnan(t) && !isnan(h)) {
      temperature = t;
      humidity = h;
    } else {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
  }
}

/**
 * Handles the non-blocking state machine for obstacle avoidance.
 */
void handleObstacleAvoidance(unsigned long currentMillis) {
  bool stateFinished = (currentMillis - stateStartTime >= stateDuration);

  if (currentRobotState == STATE_FORWARD) {
    if (leftObstacle || rightObstacle) {
      // Obstacle detected while moving forward
      stopMotors();
      currentRobotState = STATE_BACKWARD;
      stateStartTime = currentMillis;
      stateDuration = 2000; // Back up for 2 seconds
      
      // Short beep alert for obstacle
      tone(BUZZER_PIN, 800, 150); 
      Serial.println(F("Obstacle detected! Reversing."));
    }
  } 
  else if (stateFinished) {
    // Current timed state has finished
    if (currentRobotState == STATE_BACKWARD) {
      stopMotors();
      delay(100); // Tiny pause for motor momentum to settle
      
      // Determine turn direction based on which sensor triggered
      if (leftObstacle && rightObstacle) {
        currentRobotState = (random(2) == 0) ? STATE_TURN_LEFT : STATE_TURN_RIGHT;
      } else if (leftObstacle) {
        currentRobotState = STATE_TURN_RIGHT; // Turn away from left obstacle
      } else {
        currentRobotState = STATE_TURN_LEFT;  // Turn away from right obstacle
      }
      
      stateStartTime = currentMillis;
      stateDuration = 1000; // Turn for 1 second
      Serial.print(F("Turning: "));
      Serial.println(currentRobotState == STATE_TURN_LEFT ? F("LEFT") : F("RIGHT"));
      
    } 
    else if (currentRobotState == STATE_TURN_LEFT || currentRobotState == STATE_TURN_RIGHT) {
      // Finished turning. Re-check sensors before moving forward to avoid repeated collisions.
      leftObstacle = (digitalRead(LEFT_IR_PIN) == LOW);
      rightObstacle = (digitalRead(RIGHT_IR_PIN) == LOW);
      
      if (!leftObstacle && !rightObstacle) {
        currentRobotState = STATE_FORWARD;
        Serial.println(F("Path clear. Moving forward."));
      } else {
        // Still blocked, back up again
        currentRobotState = STATE_BACKWARD;
        stateStartTime = currentMillis;
        stateDuration = 2000;
        Serial.println(F("Still blocked! Reversing again."));
      }
    }
  }
}

/**
 * Applies motor states based on the currentRobotState enum.
 */
void executeMotorState() {
  switch (currentRobotState) {
    case STATE_FORWARD:
      moveForward();
      break;
    case STATE_BACKWARD:
      moveBackward();
      break;
    case STATE_TURN_LEFT:
      turnLeft();
      break;
    case STATE_TURN_RIGHT:
      turnRight();
      break;
    case STATE_STOP_FLAME:
    case STATE_STOP_OBSTACLE:
      stopMotors();
      break;
  }
}

// --- Motor Control Functions ---
void moveForward() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
}

void stopMotors() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

/**
 * Updates the OLED display with current sensor data and robot state.
 */
void updateOLEDDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  
  display.print(F("T:"));
  display.print(temperature, 1);
  display.print(F("C H:"));
  display.print(humidity, 0);
  display.print(F("%"));

  display.setCursor(0, 12);
  display.print(F("Light: "));
  display.print(lightLevel);
  display.print(F("%"));

  display.setCursor(0, 24);
  display.print(F("Flame: "));
  display.print(flameDetected ? F("YES!") : F("No"));

  display.setCursor(0, 36);
  display.print(F("State: "));
  switch (currentRobotState) {
    case STATE_FORWARD: display.print(F("Forward")); break;
    case STATE_BACKWARD: display.print(F("Reverse")); break;
    case STATE_TURN_LEFT: display.print(F("Turn L")); break;
    case STATE_TURN_RIGHT: display.print(F("Turn R")); break;
    case STATE_STOP_FLAME: display.print(F("STOP FIRE")); break;
    case STATE_STOP_OBSTACLE: display.print(F("Stopped")); break;
  }

  display.setCursor(0, 48);
  display.print(F("IR-L:"));
  display.print(leftObstacle ? F("X") : F("-"));
  display.print(F(" IR-R:"));
  display.print(rightObstacle ? F("X") : F("-"));

  display.display();
}

/**
 * Logs sensor data to the SD card in CSV format.
 * Includes error handling for SD write failures.
 */
void logDataToSD() {
  if (!sdCardInitialized) return;

  sdFile = SD.open("datalog.csv", FILE_WRITE);
  if (sdFile) {
    sdFile.print(millis()); sdFile.print(F(","));
    sdFile.print(temperature, 2); sdFile.print(F(","));
    sdFile.print(humidity, 2); sdFile.print(F(","));
    sdFile.print(lightLevel); sdFile.print(F(","));
    sdFile.print(flameDetected ? 1 : 0); sdFile.print(F(","));
    
    // Write state as string
    switch (currentRobotState) {
      case STATE_FORWARD: sdFile.print(F("Forward")); break;
      case STATE_BACKWARD: sdFile.print(F("Reverse")); break;
      case STATE_TURN_LEFT: sdFile.print(F("Turn_L")); break;
      case STATE_TURN_RIGHT: sdFile.print(F("Turn_R")); break;
      case STATE_STOP_FLAME: sdFile.print(F("Stop_Flame")); break;
      case STATE_STOP_OBSTACLE: sdFile.print(F("Stop_Obs")); break;
    }
    sdFile.println();
    sdFile.close();
  } else {
    Serial.println(F("Error opening datalog.csv for writing!"));
  }
}