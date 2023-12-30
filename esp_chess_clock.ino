#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#include <TM1637Display.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define CLK_PIN D7
#define DIO_PIN D0

#define CLK_PIN2 3
#define DIO_PIN2 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TM1637Display timer(CLK_PIN, DIO_PIN);
TM1637Display timer2(CLK_PIN2, DIO_PIN2);

// Pin setup
uint8_t buttonBackPin = D3;
uint8_t buttonUpPin = D5;
uint8_t buttonDownPin = D4;
uint8_t buttonEnterPin = D6;

// game modes
enum GameMode { MODE_5_0,
                MODE_5_3,
                MODE_10_3,
                MODE_15_5,
                MODE_CUSTOM };
GameMode modes[] = { MODE_5_0, MODE_5_3, MODE_10_3, MODE_15_5, MODE_CUSTOM };
const char* modeStrings[] = { "5+0", "5+3", "10+3", "15+5", "Custom" };
const int numModes = sizeof(modes) / sizeof(modes[0]);
const int maxDisplayItems = 3;

// menu vars
int selectedItem = 0;
int startIndex = 0;
void drawMenu();

class ModeHandler {
public:
  void handleMode(GameMode mode) {
    switch (mode) {
      case MODE_CUSTOM:
        customTime();
        break;
      default:
        presetTime(modeStrings[mode]);
    }
    drawMenu();
  }

private:
  int fixedT = 1;
  int variableT = 0;
  const unsigned long secondInMillis = 1000; // One second in milliseconds

  // set fixedT and variableT based on user input
  void customTime() {
    delay(100);
    // Ask for fixed time (0-99)
    fixed:
    Serial.println("Enter fixed time (0-99):");
    while (true) {
      if (digitalRead(buttonEnterPin) == LOW && fixedT > 0) {
        break;  // Exit loop if 'e' is pressed
      } else if (digitalRead(buttonUpPin) == LOW && fixedT < 99) {
        fixedT++;
      } else if (digitalRead(buttonDownPin) == LOW && fixedT > 1) {
        fixedT--;
      } else if (digitalRead(buttonBackPin) == LOW) {
        return;
      }
      displayfixedT();
      delay(100);  // Add a small delay
    }

    delay(100);
    // Ask for variable time (0-30)
    Serial.println("Enter variable time (0-30):");
    while (true) {
      if (digitalRead(buttonEnterPin) == LOW) {
        break;  // Exit loop if 'e' is pressed and variableT is greater than 0
      } else if (digitalRead(buttonUpPin) == LOW && variableT < 30) {
        variableT++;
      } else if (digitalRead(buttonDownPin) == LOW && variableT > 0) {
        variableT--;
      } else if (digitalRead(buttonBackPin) == LOW) {
        goto fixed;
      }
      displayvariableT();
      delay(100);  // Add a small delay
    }

    gameMode(fixedT, variableT);
  }

  // parse presetTime mode string into fixedT and variableT variables
  void presetTime(const String& time) {
    // Find the position of the '+' character in the string
    int plusPos = time.indexOf('+');

    // Extract the fixed time substring from the beginning of the string
    fixedT = time.substring(0, plusPos).toInt();
    Serial.println(fixedT);
    // Extract the variable time substring after the '+' character
    variableT = time.substring(plusPos + 1).toInt();

    gameMode(fixedT, variableT);
  }

  // main game function that contains init and game loop
  void gameMode(int fixedT, int variableT) {
    // Init player times
    float p1 = fixedT;
    float p2 = fixedT;
    float bufferP; // buffer for timer function

    int moves = 2;  // Calculate moves and start at 2 so moves can be calculated using (moves/2)
    bool gameActive = false; // used to check if clock is running
    bool p1Active = true; // instead of assuming that p1 is active add a check function here first
    unsigned long previousMillis = 0; // Variable to store the last time the value was updated

    // main game loop
    while (true) {
      // Display layout init
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.setFont(&FreeMono9pt7b);
      display.println("Game mode");
      display.print(fixedT);
      display.print("+");
      display.println(variableT);
      display.print("Moves: ");
      display.print(moves/2);
      display.display();   

      // Show time on 7-bit display
      int p1Min = static_cast<int>(p1);
      int p1Sec = static_cast<int>((p1 - p1Min) * 60);      
      int p2Min = static_cast<int>(p2);
      int p2Sec = static_cast<int>((p2 - p2Min) * 60);         
      timer.showNumberDecEx(p1Min * 100 + p1Sec, 0b11100000, true);
      timer2.showNumberDecEx(p2Min * 100 + p2Sec, 0b11100000, true);

      if (gameActive) {
        if (p1Active) {
          updatePlayerTime(p1, gameActive, previousMillis);
        } else {
          updatePlayerTime(p2, gameActive, previousMillis);
        }
      }
     
      // menu controls
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'b') { 
          return;
        } else if (!gameActive && inputChar == 's') { // replace 's' with magnetSensor. That indicates first move.     // Check which player has first move => Is magnet sensor activated or deactivated?
          gameActive = true;
        } else if (inputChar == 'e') { // start&stop game by pressing enter, by flipping gameActive bool
          gameActive = !gameActive;
        }  else if (inputChar == 'c') { // Check for 'c' input to switch players and increment time
          if (p1Active) {
            p1 += static_cast<float>(variableT) / 60.0;
          } else {
            p2 += static_cast<float>(variableT) / 60.0; 
          }
          p1Active = !p1Active;
          moves++;
        }
      }

      if (digitalRead(buttonBackPin) == LOW) {
        return;
      } else if (digitalRead(buttonEnterPin) == LOW) {
        // start & stop game by pressing enter, by flipping gameActive bool
        gameActive = !gameActive;
      } else if (digitalRead(buttonDownPin) == LOW) { // Assuming 'c' is mapped to the Down button
        // Check for 'c' input to switch players and increment time
        if (p1Active) {
          p1 += static_cast<float>(variableT) / 60.0;
        } else {
          p2 += static_cast<float>(variableT) / 60.0; 
        }
        p1Active = !p1Active;
        moves++;
      }
      delay(100);  // Add a small delay
    }
  }

  void updatePlayerTime(float& playerTime, bool& gameActive, unsigned long& previousMillis) {
    unsigned long currentMillis = millis(); // Get the current time

    // This is the clock for the active player. Check if one second has passed
    if (currentMillis - previousMillis >= secondInMillis) {
      // Update the value every second
      playerTime -= 1.0 / 60.0; // Decrease by one minute

      // Print the updated value (you can replace this with your own logic)
      Serial.print("Time remaining: ");
      Serial.print(static_cast<int>(playerTime)); // Print the integer part
      Serial.print(":");
      int seconds = static_cast<int>((playerTime - static_cast<int>(playerTime)) * 60); // Extract the seconds
      if (seconds < 10) {
        Serial.print("0"); // Add leading zero if seconds is less than 10
      }
      Serial.println(seconds);

      // Update the last time the value was updated
      previousMillis = currentMillis;

      // Check if the timer has reached 0:00
      if (playerTime <= 0.0 && seconds <= 0) {
        Serial.println("Time is up! Game over!"); 
        gameActive = false;
      }
    }
  }

  void displayfixedT() {
    // Display the current fixed time value
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setFont(&FreeMono9pt7b);
    display.print("Fixed Time: ");
    display.println(fixedT);
    display.display();
  }

  void displayvariableT() {
    // Display the current variable time value
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setFont(&FreeMono9pt7b);
    display.print("Variable Time: ");
    display.println(variableT);
    display.display();
  }

};

ModeHandler modeHandler;

void drawMenu() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.setFont(&FreeMono9pt7b);
  display.println("Select mode:");

  timer.showNumberDecEx(0 * 100 + 0, 0b11100000, true);
  timer2.showNumberDecEx(0 * 100 + 0, 0b11100000, true);  

  for (int i = 0; i < maxDisplayItems; i++) {
    int index = (startIndex + i) % numModes;
    if (index == selectedItem) {
      display.println("> " + String(modeStrings[index]));  // Display the selected mode with ">"
    } else {
      display.println("  " + String(modeStrings[index]));  // Display other modes without ">"
    }
  }

  display.display();
}

void setup() {
  Serial.begin(115200);

  // Init display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  // Init buttons
  pinMode(buttonBackPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonEnterPin, INPUT_PULLUP);

  // init RX and TX pins
  //GPIO 1 (TX) swap the pin to a GPIO.
  pinMode(1, FUNCTION_3); 
  //GPIO 3 (RX) swap the pin to a GPIO.
  pinMode(3, FUNCTION_3); 

  // Initialize the timer
  timer.setBrightness(0x0a); 
  timer2.setBrightness(0x0a);

  delay(2000);
  drawMenu();  // Draw the initial menu
}

// This main loop focuses on the menu navigation. Once an option was selected it will call the modeHandler which has again loops to catch input.
void loop() {
  if (digitalRead(buttonUpPin) == LOW) {
    selectedItem = (selectedItem - 1 + numModes) % numModes;
  } else if (digitalRead(buttonDownPin) == LOW) {
    selectedItem = (selectedItem + 1) % numModes;
  } else if (digitalRead(buttonEnterPin) == LOW) {
    Serial.print("Selected Mode: ");
    Serial.println(selectedItem);
    modeHandler.handleMode(modes[selectedItem]);
  }

  // Update startIndex logic moved outside the if statements
  if (selectedItem < startIndex) {
    startIndex = selectedItem;
  } else if (selectedItem >= startIndex + maxDisplayItems) {
    startIndex = selectedItem - maxDisplayItems + 1;
  }

  drawMenu();
  delay(100);
}
