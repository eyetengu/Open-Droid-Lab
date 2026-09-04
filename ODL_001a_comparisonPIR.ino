/*
  Open Droid Lab - PIR Comparison Sketch
  Demonstrates three ways to read a PIR sensor:
  1. Basic polling
  2. Non-blocking millis() edge detection
  3. Hardware interrupt

  Change the METHOD define below to switch between them.
*/

#define PIR_PIN 4          // Change to your actual pin
#define METHOD 2           // 1 = Basic, 2 = Millis, 3 = Interrupt

// Shared state
volatile bool motionDetected = false;   // used by interrupt method
bool lastState = false;
unsigned long lastChange = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(115200);

  if (METHOD == 3) {
    attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, CHANGE);
  }

  Serial.println("PIR Comparison Sketch ready");
  Serial.print("Active method: ");
  Serial.println(METHOD);
}

void loop() {
  if (METHOD == 1) {
    readPIR_Basic();
  } 
  else if (METHOD == 2) {
    readPIR_Millis();
  } 
  else if (METHOD == 3) {
    readPIR_Interrupt();
  }
}

// -------------------------------------------------
// Method 1: Basic polling
// -------------------------------------------------
void readPIR_Basic() {
  bool current = digitalRead(PIR_PIN);
  Serial.println(current ? "Motion" : "No motion");
  delay(200);   // simple delay for readability
}

// -------------------------------------------------
// Method 2: Non-blocking millis() edge detection
// -------------------------------------------------
void readPIR_Millis() {
  bool current = digitalRead(PIR_PIN);

  if (current != lastState) {
    lastChange = millis();
    lastState = current;

    if (current) {
      Serial.println("Motion STARTED");
    } else {
      Serial.println("Motion ENDED");
    }
  }
  // No delay — loop stays free
}

// -------------------------------------------------
// Method 3: Hardware interrupt
// -------------------------------------------------
void IRAM_ATTR pirISR() {          // IRAM_ATTR needed on ESP32
  motionDetected = digitalRead(PIR_PIN);
}

void readPIR_Interrupt() {
  static bool lastReported = false;

  if (motionDetected != lastReported) {
    lastReported = motionDetected;
    Serial.println(motionDetected ? "Motion (ISR)" : "No motion (ISR)");
  }
}
