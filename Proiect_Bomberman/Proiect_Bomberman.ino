#include "LedControl.h" //  need the library
#include<LiquidCrystal.h>
#include<EEPROM.h>

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

// switch varianbles
const byte startingValue = 5;
byte currentNameIndex;
byte goodOrBad = 0;

//change name variables
const byte noOfLetters = 27;
bool changeNameChanged = false;
const byte playerNameCharIndexSize = 5;
byte playerNameCharIndex[playerNameCharIndexSize];
bool stateChangeName = true;
byte nameIndex = 0;

const char alphabet[noOfLetters] = {
  'A', 
  'B', 
  'C', 
  'D', 
  'E', 
  'F', 
  'G', 
  'H', 
  'I', 
  'J', 
  'K', 
  'L', 
  'M', 
  'N', 
  'O', 
  'P', 
  'Q', 
  'R', 
  'S', 
  'T',
  'U', 
  'V', 
  'W',
  'X',
  'Y',
  'Z',
  '.'
};

//const int contrastPin = 9;
const int brightnessPin = 9;
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
bool brightnessChanged = false;
byte brightnessShowValue = 2;
byte brightness = 160;
byte lastBrightness = 160;

byte contrast = 50;
byte lastContrast = 50;
bool contrastChanged = false;
byte contrastShowValue = 5;
bool settingsChanged = false;
const byte settingsArrowArraySize = 3;
byte xSettingsArrowLastPos = 0;
byte xSettingsArrowPos = 0;

char* settings[settingsArrowArraySize] = {
  "BACK TO MENU", "BRIGHTNESS", "CONTRAST"//, "CHANGE NAME?"
};


// highscores variables
const byte noOfHighscores = 9;
String top[9];
bool highscoresChanged = false;
byte xHighscoresArrowPos = 0;
byte xHighscoresArrowLastPos = 0;
const byte highscoresArrowArraySize = 13;

String highscores[highscoresArrowArraySize] = {
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



const char* about[aboutArrowArraySize] = {
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
//
//bool plotArrowArray[plotArrowArraySize] = {
//  true, false, false, false, false, false, false, false
//};

const char* plot[plotArrowArraySize] = {
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
bool firstRound = true;
const byte menuArrowArraySize = 5;
const byte noOfLcdRows = 2;
byte xMenuArrowLastPos = 0;
byte xMenuArrowPos = 0;
bool upOrDown = true;
bool menuChanged = false;
const byte noOfDiff = 6;
bool changeAfterGameDisplayOnce = true;


const char* menuList[menuArrowArraySize] = {
  "START GAME", "HIGHSCORES", "PLOT", "SETTINGS", "ABOUT"
};

const char* difficulties[noOfDiff] = {
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

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
 
//const int n = 8;

int counterLevelBasedOnDiff;
byte lastDifficulty = 2;
bool difficultyChanged = false;
int difficulty = 2;  // 0 - Super Easy ; 1 - Easy ; 2 - Normal ; 3 - Hard ; 4 - God Mode 
byte matrixBrightness = 2;
bool testMenuOrGame = true;

const byte heartsOnStart = 3;
byte hearts = heartsOnStart;
byte raiseHeartsNumber = 0;
byte heartRaiseThreshold = 15;

byte max2 = 0;
byte max3 = 0;
byte max4 = 0;
byte max5 = 0;
byte max6 = 0;
byte max7 = 0;

int currentTop[9];
bool playerLight = true;
const byte playerIntervalMax = 50;
byte playerInterval = 0;
const long levelInterval = 2500000;
const byte noOfLeds = 64;
byte noOfLightedLeds = 0;
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

const byte noOfMovesOnStart = 5;
const byte noOfMovesSize = 4;
byte noOfMoves[noOfMovesSize] = {
  noOfMovesOnStart, noOfMovesOnStart, noOfMovesOnStart, noOfMovesOnStart
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
  B00000},

 {B11111,  // FULL CELL           8
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111},

 {B00000,  // check               9
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000},

 {B00000,  // heart               10
  B01010,
  B10101,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000},

 {B10001,  // trophy              11
  B01010,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110},

 {B00000,  // move up             12
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000},

 {B00000,  // move down           13
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000},

 {B00000,  // move left           14
  B00000,
  B00100,
  B01100,
  B11111,
  B01100,
  B00100,
  B00000},

 {B00000,  // move right          15
  B00000,
  B00100,
  B00110,
  B11111,
  B00110,
  B00100,
  B00000}
 
};

int currentScore = 0;

//void DisplayAndSetContrast(int contrast) { 
// 
//      lcd.setCursor(0, 0);
//      lcd.print("hello there"); 
//      analogWrite(contrastPin, contrast);  
//}

const byte noOfDigits = 10;
byte intToChar[noOfDigits] = {
  30, 31, 32, 33, 34, 35, 36, 37, 38, 39
};

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
//  byte index = 0;
//  for (int i = 0; i <= 255; i++) {
//    EEPROM.update(i, index);
//    index++;
//    if (index == 1) {
//      index = 0;
//    }
//  }

  for (int index = 0; index < playerNameCharIndexSize; index++) {
    playerNameCharIndex[index] = EEPROM.read(index);
  }
    
  
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  pinMode(dinPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  pinMode(brightnessPin, OUTPUT);  
  
  randomSeed(analogRead(12));
  matrix[xPos][yPos] = 1;
  
  lcd.begin(16, 2);

  byte eepromAdress = 0;

  eepromAdress = 5;
  String auxWrite = "";
  byte currentTopIndex = 0;
  for (int index = 0; index < noOfHighscores; index++) {
    for (int indexx = 0; indexx < 5; indexx++) {
      auxWrite += alphabet[EEPROM.read(eepromAdress)];        
      eepromAdress++;
    }
    
    auxWrite += "      ";
    int multiplyingFactor = 1000;
    for (int indexx = 12; indexx < 16; indexx++) {
      auxWrite += String(EEPROM.read(eepromAdress));
//      Serial.println("----");
//      Serial.println(EEPROM.read(eepromAdress));
//      Serial.println(String(EEPROM.read(eepromAdress)));
//      Serial.println("----");
      currentTop[currentTopIndex] += multiplyingFactor * int(EEPROM.read(eepromAdress));
      multiplyingFactor /= 10;
      eepromAdress++;
    }
    Serial.print("currentTop: ");
    Serial.println(currentTop[currentTopIndex]);
    currentTopIndex++;
    top[index] = auxWrite;
    auxWrite = "";
  }

  byte highscoresIndex = 2;
  byte topIndex = 0;
  for (int index = 0; index < 3; index++) {
//    for (int indexx = 2; indexx >= 0 ; indexx--) {
    for (int indexx = 0; indexx < 3; indexx++) {
      highscores[highscoresIndex] = top[topIndex]; 
      topIndex++;
      highscoresIndex++;
    }
    highscoresIndex++;
  }
  topIndex = 0;
  highscoresIndex = 0;
  eepromAdress = 0;
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
          updateChangeName();
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

      case 5:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateSettingsArrow();
          lastMoved = millis();
        }
        if (settingsChanged == true) {
          // matrix display logic
          updateSettingsDisplay();
          settingsChanged = false;
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
        firstRound = true;
        hearts = heartsOnStart;

        currentNameIndex = 0;
        
        for (byte eepromIndex = 0; eepromIndex < startingValue; eepromIndex++) {
          EEPROM.update(eepromIndex, playerNameCharIndex[currentNameIndex]);
          currentNameIndex++;
        }
        
        lc.clearDisplay(0);
        lcd.setCursor(0, 0);
//        lcd.print("HEARTS: ");
//        lcd.setCursor(8, 0);
//        lcd.print(hearts);
//        lcd.setCursor(0, 1);
        lcd.print("DODGE THE LEDS!");
        testMenuOrGame = false;
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            matrix[row][col] = 0;
          }
        }
        xPos = 0;
        xLastPos = 0;
        yPos = 0;
        yLastPos = 0;
        counterLevelDuration = 0;
        matrixChanged = true;
        onLightedLed = 0;
        lastMoved = 0;
//        Serial.println("wth");
        if (matrixChanged == true) {
        // matrix display logic
          updateMatrix();
          matrixChanged = false;
        }
        break;

      case 8:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateBrightnessValue();
          lastMoved = millis();
        }
        if (brightnessChanged == true) {
          // matrix display logic
          updateBrightnessDisplay();
          brightnessChanged = false;
        }
        break;

      case 9:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateContrastValue();
          lastMoved = millis();
        }
        if (contrastChanged == true) {
          // matrix display logic
          updateContrastDisplay();
          contrastChanged = false;
        }
        break;

      case 10:
        if (changeAfterGameDisplayOnce == true) {
          deadFace();
        
          lcd.clear();
          lcd.setCursor(0, 0);
          if (difficulty >= 2) {
            if (goodOrBad > 0) {
              lcd.print("CONGRATULATIONS!");
            } else {
              lcd.print("GL NEXT TIME!");
            }
            lcd.setCursor(0, 1);
            if (goodOrBad == 0) {
              lcd.print("SCORE: ");
              lcd.setCursor(7, 1);
              lcd.print(currentScore);
            } else if (goodOrBad == 1) {
              lcd.print("NEW TOP1: ");
              lcd.setCursor(10, 1);
              lcd.print(currentScore);
            } else if (goodOrBad == 2) {
              lcd.print("NEW TOP2: ");
              lcd.setCursor(10, 1);
              lcd.print(currentScore);
            } else if (goodOrBad == 3) {
              lcd.print("NEW TOP3: ");
              lcd.setCursor(10, 1);
              lcd.print(currentScore);
            }
          } else {
            lcd.print("CONGRATULATIONS!");
            lcd.setCursor(0, 1);
            lcd.print("SCORE: ");
            lcd.setCursor(7, 1);
            lcd.print(currentScore);
          }
          changeAfterGameDisplayOnce = false;
        }
        
                
        for (int index = 0; index < noOfMovesSize; index++) {
          noOfMoves[index] = noOfMovesOnStart;
        }
        
        reading = digitalRead(swPin);

        if (reading  != previousReading) {
          lastDebounceTime = millis();
        }
        
        if (millis() - lastDebounceTime > debounceDelay) { 
          if (previousReading != reading){
            buttonState = reading;
            if (buttonState == LOW){
              changeAfterGameDisplayOnce = true;
              lc.clearDisplay(0);
              updateMenuDisplay();
              menuState = 0;
            }
          }   
        }
        
        previousReading = reading;
        break;
        
      default:
        break;
    }
    Serial.println(menuState);
    
  } else {    
    counterLevelDuration += counterLevelBasedOnDiff;
    playerInterval++;
    updatePlayerLed();
//    Serial.println(counterLevelDuration);
    
    if (counterLevelDuration >= levelInterval) { // || firstRound == true) {
      ////////
      
      if (firstRound != true) {
        lc.clearDisplay(0);
//        Serial.println("-----");
//        Serial.println(hearts);
//        Serial.println("-----");
        
        currentScore = 0;
        for( int index = 0; index < noOfMovesSize; index++) {
          currentScore += noOfMoves[index];
        }
        
        if (onLightedLed == true) {
          hearts--;              // jucatorul pierde o viata
          raiseHeartsNumber = 0;  // resetam nr de runde castigate fara oprire
          sadFace();
          
        } else {
          raiseHeartsNumber++;
          happyFace();
          
        }
        if (raiseHeartsNumber == heartRaiseThreshold && hearts < 5) {
          hearts++;
          raiseHeartsNumber = 0;
        }
        
        
        lcd.clear();
        lcd.createChar(10, formsArray[10]); // heart
        lcd.createChar(11, formsArray[11]); // medal
        lcd.createChar(12, formsArray[12]); // up
        lcd.createChar(13, formsArray[13]); // down
        lcd.createChar(14, formsArray[14]); // left
        lcd.createChar(15, formsArray[15]); // right
        
        for (byte index = 1; index <= hearts; index++) {
          lcd.setCursor(index, 0);
          lcd.write(byte(10));
        }
        
        lcd.setCursor(7, 0);
        lcd.write(byte(11));
        lcd.setCursor(8, 0);
        lcd.print(currentScore);
        byte lcdCol = 0;
        for (byte index = 0; index <= noOfMovesSize; index++) {
          lcd.setCursor(lcdCol, 1);
          lcd.write(byte(12 + index));
          lcd.setCursor(lcdCol + 1, 1);
          lcd.print(noOfMoves[index]);
          lcdCol += 4;
        }
  
        if (hearts == 0) {
          byte auxDiff = difficulty;
          bool hsEnabled = true;
          switch(difficulty) {
            case 2:
              auxDiff = 4;
              break;
            case 3:
              auxDiff = 3;
              break;
            case 4:
              auxDiff = 2;
              break;

            default:
              hsEnabled = false;
              break;
          }
          if (hsEnabled == true) {
            if (currentScore > currentTop[(auxDiff - 2) * 3]) {
              goodOrBad = 1;
              updateEeprom((auxDiff - 2) * 3 + 2, currentTop[(auxDiff - 2) * 3 + 1]);
              updateEeprom((auxDiff - 2) * 3 + 1, currentTop[(auxDiff - 2) * 3]);
              updateEeprom((auxDiff - 2) * 3, currentScore);
            } else if (currentScore > currentTop[(auxDiff - 2) * 3 + 1]) {
              goodOrBad = 2;
              updateEeprom((auxDiff - 2) * 3 + 2, currentTop[(auxDiff - 2) * 3 + 1]);
              updateEeprom((auxDiff - 2) * 3 + 1, currentScore);
            } else if (currentScore > currentTop[(auxDiff - 2) * 3 + 2]) {
              goodOrBad = 3;
              updateEeprom((auxDiff - 2) * 3 + 2, currentScore);
            } else {
              goodOrBad = 0;
            }
  
            byte eepromAdress = 0;
  
            eepromAdress = 5;
            String auxWrite = "";
            byte currentTopIndex = 0;
  
            for (int index = 0; index < noOfHighscores; index++) {
              currentTop[index] = 0;
            }
            
            for (int index = 0; index < noOfHighscores; index++) {
              for (int indexx = 0; indexx < 5; indexx++) {
                auxWrite += alphabet[EEPROM.read(eepromAdress)];        
                eepromAdress++;
              }
              
              auxWrite += "      ";
              int multiplyingFactor = 1000;
              for (int indexx = 12; indexx < 16; indexx++) {
                auxWrite += String(EEPROM.read(eepromAdress));
        //        Serial.println("----");
        //        Serial.println(EEPROM.read(eepromAdress));
        //        Serial.println(String(EEPROM.read(eepromAdress)));
        //        Serial.println("----");
                currentTop[currentTopIndex] += multiplyingFactor * int(EEPROM.read(eepromAdress));
                multiplyingFactor /= 10;
                eepromAdress++;
              }
              Serial.print("currentTop: ");
              Serial.println(currentTop[currentTopIndex]);
            
              currentTopIndex++;
              top[index] = auxWrite;
              auxWrite = "";
            }
          
          
            byte highscoresIndex = 2;
            byte topIndex = 0;
            for (int index = 0; index < 3; index++) {
              for (int indexx = 0; indexx < 3; indexx++) {
                highscores[highscoresIndex] = top[topIndex]; 
                topIndex++;
                highscoresIndex++;
              }
              highscoresIndex++;
            }
            topIndex = 0;
            highscoresIndex = 0;
            eepromAdress = 0;

          }  
          
            
          testMenuOrGame = true;
          menuState = 10;
          lc.clearDisplay(0);
        }
        
      } else {
        firstRound = false;
      }

      
      switch (difficulty) {
        case 0:  // Super Easy (Tutorial)
          randNumber = random(2,4);     // random(2, 8);
          max2 = 6;
          max3 = 2;
          break;
        case 1:  // Easy
          randNumber = random(2,5);     // random(2, 8);
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
          randNumber = random(2,8);     // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;
          
        default:  // God Mode
          randNumber = random(2,4);     // random(2, 8);
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
                  if (availabilityTest == false || noOfLightedLeds + 4 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += 4;
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
                  if (availabilityTest == false || noOfLightedLeds + 9 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += 9;
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
                  if (availabilityTest == false || noOfLightedLeds + 16 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += 16;
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
                  if (availabilityTest == false || noOfLightedLeds + 25 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += 25;
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
                  if (availabilityTest == false || noOfLightedLeds + 36 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += 36;
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
      if (difficulty == 4) {
        counterLevelBasedOnDiff++;
      }
    }

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
    
    ///////////////////////////////////////////////////////
    
  }
}

void updateEeprom(byte multiplyingIndex, int newTopValue) {
    /// NUME      NUME LOC  PUNCTAJ NUME LOC  PUNCTAJ NUME LOC  PUNCTAJ
    /// 0 1 2 3 4 5 6 7 8 9 10111213141516171819202122232425262728293031
    /// S L E E P S L E E P 9 9 9 9 S L E E P 9 9 9 9 S L E E P 9 9 9 9 
    // 5 - <14   ~   9
    const byte multiplyingConstant = 9;
    const byte startingValue = 5;
    const byte nameLength = 5;
    const byte scoreLength = 4;
    byte currentNameIndex = 0;
    
    
    for (byte eepromIndex = startingValue + (multiplyingIndex * multiplyingConstant); eepromIndex < startingValue + (multiplyingIndex * multiplyingConstant) + nameLength; eepromIndex++) {
      EEPROM.update(eepromIndex, playerNameCharIndex[currentNameIndex]);
      currentNameIndex++;
    }
    
    int dividingFactor = 1000;
    for (byte eepromIndex = startingValue + (multiplyingIndex * multiplyingConstant) + nameLength; eepromIndex < startingValue + (multiplyingIndex * multiplyingConstant) + nameLength + scoreLength; eepromIndex++) {
      EEPROM.update(eepromIndex, (newTopValue / dividingFactor) % 10);
//      Serial.println();
//      Serial.print(newTopValue);
//      Serial.print(" / ");
//      Serial.print(dividingFactor);
//      Serial.print(" % 10 = ");
//      Serial.print((newTopValue / dividingFactor) % 10);
//      Serial.println("----");
      dividingFactor /= 10;
    }
    
}



void updateStartGameChangeNameDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[9]);
  lcd.createChar(2, formsArray[7]);
  
  lcd.setCursor(0, 0);
  lcd.print("    SET NAME    ");
  if (nameIndex <= playerNameCharIndexSize) {
    for (int index = 2; index < 13; index += 2) {
      lcd.setCursor(index, 1);
      lcd.write(byte(2));
    }
    lcd.setCursor(nameIndex * 2 + 2, 1);
    lcd.write(byte(0));
  } else {
    lcd.setCursor(nameIndex * 2, 1);
    lcd.write(byte(0));
  }
  byte col = 3;
  for (int index = 0; index < playerNameCharIndexSize; index++) {
    lcd.setCursor(col, 1);
    col += 2;
    lcd.print(alphabet[playerNameCharIndex[index]]);
  }
  lcd.setCursor(col, 1);
  lcd.write(byte(1));
}


void updateChangeName() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) { 
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        if (nameIndex == playerNameCharIndexSize) {
          upOrDown = true;
          menuState = 7;
        
        } else {
          stateChangeName = !stateChangeName;
          
        }
      }
    }   
  }
  
  previousReading = reading;
  byte lastNameIndex = nameIndex;
  byte lastPlayerNameCharIndex = playerNameCharIndex[nameIndex];
  
  if (stateChangeName == true) {
    if (yValue < maxThreshold) {
      if (nameIndex < 6) {
        nameIndex++;
      } 
    } 
  
    if (yValue > minThreshold) {
      if (nameIndex > 0) {
        nameIndex--;
      }
    }
  } else {
    if (xValue > maxThreshold) {
      if (playerNameCharIndex[nameIndex] > 0) {
        playerNameCharIndex[nameIndex]--;
      } else {
        playerNameCharIndex[nameIndex] = noOfLetters - 2;
//        Serial.println(playerNameCharIndex[nameIndex]);
      }
    }
  
    if (xValue < minThreshold) {
      if (playerNameCharIndex[nameIndex] < noOfLetters - 2) {
        playerNameCharIndex[nameIndex]++;
      } else {
        playerNameCharIndex[nameIndex] = 0;
//        Serial.println(playerNameCharIndex[nameIndex]);
//        Serial.println("---------");
      }
    }
  }
  if (lastNameIndex != nameIndex) {
    changeNameChanged = true;
    updateStartGameChangeNameDisplay();
  } else if (playerNameCharIndex[nameIndex] != lastPlayerNameCharIndex) {
    changeNameChanged = true;
    updateStartGameChangeNameDisplay();
  } 
  
  
}


void updateBrightnessDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[4]);
  lcd.createChar(1, formsArray[8]);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  
  lcd.setCursor(1, 0);
  lcd.print("SET BRIGHTNESS");

  lcd.setCursor(1, 1);
//  Serial.println(difficulties[difficulty]);
  switch(brightness) {
    case 50:
      brightnessShowValue = 4;
      break;

    case 100:
      brightnessShowValue = 3;
      break;

    case 160:
      brightnessShowValue = 2;
      break;

    case 255:
      brightnessShowValue = 1;
      break;

    default: 
      brightnessShowValue = 5;
      break;
  }
  for (int index = 1; index <= brightnessShowValue; index++) {
    lcd.setCursor(index, 1);
    lcd.write(byte(1));
  }
  
}


void updateBrightnessValue() {
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
        updateSettingsDisplay();
        menuState = 5;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastBrightness = brightness;
  
  if (xValue > maxThreshold) {
    if (brightness < 255) {
      switch(brightness) {
        case 0:
          brightness = 50;
          break;
        case 50:
          brightness = 100;
          break;
        case 100:
          brightness = 160;
          break;
        case 160:
          brightness = 255;
          break;
      }
    } 
  }

  if (xValue < minThreshold) {
    if (brightness > 0) {
      switch(brightness) {
        case 255:
          brightness = 160;
          break;
        case 160:
          brightness = 100;
          break;
        case 100:
          brightness = 50;
          break;
        case 50:
          brightness = 0;
          break;
        default:
          break;
      }
    }
  }

  if (brightness != lastBrightness) {
    brightnessChanged = true;
//    updateStartGameDifDisplay();
  } 
}



void updateContrastDisplay() {
  lcd.clear();
  lcd.createChar(0, formsArray[4]);
  lcd.createChar(1, formsArray[8]);

  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  
  lcd.setCursor(1, 0);
  lcd.print("SET CONTRAST");

  lcd.setCursor(1, 1);
//  Serial.println(difficulties[difficulty]);
  switch(contrast) {
    case 50:
      contrastShowValue = 4;
      break;

    case 70:
      contrastShowValue = 3;
      break;

    case 100:
      contrastShowValue = 2;
      break;

    case 115:
      contrastShowValue = 1;
      break;

    default: 
      contrastShowValue = 5;
      break;
  }
  for (int index = 1; index <= contrastShowValue; index++) {
    lcd.setCursor(index, 1);
    lcd.write(byte(1));
  }
  
}

void updateContrastValue() {
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
        updateSettingsDisplay();
        menuState = 5;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastContrast = contrast;
  
  if (xValue > maxThreshold) {
    if (contrast < 115) {
      switch(contrast) {
        case 0:
          contrast = 50;
          break;
        case 50:
          contrast = 70;
          break;
        case 70:
          contrast = 100;
          break;
        case 100:
          contrast = 115;
          break;
      }
    } 
  }

  if (xValue < minThreshold) {
    if (contrast > 0) {
      switch(contrast) {
        case 115:
          contrast = 100;
          break;
        case 100:
          contrast = 70;
          break;
        case 70:
          contrast = 50;
          break;
        case 50:
          contrast = 0;
          break;
        default:
          break;
      }
    }
  }

  if (contrast != lastContrast) {
    contrastChanged = true;
//    DisplayAndSetContrast(contrast);
//    updateStartGameDifDisplay();
  } 
}


void updateSettingsDisplay() {
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
      lcd.print(settings[xSettingsArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
      lcd.print(settings[xSettingsArrowPos - row]);
    }
  } 
}

void updateSettingsArrow() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        switch(xSettingsArrowPos) {
          case 0:
            upOrDown = true;
            menuState = 0;
            updateMenuDisplay();
            break;

          case 1:
            upOrDown = true;
            // highscores function
            menuState = 8;
            updateBrightnessDisplay();
            break;
            
          case 2:
            upOrDown = true;
            // plot function
            menuState = 9;
             updateContrastDisplay();
            break;

          case 3:
            upOrDown = true;
            // settings function
//            menuState = ?;
//            updateChangeNameDisplay();
            break;

          default:
            break;

        }
        if (xSettingsArrowPos == 0) {
          menuState = 0;
          updateMenuDisplay();
        }
      } 
    }   
  }
  
  previousReading = reading;

  xSettingsArrowLastPos = xSettingsArrowPos;
  if (xValue > minThreshold) {
    if (xSettingsArrowPos < settingsArrowArraySize - 1) {
      xSettingsArrowPos++;
    } 
    else {
      xSettingsArrowPos = 0;
    }
    
  }

  if (xValue < maxThreshold) {
    if (xSettingsArrowPos > 0) {
      xSettingsArrowPos--;
    }
    else {
      xSettingsArrowPos = settingsArrowArraySize - 1;
    }
  }

  if (xSettingsArrowPos != xSettingsArrowLastPos) {
    settingsChanged = true;
    if (xSettingsArrowPos > xSettingsArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    
  }
  
}




//
//
//char* top[9];
//bool highscoresChanged = false;
//byte xHighscoresArrowPos = 0;
//byte xHighscoresArrowLastPos = 0;
//const byte highscoresArrowArraySize = 13;
//
//char* highscores[highscoresArrowArraySize] = {
//  "BACK TO MENU",
//  "GODE MODE:",
//  top[0], top[1], top[2],
//  "HARD:",
//  top[3], top[4], top[5],
//  "NORMAL:",
//  top[6], top[7], top[8]
//};



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

  delay(4000);

  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("  CREATED BY  ");
  lcd.write(byte(0));
  lcd.setCursor(0, 1);
//  lcd.write(byte(0));
  lcd.print(" SLEEPY STUDIOS ");

  delay(2000);
  
  lcd.clear();
  enterMenu();
}

void enterMenu() {
  lcd.createChar(0, formsArray[3]);
  lcd.createChar(1, formsArray[5]);
  upOrDown = true;
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print("START GAME");
  lcd.setCursor(1, 1);
  lcd.print("HIGHSCORES");

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
//      Serial.println(menuList[xMenuArrowPos + row]);
      lcd.print(menuList[xMenuArrowPos + row]);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(1, row - 1);
//      Serial.println(menuList[xMenuArrowPos - row]);
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
//  Serial.println(difficulties[difficulty]);
  lcd.print(difficulties[difficulty]);
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
        switch (difficulty) {
          case 2:
            counterLevelBasedOnDiff = 20;
            break;
            
          case 3:
            counterLevelBasedOnDiff = 25;
            break;
            
          case 4: 
            counterLevelBasedOnDiff = 40;
            break;

          default:
            counterLevelBasedOnDiff = 10;
            break;
        }
        
        upOrDown = true;
        updateStartGameChangeNameDisplay();
        menuState = 2;
        
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
            // highscores function
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
            updateSettingsDisplay();
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
//    Serial.println("hello there");
    
  }
  
}



void updatePlayerLed() {
//  Serial.println(playerInterval);
  if (playerInterval >= playerIntervalMax) {
//    Serial.println(playerIntervalMax);
    playerInterval = 0;
    playerLight = !playerLight;
    lc.setLed(0, xPos, yPos, playerLight);
  }
}


void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
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

    if (xPos > xLastPos && firstRound == false) {
      noOfMoves[0]--; 
    }
    if (xPos < xLastPos && firstRound == false) {
      noOfMoves[1]--; 
    }
    if (yPos > yLastPos && firstRound == false) {
      noOfMoves[2]--; 
    }
    if (yPos < yLastPos && firstRound == false) {
      noOfMoves[3]--; 
    }

//    Serial.println("///////////////");
    for(int i = 0; i <= 3; i++) {
//      Serial.println(noOfMoves[i]);
    }
//    Serial.println("///////////////");
    
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
  if (difficulty < 4) {
    delay(1400);
  } else {
    delay(700);
  }
}
