/*
  Open Droid Lab - PIR + Serial (Production Version)
  Uses non-blocking millis() edge detection
  and sends clean state changes over serial
  for the ROS2 / Raspberry Pi side.
*/

#define PIR_PIN 4

bool lastState = false;
unsigned long lastChange = 0;

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(115200);
  Serial.println("Open Droid Lab - PIR millis + serial ready");
}

void loop() {
  bool current = digitalRead(PIR_PIN);

  if (current != lastState) {
    lastChange = millis();
    lastState = current;

    // Send a clean, simple message the ROS2 node can parse
    if (current) {
      Serial.println("MOTION:1");
    } else {
      Serial.println("MOTION:0");
    }
  }
}
