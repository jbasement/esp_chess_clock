const int reedSwitchPin = D8;

void setup() {
  Serial.begin(115200);
  pinMode(reedSwitchPin, INPUT_PULLUP);
}

void loop() {
  int reedSwitchState = digitalRead(reedSwitchPin);

  if (reedSwitchState == LOW) {
    Serial.println("Reed-switch active!");
  } else {
    Serial.println("Reed-swtich inactive.");
  }

  delay(500);
}
