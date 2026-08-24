#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD display (I2C address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize Servo Motor
Servo gateServo;

// Pin Definitions
const int irEntryPin = 2;   // IR Sensor at Entry Gate
const int irExitPin  = 3;   // IR Sensor at Exit Gate
const int servoPin   = 9;   // Servo Motor Pin

// System Variables
int totalSlots = 4;         // Total parking capacity
int availableSlots = 4;     // Currently available spots

bool entryHandled = false;
bool exitHandled  = false;

void setup() {
  // Pin Configurations
  pinMode(irEntryPin, INPUT);
  pinMode(irExitPin, INPUT);
  
  // Servo Initialization
  gateServo.attach(servoPin);
  gateServo.write(0); // Gate Closed Position (0 degrees)

  // LCD Initialization
  lcd.init();
  lcd.backlight();
  
  // Welcome Screen
  lcd.setCursor(0, 0);
  lcd.print(" Smart Parking ");
  lcd.setCursor(0, 1);
  lcd.print(" System Ready! ");
  delay(2000);
  lcd.clear();

  updateDisplay();
}

void loop() {
  int entryStatus = digitalRead(irEntryPin);
  int exitStatus  = digitalRead(irExitPin);

  // Vehicle Entry Logic (Active LOW IR Sensor)
  if (entryStatus == LOW && !entryHandled) {
    if (availableSlots > 0) {
      availableSlots--;
      updateDisplay();
      openGate();
      entryHandled = true;
    } else {
      displayFullWarning();
    }
  }

  if (entryStatus == HIGH) {
    entryHandled = false;
  }

  // Vehicle Exit Logic
  if (exitStatus == LOW && !exitHandled) {
    if (availableSlots < totalSlots) {
      availableSlots++;
      updateDisplay();
      openGate();
      exitHandled = true;
    }
  }

  if (exitStatus == HIGH) {
    exitHandled = false;
  }

  delay(100);
}

// Function to open and automatically close the gate bar
void openGate() {
  gateServo.write(90); // Open Gate
  delay(3000);         // Wait 3 seconds for vehicle to pass
  gateServo.write(0);  // Close Gate
}

// Function to update LCD screen with current slots
void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Park");
  lcd.setCursor(0, 1);
  lcd.print("Slots Left: ");
  lcd.print(availableSlots);
}

// Display warning when parking is completely full
void displayFullWarning() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Parking FULL! ");
  lcd.setCursor(0, 1);
  lcd.print(" Please Wait... ");
  delay(2000);
  updateDisplay();
}
