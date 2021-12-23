#include "LedControl.h"
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include <NewTone.h>


const int contrastPin = A2;
const int lcdBrightnessPin = A3;
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

const int longNoteDuration = 500;
const byte mediumNoteDuration = 250;
const byte shortNoteDuration = 125;
const byte arrowBuzzerTone = 125;
const byte enterButtonBuzzerTone = 50;
const byte backToMenuButtonBuzzerTone = 80;
const byte letterChangeBuzzerTone = 165;
byte switchStatesDuringNameChangeArrowDisplay = 3;

const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_AS4 = 466;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_D5 = 587;
const int NOTE_FS5 = 740;
const int NOTE_C6 = 1047;
const int NOTE_D6 = 1175;
const int NOTE_F6 = 1397;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int buzzerPin = 9;

const byte lcdBrightnessEepromAdress = 89;
const byte contrastEepromAdress = 90;
const byte matrixBrightnessEepromAdress = 91;

const byte maxSquareSideLengthForSuperEasy = 3;
const byte maxSquareSideLengthForEasy = 4;
const byte maxSquareSideLengthForNormalAndHard = 7;
const byte maxSquareSideLengthForGodMode = 4;

const byte menuArrowStartGamePos = 0;
const byte menuArrowHighscoresPos = 1;
const byte menuArrowPlotPos = 2;
const byte menuArrowSettingsGamePos = 3;
const byte menuArrowAboutGamePos = 4;

const byte settingsArrowBackToMenuPos = 0;
const byte settingsArrowLcdBrightnessPos = 1;
const byte settingsArrowContrastPos = 2;
const byte settingsArrowMatrixBrightnessPos = 3;
const byte settingsArrowResetHighscoresPos = 4;

const byte squareWithSideLengthOf2 = 2;
const byte squareWithSideLengthOf3 = 3;
const byte squareWithSideLengthOf4 = 4;
const byte squareWithSideLengthOf5 = 5;
const byte squareWithSideLengthOf6 = 6;
const byte squareWithSideLengthOf7 = 7;
const byte noOfMatrixRows = 8;
const byte noOfMatrixCols = 8;
const byte minSquareSideLength = 2;
const byte gapPetweenFirstAndThirdPlaceIndex = 2;
const byte gapPetweenFirstAndSecondPlaceIndex = 1;
const byte noOfDifficultiesWithNoHS = 2; // super easy and easy
const byte noOfHighscoresForEachDiff = 3;
const byte firstMatrixRowPos = 0;
const byte firstMatrixColPos = 0;
const byte firstPlayerNameCharIndex = 0;
const byte nameIndexPosMultiplyer = 2;
const byte firstLetterPosDisplay = 2;

const byte eepromFirstHighscoresAdress = 5;
const byte eepromLastHighscoresAdress = 88;
const byte lowestContrastValue = 0;
const byte highestContrastValue = 45;
const byte lowestLcdBrightnessValue = 10;
const byte highestLcdBrightnessValue = 100;

const byte highscoresMenuScoreFirstPos = 12;
const byte highscoresMenuScoreLastPos = 16;

const byte lastLetterPosDisplay = 13;
const byte letterDisplayInterval = 2;
const byte firstElementOfArrayIndex = 0;
const byte matrixIndex = 0;
const bool matrixSaveModeState = false;
const byte randomSeedPin = 12;
const byte playerPositionLedValue = 1;
const byte noOfLcdColumns = 16;
const byte noOfLcdRows = 2;
const byte eepromAdressForHighscores = 5;
const byte eepromAdressForName = 0;
String emptyString = "";
String spaceBetweenNameAndScore = "      ";
const int fitrstMultiplyingFactorForScore = 1000;
const byte multiplyingAndDividingFactorGap = 10;
const byte currentTopIndexFirstValue = 0;
const int baudRate = 9600;
const int gameNameDisplayTime = 4000;
const int creatorNameDisplayTime = 2000;
const byte firstCellOfTheLcdPos = 0;
const byte currentNameIndexInitialValue = 0;

const byte secondLcdRowPos = 1;
const byte firstLcdRowPos = 0;
const byte secondLcdColPos = 1;
const byte firstLcdColPos = 0;

const byte mainMenuStateValue = 0;
const byte setDifficultyMenuStateValue = 1;
const byte changingNameMenuStateValue = 2;
const byte highscoresMenuStateValue = 3;
const byte plotStateValue = 4;
const byte settingsMenuStateValue = 5;
const byte aboutStateValue = 6;
const byte setupGameOnStartStateValue = 7;
const byte lcdBrightnessMEnuStateValue = 8;
const byte contrastMenuStateValue = 9;
const byte gameEndDisplayMenuStateValue = 10;
const byte matrixBrightnessMenuStateValue = 11;
const byte resetHighscoresMenuStateValue = 12;
const byte arrowMoveToneTimer = 20;



const byte startingValue = 5;
byte currentNameIndex;
byte goodOrBad = 0;

const byte noOfLetters = 27;
bool changeNameChanged = false;
const byte playerNameCharIndexSize = 5;
byte playerNameCharIndex[playerNameCharIndexSize];
bool stateChangeName = true;
byte nameIndex = 0;

const byte noHighscore = 0;

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


// settings variables
bool lcdBrightnessChanged = false;
byte lcdBrightnessShowValue = 2;
byte lcdBrightness = 60;
byte lastLcdBrightness = 60;

byte yResetHighscoresArrowLastPos = 0;
byte yResetHighscoresArrowPos = 0;
const byte resetHighscoresArrowArraySize = 2;
bool resetHighscoresChanged = false;
char* resetHighscoresArray[resetHighscoresArrowArraySize] = {
  "NO", "YES"
};

byte contrast = 35;
byte lastContrast = 35;
bool contrastChanged = false;
byte contrastShowValue = 4;
bool settingsChanged = false;
byte matrixContrast = 0;
const byte settingsArrowArraySize = 5;
byte xSettingsArrowLastPos = 0;
byte xSettingsArrowPos = 0;

const char* settings[settingsArrowArraySize] = {
  "BACK TO MENU", "LCD BRIGHTNESS", "CONTRAST", "MATR BRIGHTNESS", "RES SCOREBOARD"
};


bool matrixBrightnessChanged = false;
byte matrixBrightnessShowValue;
byte lastMatrixBrightness = 2;
byte matrixBrightness = 2;

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
const byte aboutArrowArraySize = 6;
bool aboutChanged = false;



const char* about[aboutArrowArraySize] = {
    "BACK TO MENU",
    "X  BOMBERMAN  X",
    "X BROUGHT  BY X",
    "SLEEPY  STUDIOS",
    "github.com/",
    "stefanpascu"
  };

// plot variables
bool plotChanged = false;
byte xPlotArrowPos = 0;
byte xPlotArrowLastPos = 0;
const byte plotArrowArraySize = 4;

const char* plot[plotArrowArraySize] = {
    "BACK TO MENU   ",      
    "I have to dodge",
    "the bombs I, ",
    "myself, blew up"
  };

// menu variables
bool firstRound = true;
const byte menuArrowArraySize = 5;
byte xMenuArrowLastPos = 0;
byte xMenuArrowPos = 0;
bool upOrDown = true;
bool menuChanged = false;
const byte noOfDiff = 6;
bool changeAfterGameDisplayOnce = true;


const char* menuList[menuArrowArraySize] = {
  "START GAME", "HIGHSCORES", "PLOT", "SETTINGS", "ABOUT"
};

const byte superEasy = 0;
const byte easy = 1;
const byte normal = 2;
const byte hard = 3;
const byte godMode = 4;

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
bool testMenuOrGame = true;

const byte heartsOnStart = 3;
byte hearts = heartsOnStart;
byte raiseHeartsNumber = 0;
const byte heartRaiseThreshold = 15;

byte max2 = 0;
byte max3 = 0;
byte max4 = 0;
byte max5 = 0;
byte max6 = 0;
byte max7 = 0;

int currentTop[9];
bool playerLight = true;
const int playerIntervalMax = 20000;
int playerInterval = 0;
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

const byte noOfMovesOnStart = 3;
const byte movesPerTurn = 3;
const byte noOfMovesSize = 4;
int noOfMoves[noOfMovesSize] = {
  noOfMovesOnStart, noOfMovesOnStart, noOfMovesOnStart, noOfMovesOnStart
};


const byte heartFormIndex = 0;
const byte bombFormIndex = 1;
const byte starFormIndex = 2;
const byte menuHoverArrowFormIndex = 3;
const byte upAndDownArrowFormIndex = 4;
const byte upArrowFormIndex = 5;
const byte downArrowFormIndex = 6;
const byte emptyCellFormIndex = 7;
const byte fullCellFormIndex = 8;
const byte checkFormIndex = 9;
const byte medalFormIndex = 10;
const byte moveUpFormIndex = 11;
const byte moveDownFormIndex = 12;
const byte moveLeftFormIndex = 13;
const byte moveRightFormIndex = 14;
const byte sadFaceFormIndex = 15;

int currentScore = 0;

const byte noOfDigits = 10;

byte formsArray[][8] = {
 {B00000,  // heart               0
  B01010,
  B10101,
  B10001,
  B01010,
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

 {B11111,  // full cell           8
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

 {B10001,  // medal               10
  B01010,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110},

 {B00000,  // move up             11
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000},

 {B00000,  // move down           12
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
  B00000},

 {B00000,  // move left           13
  B00000,
  B00100,
  B01100,
  B11111,
  B01100,
  B00100,
  B00000},

 {B00000,  // move right          14
  B00000,
  B00100,
  B00110,
  B11111,
  B00110,
  B00100,
  B00000},

 {B00000,  // sad face            15
  B01010,
  B01010,
  B00000,
  B01110,
  B10001,
  B10001,
  B00000},
 
};
