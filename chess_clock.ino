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
const char *modeStrings[] = { "5+0", "5+3", "10+3", "15+5", "Custom" };
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
  int fixedTime = 0;
  int variableTime = 0;

  void customTime() {
    // Ask for fixed time (0-99)
    fixed:
    Serial.println("Enter fixed time (0-99):");
    while (true) {
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'e') {
          break;  // Exit loop if 'e' is pressed
        } else if (inputChar == 'u' && fixedTime < 99) {
          fixedTime++;
        } else if (inputChar == 'd' && fixedTime > 0) {
          fixedTime--;
        } else if (inputChar == 'b') {
          return;
        }
        displayFixedTime();
      }
    }

    // Ask for variable time (1-30)
    Serial.println("Enter variable time (0-30):");
    while (true) {
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'e') {
          break;  // Exit loop if 'e' is pressed and variableTime is greater than 0
        } else if (inputChar == 'u' && variableTime < 30) {
          variableTime++;
        } else if (inputChar == 'd' && variableTime > 0) {
          variableTime--;
        } else if (inputChar == 'b') {
          goto fixed;
        }
        displayVariableTime();
      }
    }

    // Display or process the custom time values as needed
    Serial.print("Fixed Time: ");
    Serial.println(fixedTime);
    Serial.print("Variable Time: ");
    Serial.println(variableTime);
    
    gameMode();    
  }

  void presetTime(const String& time) {
    
    Serial.println("In preset time function");
    // Find the position of the '+' character in the string
    int plusPos = time.indexOf('+');

    // Extract the fixed time substring from the beginning of the string
    fixedTime = time.substring(0, plusPos).toInt();
    Serial.println(fixedTime);
    // Extract the variable time substring after the '+' character
    variableTime = time.substring(plusPos + 1).toInt();
    Serial.println(variableTime);
    
    gameMode();
  }

  void displayFixedTime() {
    // Display the current fixed time value
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setFont(&FreeMono9pt7b);
    display.print("Fixed Time: ");
    display.println(fixedTime);
    display.display();
  }

  void displayVariableTime() {
    // Display the current variable time value
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setFont(&FreeMono9pt7b);
    display.print("Variable Time: ");
    display.println(variableTime);
    display.display();
  }

  void gameMode() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setFont(&FreeMono9pt7b);
    display.print("Game mode");
    display.display();

    while (true) {
      if (Serial.available() > 0) {
        char inputChar = Serial.read();
        if (inputChar == 'b') {
          return;
        }
      }
    }    
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
