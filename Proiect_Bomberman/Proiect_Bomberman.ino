#include "LedControl.h" //  need the library
#include<LiquidCrystal.h>
#include<EEPROM.h>

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int RS = 7;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int xPin = A0;
const int yPin = A1;
const int swPin = 8;


// settings variables

const byte settingsArrowArraySize = 3;
bool settingsArrowArray[settingsArrowArraySize] = {
  true, false, false
};

char* settings[settingsArrowArraySize] = {
  "BACK TO MENU",
  "LUMINOSITY",
  "CONTRAST"
};



// highscores variables
char* top[9];
bool highscoresChanged = false;
byte xHighscoresArrowPos = 0;
byte xHighscoresArrowLastPos = 0;
const byte highscoresArrowArraySize = 13;

bool highscoresArrowArray[highscoresArrowArraySize] = {
  true, false, false, false, false, false, false, false, false, false, false, false, false
};

char* highscores[highscoresArrowArraySize] = {
  "BACK TO MENU",
  "GODE MODE:",
  top[0], top[1], top[2],
  "HARD:",
  top[3], top[4], top[5],
  "NORMAL:",
  top[6], top[7], top[8]
};


// about variables
byte xAboutArrowPos = 0;
byte xAboutArrowLastPos = 0;
const byte aboutArrowArraySize = 10;
bool aboutChanged = false;

bool aboutArrowArray[aboutArrowArraySize] = {
  true, false, false, false, false, false, false, false, false, false
};


char* about[aboutArrowArraySize] = {
    "BACK TO MENU",
    "X  BOMBERMAN  X",
    "X  BROUGHT TO X",
    "X    YOU BY   X",
    "SLEEPY STUDIOS",
    "---------------",
    "PERSONAL GITHUB",
    "LINK:",
    "github.com/",
    "stefanpascu"
  };

// plot variables
bool plotChanged = false;
byte xPlotArrowPos = 0;
byte xPlotArrowLastPos = 0;
const byte plotArrowArraySize = 8;

bool plotArrowArray[plotArrowArraySize] = {
  true, false, false, false, false, false, false, false
};

char* plot[plotArrowArraySize] = {
    "BACK TO MENU",      
    "Fereste-te de  ",
    "de bombele",
    "(ledurile ",
    "aprinse) pe ",
    "care le-ai ", 
    "aruncat singur",
    "in aer!"
  };

// menu variables
const byte menuArrowArraySize = 5;
const byte noOfLcdRows = 2;
byte xMenuArrowLastPos = 0;
byte xMenuArrowPos = 0;
bool upOrDown = true;
bool menuChanged = false;
const byte noOfDiff = 6;

bool changeNameChanged = false;
char* playerName[5];
byte playerNameCharIndex[5];

char* alphabet[26] = {
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};
bool menuArrowArray[menuArrowArraySize] = {
  false, false, false, false, false
};

char* menuList[menuArrowArraySize] = {
  "START GAME", "HIGHSCORES", "PLOT", "SETTINGS", "ABOUT"
};

char* difficulties[noOfDiff] = {
  "SUPER EASY", "EASY", "NORMAL", "HARD", "GOD MODE", "GOD MODE"
};

const byte noOfStartDifRows = 2;
byte menuState = 0;
bool reading = HIGH;
bool previousReading = HIGH;
bool lastDebounceTime = HIGH;
int debounceDelay = 50;
bool buttonState = LOW;
const int numMenuItems = sizeof menuList / sizeof *menuList;
const int numLcdRows = 2;

const int noOfTryings = 50;
const int minThreshold = 200;
const int maxThreshold = 600;
const int rows = 8;
const int cols = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
 
const int n = 8;

byte lastDifficulty = 2;
bool difficultyChanged = false;
int difficulty = 2;  // 0 - Super Easy ; 1 - Easy ; 2 - Normal ; 3 - Hard ; 4 - God Mode 
byte matrixBrightness = 2;
bool testMenuOrGame = true;

byte hearts = 3;
byte raiseHeartsNumber = 0;
byte heartRaiseThreshold = 15;

byte max2 = 0;
byte max3 = 0;
byte max4 = 0;
byte max5 = 0;
byte max6 = 0;
byte max7 = 0;

const long levelInterval = 599999;
const byte moveInterval = 200;
unsigned long long lastMoved = 0;

//bool testLevelChange = false;
bool testBeginTurnedOffLed = true;
bool firstTestOnLightedLed = true;
bool onLightedLed;
long lineBegin;
long colBegin;
bool availabilityTest = true;
int noOfFailedTryings; 
bool joyMovedX = false;
bool joyMovedY = false;
int yValue;
int xValue;
int xPos = 0;
int yPos = 0; 
byte xLastPos = 0;
byte yLastPos = 0;
byte randNumber;

long counterLevelDuration = 0;

bool matrixChanged = true;
const byte matrixSize = 8;
byte matrix[matrixSize][matrixSize] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

byte noOfMoves[4] = {
  8, 8, 8, 8
};

byte formsArray[][8] = {
 {B00000,  // heart               0
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000},
  
 {B00010,  // bomb                1
  B00111,
  B01000,
  B00100,
  B01110,
  B10001,
  B10001,
  B01110},

 {B00000,  // star                2
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000},

 {B01000,  // menu hover arrow    3
  B01100,
  B01110,
  B01111,
  B01110,
  B01100,
  B01000,
  B00000},

 {B00100,  // up + down arrows    4
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100},

 {B00000,  // up arrow            5
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000},

 {B00000,  // down arrow          6
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000},

 {B00000,  // empty cell          7
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000}
};



void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  randomSeed(analogRead(12));
  matrix[xPos][yPos] = 1;
  menuArrowArray[xMenuArrowPos] = true;
  
  lcd.begin(16, 2);
  prepareSetupOnStart();
  
  Serial.begin(9600);  
}



void loop() {
  if (testMenuOrGame == true) {
    lc.clearDisplay(0);
    switch(menuState) {
      case 0:
        if (millis() - lastMoved > moveInterval) {
        // game logic
          updateMenuArrow();
          lastMoved = millis();
        }
        if (menuChanged == true) {
          // matrix display logic
           updateMenuDisplay();
           menuChanged = false;
        }
        break;
        
      case 1:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateDifficulty();
          lastMoved = millis();
        }
        if (difficultyChanged == true) {
          // matrix display logic
          updateStartGameDifDisplay();
          difficultyChanged = false;
        }
        break;

      case 2:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          //FUNCTION FOR SETTINGS
          lastMoved = millis();
        }
        if (changeNameChanged == true) {
          // matrix display logic
          updateStartGameChangeNameDisplay();
          changeNameChanged = false;
        }
        break;

      case 3:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateHighscoresArrow();
          lastMoved = millis();
        }
        if (highscoresChanged == true) {
          // matrix display logic
          updateHighscoresDisplay();
          highscoresChanged = false;
        }
        break;

      case 4:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updatePlotArrow();
          lastMoved = millis();
        }
        if (plotChanged == true) {
          // matrix display logic
          updatePlotDisplay();
          plotChanged = false;
        }
        break;

      case 6:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateAboutArrow();
          lastMoved = millis();
        }
        if (aboutChanged == true) {
          // matrix display logic
          updateAboutDisplay();
          aboutChanged = false;
        }
        break;

      case 7:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HEARTS: ");
        lcd.setCursor(8, 0);
        lcd.print(hearts);
        lcd.setCursor(0, 1);
        lcd.print("NO OF MOVES: ???");
        testMenuOrGame = false;
        break;

      default:
        break;
    }
    
  } else {
    counterLevelDuration++;
    
    if (counterLevelDuration >= levelInterval) {
      ////////
      lc.clearDisplay(0);
      Serial.println("-----");
      Serial.println(hearts);
      Serial.println("-----");
      if (onLightedLed == true) {
        hearts--;              // jucatorul pierde o viata
        raiseHeartsNumber = 0;  // resetam nr de runde castigate fara oprire
        sadFace();
      } else {
        raiseHeartsNumber++;
        happyFace();
      }
      if (raiseHeartsNumber == heartRaiseThreshold) {
        hearts++;
      }
      if (hearts == 0) {
        lcd.clear();
        updateMenuDisplay();
        menuState = 0;
        testMenuOrGame = true;
        deadFace();
      }
      lc.clearDisplay(0);
      ////////
      
      switch (difficulty) {
        case 0:  // Super Easy (Tutorial)
          randNumber = random(2,4);   // random(2, 8);
          max2 = 6;
          max3 = 2;
          break;
        case 1:  // Easy
          randNumber = random(2,5);   // random(2, 8);
          max2 = 6;
          max3 = 4;
          break;
          
        case 2:  // Normal
          randNumber = random(2, 8);;   // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;
          
        case 3:  // Hard
          randNumber = random(2,8);   // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;
          
        default:  // God Mode
          randNumber = random(2,4);   // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;

      }

      for (int i = 0; i <= 3; i++) {
        noOfMoves[i] += 4;
//        zeroMoves[i] = false;
      }
      
      counterLevelDuration = 0;
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          matrix[i][j] = 0;
        }
      }     
      
      onLightedLed = false;
      while (randNumber != 1) {
        switch (randNumber) {
          case 2:
            
            for (int counter = 1; counter <= max2; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 7);
                  colBegin = random(0, 7);
                  for(int testLin = lineBegin; testLin <= lineBegin + 1; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 1; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 1; row++) {
                      for(int col = colBegin; col <= colBegin + 1; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            randNumber = 1;  
            availabilityTest = true;
            break;
          case 3:
            
            for (int counter = 1; counter <= max3; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 6);
                  colBegin = random(0, 6);
                  
                  for(int testLin = lineBegin; testLin <= lineBegin + 2; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 2; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 2; row++) {
                      for(int col = colBegin; col <= colBegin + 2; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            randNumber = 2;
            availabilityTest = true;
            break;
            
          case 4:
            for (int counter = 1; counter <= max4; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 5);
                  colBegin = random(0, 5);
                  for(int testLin = lineBegin; testLin <= lineBegin + 3; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 3; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 3; row++) {
                      for(int col = colBegin; col <= colBegin + 3; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            randNumber = 3;
            availabilityTest = true;
            break;
            
          case 5:
            for (int counter = 1; counter <= max5; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 4);
                  colBegin = random(0, 4);
                  for(int testLin = lineBegin; testLin <= lineBegin + 4; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 4; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 4; row++) {
                      for(int col = colBegin; col <= colBegin + 4; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            randNumber = 3;
            break;
            
          case 6:
            for (int counter = 1; counter <= max6; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 3);
                  colBegin = random(0, 3);
                  for(int testLin = lineBegin; testLin <= lineBegin + 5; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 5; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 5; row++) {
                      for(int col = colBegin; col <= colBegin + 5; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            availabilityTest = true;
            randNumber = 2;
            break;
            
          default:  // case 7:
            for (int counter = 1; counter <= max7; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  lineBegin = random(0, 2);
                  colBegin = random(0, 2);
                  for(int testLin = lineBegin; testLin <= lineBegin + 6; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 6; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false) {
                    noOfFailedTryings++;
                  } else {
                    for(int row = lineBegin; row <= lineBegin + 6; row++) {
                      for(int col = colBegin; col <= colBegin + 6; col++) {
                        if (xPos == row && yPos == col) {
                          onLightedLed = true;
                        }
                        matrix[row][col] = 1;
                      }
                    }
                  }
                  
                }
              }
            }
            availabilityTest = true;
            randNumber = 1;
            break;
        }
  
        updateMatrix();
      }
    }
      
    
    ///////////////////////////////////////////////////////
    
    
    if (millis() - lastMoved > moveInterval) {
     // game logic
       updatePositions();
       lastMoved = millis();
     }
    
     if (matrixChanged == true) {
      // matrix display logic
       updateMatrix();
       matrixChanged = false;
     }
  }
}





void updateHighscoresDisplay() {
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[7]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(highscores[xHighscoresArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
      lcd.print(highscores[xHighscoresArrowPos - row]);
    }
  } 
}


void updateHighscoresArrow() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        if (xHighscoresArrowPos == 0) {
          menuState = 0;
          updateMenuDisplay();
        }
      } 
    }   
  }
  
  previousReading = reading;

  xHighscoresArrowLastPos = xHighscoresArrowPos;
  if (xValue > minThreshold) {
    if (xHighscoresArrowPos < highscoresArrowArraySize - 1) {
      xHighscoresArrowPos++;
    } 
    else {
      xHighscoresArrowPos = 0;
    }
    
  }

  if (xValue < maxThreshold) {
    if (xHighscoresArrowPos > 0) {
      xHighscoresArrowPos--;
    }
    else {
      xHighscoresArrowPos = highscoresArrowArraySize - 1;
    }
  }

  if (xHighscoresArrowPos != xHighscoresArrowLastPos) {
    highscoresChanged = true;
    if (xHighscoresArrowPos > xHighscoresArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    highscoresArrowArray[xHighscoresArrowLastPos] = false;
    highscoresArrowArray[xHighscoresArrowPos] = true;
    
  }
  
}



void updateAboutDisplay() {
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[7]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(about[xAboutArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
      lcd.print(about[xAboutArrowPos - row]);
    }
  } 
}

void updateAboutArrow() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        if (xAboutArrowPos == 0) {
          menuState = 0;
          upOrDown = false;
          updateMenuDisplay();
        }
      } 
    }   
  }
  
  previousReading = reading;

  
  xAboutArrowLastPos = xAboutArrowPos;
  if (xValue > minThreshold) {
    if (xAboutArrowPos < aboutArrowArraySize - 1) {
      xAboutArrowPos++;
    } 
    else {
      xAboutArrowPos = 0;
    }
    
  }

  if (xValue < maxThreshold) {
    if (xAboutArrowPos > 0) {
      xAboutArrowPos--;
    }
    else {
      xAboutArrowPos = aboutArrowArraySize - 1;
    }
  }

  if (xAboutArrowPos != xAboutArrowLastPos) {
    aboutChanged = true;
    if (xAboutArrowPos > xAboutArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    aboutArrowArray[xAboutArrowLastPos] = false;
    aboutArrowArray[xAboutArrowPos] = true;
    
  }
  
}




void updatePlotArrow() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        if (xPlotArrowPos == 0) {
          menuState = 0;
          updateMenuDisplay();
        }
      } 
    }   
  }
  
  previousReading = reading;

  
  xPlotArrowLastPos = xPlotArrowPos;
  if (xValue > minThreshold) {
    if (xPlotArrowPos < plotArrowArraySize - 1) {
      xPlotArrowPos++;
    } 
    else {
      xPlotArrowPos = 0;
    }
    
  }

  if (xValue < maxThreshold) {
    if (xPlotArrowPos > 0) {
      xPlotArrowPos--;
    }
    else {
      xPlotArrowPos = plotArrowArraySize - 1;
    }
  }

  if (xPlotArrowPos != xPlotArrowLastPos) {
    plotChanged = true;
    if (xPlotArrowPos > xPlotArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    plotArrowArray[xPlotArrowLastPos] = false;
    plotArrowArray[xPlotArrowPos] = true;
    
  }
  
}

void updatePlotDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[7]);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));

  lcd.setCursor(1, 0);
  lcd.print("BACK TO MENU");
  
  lcd.setCursor(1, 1);
  lcd.print(plot[1]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(plot[xPlotArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
      lcd.print(plot[xPlotArrowPos - row]);
    }
  } 
}

void prepareSetupOnStart() {
  lcd.createChar(0, formsArray[2]);
  lcd.createChar(1, formsArray[1]);
  
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("  WELCOME TO  ");
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.print("  BOMBERMAN");
  lcd.write(byte(1));
  lcd.print("  ");
  lcd.write(byte(0));

  delay(1000);

  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("  CREATED BY  ");
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
//  lcd.write(byte(0));
  lcd.print(" SLEEPY STUDIOS ");

  delay(1000);
  
  lcd.clear();
  enterMenu();
}

void enterMenu() {
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[5]);
  lcd.createChar(2, formsArray[6]);
  upOrDown = true;
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("START GAME");
  lcd.setCursor(1, 1);
  lcd.print("HIGHSCORES");
  lcd.setCursor(15, 1);
  lcd.write(byte(2));
}


void updateMenuDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[7]);
//  Serial.println(xMenuArrowPos);
  if (upOrDown == true) {
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 1);
    lcd.write(byte(1));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      Serial.println(menuList[xMenuArrowPos + row]);
      lcd.print(menuList[xMenuArrowPos + row]);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
      Serial.println(menuList[xMenuArrowPos - row]);
      lcd.print(menuList[xMenuArrowPos - row]);
    }
  } 
}


void updateStartGameDifDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[4]);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  
  lcd.setCursor(1, 0);
  lcd.print("SET DIFFICULTY");

  lcd.setCursor(1, 1);
  Serial.println(difficulties[difficulty]);
  lcd.print(difficulties[difficulty]);
}

void updateStartGameChangeNameDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[3]);
  
  lcd.setCursor(0, 0);
  lcd.print("    SET NAME    ");

  lcd.setCursor(2, 1);
  lcd.write(byte(0));
  
}

void updateDifficulty() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        //////////////////////////////////////////////////////////////////////
        upOrDown = true;
//        updateStartGameChangeNameDisplay();
        menuState = 7;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastDifficulty = difficulty;
  
  if (xValue < maxThreshold) {
    if (difficulty < 5) {
      difficulty++;
    } 
  }

  if (xValue > minThreshold) {
    if (difficulty > 0) {
      difficulty--;
    }
  }

  if (difficulty != lastDifficulty) {
    difficultyChanged = true;
//    updateStartGameDifDisplay();
  }
  
}


void updateMenuArrow() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        switch(xMenuArrowPos) {
          case 0:
            upOrDown = true;
            menuState = 1;
            updateStartGameDifDisplay();
            break;

          case 1:
            upOrDown = true;
            // Highscores function
            menuState = 3;
            updateHighscoresDisplay();
            break;
            
          case 2:
            upOrDown = true;
            // plot function
            menuState = 4;
            updatePlotDisplay();
            break;

          case 3:
            upOrDown = true;
            // settings function
            menuState = 5;
            break;

          case 4:
            upOrDown = true;
            // about function
            menuState = 6;
            updateAboutDisplay();
            break;
        }
        
        
      } 
    }   
  }
  
  previousReading = reading;

  
  xMenuArrowLastPos = xMenuArrowPos;
  if (xValue > minThreshold) {
    if (xMenuArrowPos < menuArrowArraySize - 1) {
      xMenuArrowPos++;
    } 
    else {
      xMenuArrowPos = 0;
    }
    
  }

  if (xValue < maxThreshold) {
    if (xMenuArrowPos > 0) {
      xMenuArrowPos--;
    }
    else {
      xMenuArrowPos = menuArrowArraySize - 1;
    }
  }

  if (xMenuArrowPos != xMenuArrowLastPos) {
    menuChanged = true;
    if (xMenuArrowPos > xMenuArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    menuArrowArray[xMenuArrowLastPos] = false;
    menuArrowArray[xMenuArrowPos] = true;
    
  }
  
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
  
  if (onLightedLed == false) {
    lc.setLed(0, xPos, yPos, true);
  } else {
    lc.setLed(0, xPos, yPos, false);
  }
}

void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  
  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue > minThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    } 
    else {
      xPos = 0;
    }
  }

  if (xValue < maxThreshold) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }

  if (yValue < maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    } 
    else {
      yPos = 0;
    }
  }

  if (yValue > minThreshold) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    if (onLightedLed == false) {
      matrix[xLastPos][yLastPos] = 0;
    } else {
      matrix[xLastPos][yLastPos] = 1;
    }
    if (matrix[xPos][yPos] == 1) {
      onLightedLed = true;
    } else {
      onLightedLed = false;
    }


    if(xPos > xLastPos && noOfMoves[0] == 0) {
      xPos = xLastPos;
    }
    if(xPos < xLastPos  && noOfMoves[1] == 0) {
      xPos = xLastPos;
    }

    
    if(yPos > yLastPos && noOfMoves[2] == 0) {
      yPos = yLastPos;
    }
    if(yPos < yLastPos && noOfMoves[3] == 0) {
      yPos = yLastPos;
    }

    if (xPos > xLastPos) {
      noOfMoves[0]--; 
    }
    if (xPos < xLastPos) {
      noOfMoves[1]--; 
    }
    if (yPos > yLastPos) {
      noOfMoves[2]--; 
    }
    if (yPos < yLastPos) {
      noOfMoves[3]--; 
    }

    Serial.println("///////////////");
    for(int i = 0; i <= 3; i++) {
      Serial.println(noOfMoves[i]);
    }
    Serial.println("///////////////");
    
    matrix[xPos][yPos] = 1;
    
  }
  
}

//Happy Face
void happyFace()
{
  int i = 0, j = 0;
  for (i = 1; i <= 3; i++) {
    for (j = 1; j <= 2; j++) {
      lc.setLed(0, i, j, true);
      lc.setLed(0, i, j + 4, true);
    }
  }
  lc.setLed(0, 5, 1, true);
  lc.setLed(0, 6, 1, true);//
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 7, 2, true);//
  lc.setLed(0, 7, 3, true);
  lc.setLed(0, 7, 4, true);
  lc.setLed(0, 7, 5, true);//
  lc.setLed(0, 6, 5, true);
  lc.setLed(0, 6, 6, true);//
  lc.setLed(0, 5, 6, true);
  delay(1000);
}

//Sad Face
void sadFace()
{
  int i = 0, j = 0;
  for (i = 1; i <= 3; i++) {
    for (j = 1; j <= 2; j++) {
      lc.setLed(0, i, j, true);
      lc.setLed(0, i, j + 4, true);
    }
  }
  lc.setLed(0, 6, 1, true);//
  lc.setLed(0, 5, 2, true);//
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);//
  lc.setLed(0, 6, 6, true);//
  delay(1000);
}

//Dead Face
void deadFace() {
  int i = 0, j = 2;
  for (i = 0; i <= 2; i++) {
    lc.setLed(0, i + 1, i, true);//
    lc.setLed(0, i + 1, i + 5, true);//
    lc.setLed(0, j + 1, i, true);//
    lc.setLed(0, j + 1, i + 5, true);//
    j--;
  }
  lc.setLed(0, 6, 1, true);//
  lc.setLed(0, 5, 2, true);//
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);//
  lc.setLed(0, 6, 6, true);//
  delay(3000);
}
