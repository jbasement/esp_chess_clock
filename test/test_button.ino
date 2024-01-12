uint8_t buttonPin = D3;

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Set the button pin as an input with pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("program gestartet");
}

void loop() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed
  if (buttonState == LOW) {
    // Print a message to the serial console
    Serial.println("Button pressed");

    // Add a delay to avoid rapid multiple detections
    delay(1000);
  }
}