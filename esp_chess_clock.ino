#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum GameMode { MODE_5_0,
                MODE_5_3,
                MODE_10_3,
                MODE_15_5,
                MODE_CUSTOM };

GameMode modes[] = { MODE_5_0, MODE_5_3, MODE_10_3, MODE_15_5, MODE_CUSTOM };
const char* modeStrings[] = { "5+0", "5+3", "10+3", "15+5", "Custom" };
const int numModes = sizeof(modes) / sizeof(modes[0]);
const int maxDisplayItems = 3;

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
  int fixedT, variableT = 0;
  const unsigned long secondInMillis = 1000; // One second in milliseconds

  // set fixedT and variableT based on user input
  void customTime() {
    // Ask for fixed time (0-99)
    fixed:
    Serial.println("Enter fixed time (0-99):");
    while (true) {
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'e' && fixedT > 0) {
          break;  // Exit loop if 'e' is pressed
        } else if (inputChar == 'u' && fixedT < 99) {
          fixedT++;
        } else if (inputChar == 'd' && fixedT > 0) {
          fixedT--;
        } else if (inputChar == 'b') {
          return;
        }
        displayfixedT();
      }
    }

    // Ask for variable time (1-30)
    Serial.println("Enter variable time (0-30):");
    while (true) {
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'e') {
          break;  // Exit loop if 'e' is pressed and variableT is greater than 0
        } else if (inputChar == 'u' && variableT < 30) {
          variableT++;
        } else if (inputChar == 'd' && variableT > 0) {
          variableT--;
        } else if (inputChar == 'b') {
          goto fixed;
        }
        displayvariableT();
      }
    }

    gameMode(fixedT, variableT);
  }

  // parse presetTime mode string into fixedT and variableT variables
  void presetTime(const String& time) {

    Serial.println("In preset time function");
    // Find the position of the '+' character in the string
    int plusPos = time.indexOf('+');

    // Extract the fixed time substring from the beginning of the string
    fixedT = time.substring(0, plusPos).toInt();
    Serial.println(fixedT);
    // Extract the variable time substring after the '+' character
    variableT = time.substring(plusPos + 1).toInt();
    Serial.println(variableT);

    gameMode(fixedT, variableT);
  }

  // main game function that contains init and game loop
  void gameMode(int fixedT, int variableT) {
    // Init player times
    float p1 = fixedT;
    float p2 = fixedT;
    float bufferP; // buffer for timer function

    int moves = 2;
    bool gameActive = false;
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

      if (gameActive) {
        unsigned long currentMillis = millis(); // Get the current time

        if (p1Active) {
          bufferP = p1;
        } else {
          bufferP = p2;
        }

        // This is the clock for the active player. Check if one second has passed
        if (currentMillis - previousMillis >= secondInMillis) {
          // Update the value every second
          bufferP -= 1.0 / 60.0; // Decrease by one minute

          // Print the updated value (you can replace this with your own logic)
          Serial.print("Time remaining: ");
          Serial.print(static_cast<int>(bufferP)); // Print the integer part
          Serial.print(":");
          int seconds = static_cast<int>((bufferP - static_cast<int>(bufferP)) * 60); // Extract the seconds
          if (seconds < 10) {
            Serial.print("0"); // Add leading zero if seconds is less than 10
          }
          Serial.println(seconds);

          // Update the last time the value was updated
          previousMillis = currentMillis;

          // Check if the timer has reached 0:00
          if (bufferP <= 0.0 && seconds <= 0) {
            Serial.println("Time is up! Game over!"); // Print your message here
            gameActive = false;
          }
        }
  
        if (p1Active) {
          p1 = bufferP;
        } else {
          p2 = bufferP;
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

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  drawMenu();  // Draw the initial menu
}

// This main loop focuses on the menu navigation. Once an option was selected it will call the modeHandler which has again loops to catch input.
void loop() {
  if (Serial.available() > 0) {
    char incomingChar = Serial.read();  // Read the incoming character
    Serial.print(incomingChar);


    // Perform actions based on the received character
    switch (incomingChar) {
      case 'u':  // 'u' for up
        selectedItem = (selectedItem - 1 + numModes) % numModes;
        break;
      case 'd':  // 'd' for down
        selectedItem = (selectedItem + 1) % numModes;
        break;
      case 'e':  // 'e' for enter
        Serial.print("Selected Mode: ");
        Serial.println(selectedItem);
        modeHandler.handleMode(modes[selectedItem]);
        break;
    }

    // Update startIndex logic moved outside the switch statement
    if (selectedItem < startIndex) {
      startIndex = selectedItem;
    } else if (selectedItem >= startIndex + maxDisplayItems) {
      startIndex = selectedItem - maxDisplayItems + 1;
    }

    drawMenu();
  }
}
