#include <TM1637Display.h>

// Define the pins for TM1637
#define CLK_PIN D7
#define DIO_PIN D0

#define CLK_PIN2 3
#define DIO_PIN2 1

TM1637Display timer(CLK_PIN, DIO_PIN);
TM1637Display timer2(CLK_PIN2, DIO_PIN2);

void setup() {
  Serial.begin(115200);

  //GPIO 1 (TX) swap the pin to a GPIO.
  pinMode(1, FUNCTION_3); 
  //GPIO 3 (RX) swap the pin to a GPIO.
  pinMode(3, FUNCTION_3); 

  // Initialize the timer
  timer.setBrightness(0x0a);  // Adjust brightness if needed
  timer.showNumberDec(0, false);  // Display initial value
  timer2.setBrightness(0x0a);  // Adjust brightness if needed
  timer2.showNumberDec(0, false);  // Display initial value
}

void loop() {
  static int counter = 0;

  // Display the counter value on the 7-segment timer
  timer.showNumberDec(counter, false);
  timer2.showNumberDec(counter, false);

  // Increment the counter
  counter++;

  // Wait for a short duration before updating the timer
  delay(1000);  // Adjust the delay according to your needs
}
