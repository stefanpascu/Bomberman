#include "Global_variables.h"
#include "SoftPWM.h"

void setup() {
  pinMode(dinPin, OUTPUT);
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(contrastPin, OUTPUT);
  pinMode(lcdBrightnessPin, OUTPUT);
  
  prepareSetupOnStart();
  
  Serial.begin(baudRate);  
}


void loop() {
  // based on testMenuOrGame's value, the algorithm decides if the player is browsing the menu or playing the game 
  if (testMenuOrGame == true) {
    // stops any sound that is still playing
    noNewTone (buzzerPin);
    duringTheMenuNavigation();
    
  } else {    
    // the timers for round duration and player led blinking are updated
    counterLevelDuration += counterLevelBasedOnDiff;
    playerInterval++;
    updatePlayerLed();

    // if a round's time runs out
    if (counterLevelDuration >= levelInterval) {
      // the first round is not taken into consideration and is the time when the player 
      // may not lose points in order to place himself strategically on the matrix
      if (firstRound != true) {
        lc.clearDisplay(matrixIndex);
        
        currentScore = 0;
        // the current score is recalculated accordingly to the last round
        for( int index = 0; index < noOfMovesSize; index++) {
          currentScore += noOfMoves[index];
        }

        // if the player finishes the round placed over a lighted led, he/she loses a "heart" and the counter for bonus lives is reinitialised.
        // Otherwise, the counter is updated positively. In both cases, a face is displayed on the matrix representing how their last turn ended up 
        if (onLightedLed == true) {
          hearts--;              // jucatorul pierde o viata
          raiseHeartsNumber = 0;  // resetam nr de runde castigate fara oprire
          sadFace();
          
        } else {
          raiseHeartsNumber++;
          happyFace();
          if (raiseHeartsNumber == heartRaiseThreshold && hearts < 5) {
            NewTone (buzzerPin, NOTE_C6);
            delay (shortNoteDuration);
            NewTone (buzzerPin, NOTE_D6);
            delay (shortNoteDuration);
            NewTone (buzzerPin, NOTE_F6);
            delay (shortNoteDuration);
            noNewTone (buzzerPin);
            
            hearts++;
            raiseHeartsNumber = 0;
          }
        }

        
        
        lcd.clear();
        lcd.createChar(heartFormIndex, formsArray[heartFormIndex]); // heart
        lcd.createChar(medalFormIndex, formsArray[medalFormIndex]); // medal
        lcd.createChar(moveUpFormIndex, formsArray[moveUpFormIndex]); // up
        lcd.createChar(moveDownFormIndex, formsArray[moveDownFormIndex]); // down
        lcd.createChar(moveLeftFormIndex, formsArray[moveLeftFormIndex]); // left
        lcd.createChar(moveRightFormIndex, formsArray[moveRightFormIndex]); // right

        // prints the number of hearts the player has
        for (byte index = 1; index <= hearts; index++) {
          lcd.setCursor(index, firstLcdRowPos);
          lcd.write(byte(heartFormIndex));
        }

        // prints the score the plaher had after the last round
        lcd.setCursor(7, firstLcdRowPos);
        lcd.write(byte(medalFormIndex));
        lcd.setCursor(8, firstLcdRowPos);
        lcd.print(currentScore);
        byte lcdCol = 0;
        // prints the number of moves left in each direction
        for (byte index = 0; index < noOfMovesSize; index++) {
          lcd.setCursor(lcdCol, secondLcdRowPos);
          lcd.write(byte(moveUpFormIndex + index));
          lcd.setCursor(lcdCol + 1, secondLcdRowPos);
          lcd.print(noOfMoves[index]);
          lcdCol += 4;
        }

        // the player dies and the game is finished
        if (hearts == 0) {
          byte auxDiff = difficulty;
          bool hsEnabled = true;
          switch(difficulty) {
            case normal:
              auxDiff = 4;
              break;
            case hard:
              auxDiff = 3;
              break;
            case godMode:
              auxDiff = 2;
              break;

            default:
              hsEnabled = false;
              break;
          }
          // the scoreboard is updated if the player scored enough in his/her playthrough
          if (hsEnabled == true) {
            if (currentScore > currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff]) {
              goodOrBad = 1;  // 1st place
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndThirdPlaceIndex, currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndSecondPlaceIndex]);
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndSecondPlaceIndex, currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff]);
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff, currentScore);
            } else if (currentScore > currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndSecondPlaceIndex]) {
              goodOrBad = 2;  // 2nd place
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndThirdPlaceIndex, currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndSecondPlaceIndex]);
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndSecondPlaceIndex, currentScore);
            } else if (currentScore > currentTop[(auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndThirdPlaceIndex]) {
              goodOrBad = 3;  // 3rd place
              updateHighscoresAndNameInEeprom((auxDiff - noOfDifficultiesWithNoHS) * noOfHighscoresForEachDiff + gapPetweenFirstAndThirdPlaceIndex, currentScore);
            } else {
              goodOrBad = 0;  // no place in top 3
            }
  
            readAndSetHighscoresVariablesFromEeprom();

          }  
          
          testMenuOrGame = true;
          menuState = gameEndDisplayMenuStateValue;
          lc.clearDisplay(matrixIndex);
        }
        
      } else {
        firstRound = false;
      }
      
      noOfLightedLeds = 0;
      for (int i = 0; i < noOfMovesSize; i++) {
        noOfMoves[i] += movesPerTurn;
      }
      
      counterLevelDuration = 0;
      for (int i = firstMatrixRowPos; i < noOfMatrixRows; i++) {
        for (int j = firstMatrixColPos; j < noOfMatrixCols; j++) {
          matrix[i][j] = 0;
        }
      } 
          
      // the function that generates every round
      levelMatrixFormsGenerator();
    }

    // player position update and display
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

// contains diverse initialisers for the matrix, lcd, seeds and other variables or libraries and also displays the name of the game and of the creator
void prepareSetupOnStart() { 
  // Initialize SoftPWM which allows Digital OUTPUT pins to be used as Analog OUTPUT pins
  SoftPWMBegin();

  for (int auxIndex = firstElementOfArrayIndex; auxIndex < playerNameCharIndexSize; auxIndex++) {
    playerNameCharIndex[auxIndex] = EEPROM.read(auxIndex);
  }

  // turn off power saving, enables display
  lc.shutdown(matrixIndex, matrixSaveModeState); 
  // brightness possible values: 0~15
  lc.setIntensity(matrixIndex, matrixBrightness); 
  // clear screen
  lc.clearDisplay(matrixIndex); 

  // setting the seed for the random numbers that generate the squares
  randomSeed(analogRead(randomSeedPin));
  
  matrix[xPos][yPos] = playerPositionLedValue;
  
  lcd.begin(noOfLcdColumns, noOfLcdRows);

  readAndSetHighscoresVariablesFromEeprom();

  lcd.createChar(starFormIndex, formsArray[starFormIndex]);
  lcd.createChar(bombFormIndex, formsArray[bombFormIndex]);

  readBrightnessAndContrastInEeprom();

  SoftPWMSetPercent(contrastPin, contrast);
  SoftPWMSetPercent(lcdBrightnessPin, lcdBrightness);
  
  lcd.setCursor(firstLcdColPos, firstLcdRowPos);
  lcd.write(byte(starFormIndex));
  
  // F() is used for flash strings in order to consume less memory
  lcd.print(F("  WELCOME TO  "));
  lcd.write(byte(starFormIndex));
  
  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.write(byte(starFormIndex));
  lcd.print(F("  BOMBERMAN"));
  lcd.write(byte(bombFormIndex));
  lcd.print(F("  "));
  lcd.write(byte(starFormIndex));

  delay(gameNameDisplayTime);

  lcd.setCursor(firstLcdColPos, firstLcdRowPos);
  lcd.write(byte(starFormIndex));
  lcd.print(F("  CREATED BY  "));
  lcd.write(byte(starFormIndex));
  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.print(F(" SLEEPY STUDIOS "));

  delay(creatorNameDisplayTime);
  
  lcd.clear();
  enterMenu();
}

void duringTheMenuNavigation() {
    // each case represents a node in the main menu's graph
    switch(menuState) {
      case mainMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateMenuArrow();
          lastMoved = millis();
        }
        if (menuChanged == true) {
          // matrix display logic
           updateMenuDisplay();
           menuChanged = !menuChanged;
        }
        break;
        
      case setDifficultyMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateDifficulty();
          lastMoved = millis();
        }
        if (difficultyChanged == true) {
          // matrix display logic
          updateStartGameDifDisplay();
          difficultyChanged = !difficultyChanged;
        }
        break;

      case changingNameMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateChangeName();
          lastMoved = millis();
        }
        if (changeNameChanged == true) {
          // matrix display logic
          updateStartGameChangeNameDisplay();
          changeNameChanged = !changeNameChanged;
        }
        break;

      case highscoresMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateHighscoresArrow();
          lastMoved = millis();
        }
        if (highscoresChanged == true) {
          // matrix display logic
          updateHighscoresDisplay();
          highscoresChanged = !highscoresChanged;
        }
        break;

      case plotStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updatePlotArrow();
          lastMoved = millis();
        }
        if (plotChanged == true) {
          // matrix display logic
          updatePlotDisplay();
          plotChanged = !plotChanged;
        }
        break;

      case settingsMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateSettingsArrow();
          lastMoved = millis();
        }
        if (settingsChanged == true) {
          // matrix display logic
          updateSettingsDisplay();
          settingsChanged = !settingsChanged;
        }
        break;

      case aboutStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateAboutArrow();
          lastMoved = millis();
        }
        if (aboutChanged == true) {
          // matrix display logic
          updateAboutDisplay();
          aboutChanged = !aboutChanged;
        }
        break;

      // duringTheMenuNavigation, or case 7, is entered when the player starts a playthrough 
      // and every variable needs to be reinitialised or updated 
      case setupGameOnStartStateValue:
        lcd.clear();
        firstRound = true;
        hearts = heartsOnStart;

        currentNameIndex = currentNameIndexInitialValue;
        
        for (byte eepromIndex = eepromAdressForName; eepromIndex < startingValue; eepromIndex++) {
          EEPROM.update(eepromIndex, playerNameCharIndex[currentNameIndex]);
          currentNameIndex++;
        }
        
        lc.clearDisplay(matrixIndex);
        lcd.setCursor(firstLcdColPos, firstLcdRowPos);
        lcd.print(F("DODGE THE LEDS!"));
        testMenuOrGame = false;
        for (int row = firstMatrixRowPos; row < matrixSize; row++) {
          for (int col = firstMatrixColPos; col < matrixSize; col++) {
            matrix[row][col] = 0;
          }
        }
        xPos = firstMatrixRowPos;
        xLastPos = firstMatrixRowPos;
        yPos = firstMatrixColPos;
        yLastPos = firstMatrixColPos;
        counterLevelDuration = 0;
        matrixChanged = true;
        onLightedLed = 0;
        lastMoved = 0;
        if (matrixChanged == true) {
          // matrix display logic
          updateMatrix();
          matrixChanged = !matrixChanged;
        }
        break;

      case lcdBrightnessMEnuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateLcdBrightnessValue();
          lastMoved = millis();
        }
        if (lcdBrightnessChanged == true) {
          // matrix display logic
          updateLcdBrightnessDisplay();
          lcdBrightnessChanged = !lcdBrightnessChanged;
        }
        break;

      case contrastMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateContrastValue();
          lastMoved = millis();
        }
        if (contrastChanged == true) {
          // matrix display logic
          updateContrastDisplay();
          contrastChanged = !contrastChanged;
        }
        break;

      // gameEndDisplayMenuStateValue, or case 10, is the final display of the game results accordingly to how the player managed
      case gameEndDisplayMenuStateValue:
        lc.clearDisplay(matrixIndex);
        if (changeAfterGameDisplayOnce == true) {
          lcd.createChar(medalFormIndex, formsArray[medalFormIndex]);
          lcd.createChar(sadFaceFormIndex, formsArray[sadFaceFormIndex]);
          deadFace();
        
          lcd.clear();
          lcd.setCursor(firstLcdColPos, firstLcdRowPos);
          if (difficulty >= normal) {
            if (goodOrBad > noHighscore) {
              lcd.print(F("CONGRATS "));
              lcd.setCursor(9, firstLcdRowPos);
              for (byte index = 0; index < playerNameCharIndexSize; index++) {
                lcd.print(alphabet[playerNameCharIndex[index]]);
              }
              lcd.setCursor(14, firstLcdRowPos);
              lcd.print(F("!"));
            } else {
              lcd.print(F("TOO BAD "));
              lcd.setCursor(8, firstLcdRowPos);
              for (byte index = 0; index < playerNameCharIndexSize; index++) {
                lcd.print(alphabet[playerNameCharIndex[index]]);
              }
              lcd.setCursor(13, firstLcdRowPos);
              lcd.print(F("! "));
              lcd.setCursor(15, firstLcdRowPos);
              lcd.write(byte(sadFaceFormIndex));
            }
            lcd.setCursor(firstLcdColPos, secondLcdRowPos);
            if (goodOrBad == 0) {
              lcd.print(F("SCORE: "));
              lcd.setCursor(7, secondLcdRowPos);
              lcd.print(currentScore);
            } else if (goodOrBad == 1) {
              lcd.print(F("NEW TOP 1: "));
              lcd.setCursor(11, secondLcdRowPos);
              lcd.write(byte(medalFormIndex));
              lcd.setCursor(12, secondLcdRowPos);
              lcd.print(currentScore);
            } else if (goodOrBad == 2) {
              lcd.print(F("NEW TOP 2: "));
              lcd.setCursor(11, secondLcdRowPos);
              lcd.write(byte(medalFormIndex));
              lcd.setCursor(12, secondLcdRowPos);
              lcd.print(currentScore);
            } else if (goodOrBad == 3) {
              lcd.print(F("NEW TOP 3: "));
              lcd.setCursor(11, secondLcdRowPos);
              lcd.write(byte(medalFormIndex));
              lcd.setCursor(12, secondLcdRowPos);
              lcd.print(currentScore);
            }
          } else {
            lcd.print(F("CONGRATS "));
            lcd.setCursor(9, firstLcdRowPos);
            for (byte index = 0; index < playerNameCharIndexSize; index++) {
              lcd.print(alphabet[playerNameCharIndex[index]]);
            }
            lcd.setCursor(14, firstLcdRowPos);
            lcd.print(F("!"));
            lcd.setCursor(firstLcdColPos, secondLcdRowPos);
            lcd.print(F("SCORE: "));
            lcd.setCursor(7, secondLcdRowPos);
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

        // returning to main menu
        if (millis() - lastDebounceTime > debounceDelay) { 
          if (previousReading != reading){
            buttonState = reading;
            if (buttonState == LOW){
              NewTone (buzzerPin, backToMenuButtonBuzzerTone);
              delay (arrowMoveToneTimer);
              changeAfterGameDisplayOnce = true;
              lc.clearDisplay(matrixIndex);
              updateMenuDisplay();
              menuState = mainMenuStateValue;
            }
          }   
        }
        
        previousReading = reading;
        break;

      case matrixBrightnessMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateMatrixBrightnessValue();
          lastMoved = millis();
        }
        if (matrixBrightnessChanged == true) {
          // matrix display logic
          updateMatrixBrightnessDisplay();
          matrixBrightnessChanged = !matrixBrightnessChanged;
        }
        break;

      case resetHighscoresMenuStateValue:
        if (millis() - lastMoved > moveInterval) {
          // game logic
          updateResetHighscores();
          lastMoved = millis();
        }
        if (resetHighscoresChanged == true) {
          // matrix display logic
          resetHighscoresDisplay();
          resetHighscoresChanged = !resetHighscoresChanged;
        }
        break;
        
      default:
        break;
    }
}

// uses a seed generated using pin 12 that creates a random value for randSquareSideLength depending on the difficulty selected by the player.
// The function then generates squares of random dimensions on random positions. The function makes sure the matrix is never entirely lit or that the squares do not 
// overlap. The function tries to find coordinates to place squares and stops after a number of failed tryings
void levelMatrixFormsGenerator() {
      byte rowBegin;
      byte colBegin;
      byte randSquareSideLength;
      switch (difficulty) {
        // 1 is added because the interval for random is [ , )
        case superEasy:  
          // each max<digit> represents the maximum number of squares with the side length of <digit> capable of being added on the screen
          randSquareSideLength = random(minSquareSideLength, maxSquareSideLengthForSuperEasy + 1);     // random(2, 4);
          max2 = 6;
          max3 = 2;
          break;
        case easy:  
          randSquareSideLength = random(minSquareSideLength, maxSquareSideLengthForEasy + 1);     // random(2, 5);
          max2 = 6;
          max3 = 4;
          break;
          
        case normal:
          randSquareSideLength = random(minSquareSideLength, maxSquareSideLengthForNormalAndHard + 1);;   // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;
          
        case hard: 
          randSquareSideLength = random(minSquareSideLength, maxSquareSideLengthForNormalAndHard + 1);     // random(2, 8);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 1;
          max6 = 1;
          max7 = 1;
          break;
          
        default:  //godMode
          randSquareSideLength = random(minSquareSideLength, maxSquareSideLengthForGodMode + 1);     // random(2, 5);
          max2 = 16;
          max3 = 4;
          max4 = 4;
          max5 = 0;
          max6 = 0;
          max7 = 0;
          break;

      }
      onLightedLed = false;
      while (randSquareSideLength != 1) {
        switch (randSquareSideLength) {
          case squareWithSideLengthOf2:
            
            for (int counter = 1; counter <= max2; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 7);
                  colBegin = random(firstMatrixColPos, 7);
                  // tests if the new square will stack onto another
                  for(int testLin = rowBegin; testLin <= rowBegin + 1; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 1; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  // tests if there is enough room for a new square
                  if (availabilityTest == false || noOfLightedLeds + squareWithSideLengthOf2 * squareWithSideLengthOf2 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += squareWithSideLengthOf2 * squareWithSideLengthOf2;
                    for(int row = rowBegin; row <= rowBegin + 1; row++) {
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
            randSquareSideLength = 1;  
            availabilityTest = true;
            break;
          case squareWithSideLengthOf3:
            
            for (int counter = 1; counter <= max3; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 6);
                  colBegin = random(firstMatrixColPos, 6);
                  
                  for(int testLin = rowBegin; testLin <= rowBegin + 2; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 2; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false || noOfLightedLeds + squareWithSideLengthOf3 * squareWithSideLengthOf3 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += squareWithSideLengthOf3 * squareWithSideLengthOf3;
                    for(int row = rowBegin; row <= rowBegin + 2; row++) {
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
            randSquareSideLength = 2;
            availabilityTest = true;
            break;
            
          case squareWithSideLengthOf4:
            for (int counter = 1; counter <= max4; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 5);
                  colBegin = random(firstMatrixColPos, 5);
                  for(int testLin = rowBegin; testLin <= rowBegin + 3; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 3; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false || noOfLightedLeds + squareWithSideLengthOf4 * squareWithSideLengthOf4 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += squareWithSideLengthOf4 * squareWithSideLengthOf4;
                    for(int row = rowBegin; row <= rowBegin + 3; row++) {
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
            randSquareSideLength = 3;
            availabilityTest = true;
            break;
            
          case squareWithSideLengthOf5:
            for (int counter = 1; counter <= max5; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 4);
                  colBegin = random(firstMatrixColPos, 4);
                  for(int testLin = rowBegin; testLin <= rowBegin + 4; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 4; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false || noOfLightedLeds + squareWithSideLengthOf5 * squareWithSideLengthOf5 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += squareWithSideLengthOf5 * squareWithSideLengthOf5;
                    for(int row = rowBegin; row <= rowBegin + 4; row++) {
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
            randSquareSideLength = 3;
            break;
            
          case squareWithSideLengthOf6:
            for (int counter = 1; counter <= max6; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 3);
                  colBegin = random(firstMatrixColPos, 3);
                  for(int testLin = rowBegin; testLin <= rowBegin + 5; testLin++) {
                    for(int testCol = colBegin; testCol <= colBegin + 5; testCol++) {
                      if (matrix[testLin][testCol] == 1 && testLin != xPos && testLin != yPos) {
                        availabilityTest = false;
                        break;
                      }
                    }
                  }
                  if (availabilityTest == false || noOfLightedLeds + squareWithSideLengthOf6 * squareWithSideLengthOf6 >= noOfLeds) {
                    noOfFailedTryings++;
                  } else {
                    noOfLightedLeds += squareWithSideLengthOf6 * squareWithSideLengthOf6;
                    for(int row = rowBegin; row <= rowBegin + 5; row++) {
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
            randSquareSideLength = 2;
            break;
            
          default:  // case squareWithSideLengthOf7
            for (int counter = 1; counter <= max7; counter++) {
              if (availabilityTest == true) {
                noOfFailedTryings = 0;
                availabilityTest = false;
                while (noOfFailedTryings <= noOfTryings && availabilityTest == false) {
                  availabilityTest = true;
                  rowBegin = random(firstMatrixRowPos, 2);
                  colBegin = random(firstMatrixColPos, 2);
                  for(int testLin = rowBegin; testLin <= rowBegin + 6; testLin++) {
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
                    for(int row = rowBegin; row <= rowBegin + 6; row++) {
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
            randSquareSideLength = 1;
            break;
        }
  
        updateMatrix();
      }
      // rounds become shorter and shorter depending on the difficulty
      if (difficulty == 4) {
        counterLevelBasedOnDiff += 3;
      } else if (difficulty == 3) {
        counterLevelBasedOnDiff += 2;
      } else if (difficulty == 2) {
        counterLevelBasedOnDiff += 1;
      }
}

// sets values in eeprom to 0 and updates the variables used during the respective run
void resetHighscoresDisplay() {
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);

  lcd.clear();
  lcd.setCursor(firstLcdColPos, firstLcdRowPos);
  lcd.write(byte(emptyCellFormIndex));
  lcd.setCursor(2, firstLcdRowPos);
  lcd.print(F("ARE YOU SURE?"));
  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.write(byte(emptyCellFormIndex));
  
  if (yResetHighscoresArrowPos == 0) {
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(11, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    
  } else if (yResetHighscoresArrowPos == 1){
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(11, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
  } 
  lcd.setCursor(1, secondLcdRowPos);
  lcd.print(resetHighscoresArray[0]);
  lcd.setCursor(12, secondLcdRowPos);
  lcd.print(resetHighscoresArray[1]); 
}



// updates the position of the hovering arrow and the values of the name
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
          NewTone (buzzerPin, enterButtonBuzzerTone);
          delay (arrowMoveToneTimer);
          switchStatesDuringNameChangeArrowDisplay = 3;
          upOrDown = true;
          menuState = setupGameOnStartStateValue;
        } else {
          stateChangeName = !stateChangeName;
          if (stateChangeName != true) {
            switchStatesDuringNameChangeArrowDisplay = 4;
          } else {
            switchStatesDuringNameChangeArrowDisplay = 3;
          }
          updateStartGameChangeNameDisplay();
        }
      }
    }   
  }
  
  previousReading = reading;
  byte lastNameIndex = nameIndex;
  byte lastPlayerNameCharIndex = playerNameCharIndex[nameIndex];
  
  if (stateChangeName == true) {
    if (yValue < maxThreshold) {
      if (nameIndex < playerNameCharIndexSize + 1) {
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
      }
    }
  
    if (xValue < minThreshold) {
      if (playerNameCharIndex[nameIndex] < noOfLetters - 2) {
        playerNameCharIndex[nameIndex]++;
      } else {
        playerNameCharIndex[nameIndex] = 0;
      }
    }
  }
  if (lastNameIndex != nameIndex) {
    NewTone (buzzerPin, arrowBuzzerTone);
    changeNameChanged = true;
    updateStartGameChangeNameDisplay();
  } else if (playerNameCharIndex[nameIndex] != lastPlayerNameCharIndex) {
    NewTone (buzzerPin, letterChangeBuzzerTone);
    changeNameChanged = true;
    updateStartGameChangeNameDisplay();
  } 
  
  
}



// displays depending on the hovering arrow's position and placement on the rows of the lcd
void updateMenuDisplay() {
  lcd.clear();
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);
  if (upOrDown == true) {
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(secondLcdColPos, row);
      lcd.print(menuList[xMenuArrowPos + row]);
    }
  } else {
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(secondLcdColPos, row - 1);
      lcd.print(menuList[xMenuArrowPos - row]);
    }
  } 
}

// initialises the first frame of the difficulty display
void updateStartGameDifDisplay() {
  lcd.clear();
  lcd.createChar(upAndDownArrowFormIndex, formsArray[upAndDownArrowFormIndex]);

  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.write(byte(upAndDownArrowFormIndex));
  
  lcd.setCursor(secondLcdColPos, firstLcdRowPos);
  lcd.print(F("SET DIFFICULTY"));

  lcd.setCursor(secondLcdColPos, secondLcdRowPos);
  lcd.print(difficulties[difficulty]);
}


// updates the value of the difficulty
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
        NewTone (buzzerPin, enterButtonBuzzerTone);
        switch (difficulty) {
          case 2:
            counterLevelBasedOnDiff = 15;
            break;
            
          case 3:
            counterLevelBasedOnDiff = 20;
            break;
            
          case 4: 
            counterLevelBasedOnDiff = 30;
            break;

          default:
            counterLevelBasedOnDiff = 15;
            break;
        }
        
        upOrDown = true;
        updateStartGameChangeNameDisplay();
        menuState = changingNameMenuStateValue;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastDifficulty = difficulty;
  
  if (xValue < maxThreshold) {
    if (difficulty < godMode + 1) {
      difficulty++;
    } 
  }

  if (xValue > minThreshold) {
    if (difficulty > superEasy) {
      difficulty--;
    }
  }

  if (difficulty != lastDifficulty) {
    NewTone (buzzerPin, arrowBuzzerTone);
    difficultyChanged = true;
    
  }
  
}


// updates the position of the hovering arrow and the options presented
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
        NewTone (buzzerPin, enterButtonBuzzerTone);
        switch(xMenuArrowPos) {
          case menuArrowStartGamePos:
            upOrDown = true;
            menuState = setDifficultyMenuStateValue;
            updateStartGameDifDisplay();
            break;

          case menuArrowHighscoresPos:
            upOrDown = true;
            // highscores function
            menuState = highscoresMenuStateValue;
            updateHighscoresDisplay();
            break;
            
          case menuArrowPlotPos:
            upOrDown = true;
            // plot function
            menuState = plotStateValue;
             updatePlotDisplay();
            break;

          case menuArrowSettingsGamePos:
            upOrDown = true;
            // settings function
            menuState = settingsMenuStateValue;
            updateSettingsDisplay();
            break;

          case menuArrowAboutGamePos:
            upOrDown = true;
            // about function
            menuState = aboutStateValue;
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
    NewTone (buzzerPin, arrowBuzzerTone);

    if (xMenuArrowPos > xMenuArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    
  }
  
}


// keeps truning on and off the led where the player is positioned so that it makes a blinking pattern
void updatePlayerLed() {
  if (playerInterval >= playerIntervalMax) {
    playerInterval = 0;
    playerLight = !playerLight;
    lc.setLed(0, xPos, yPos, playerLight);
  }
}


// ruens on and off the leds according to the matrix of 1s and 0s
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}


// player movement function
void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  
  xLastPos = xPos;
  yLastPos = yPos;

  // tests if the player moved the joystick's potentiometers to pass the maximum or minimum values set
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
  // tests if the player was on a lighted led, if so, it lets it ligthed, otherwise it turns the led off
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

    // doesn't let the player move in a direction if she/he has no moves left for it
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

    matrix[xPos][yPos] = 1;
    
  }
  
}


void updateMatrixBrightnessInEeprom() {
  EEPROM.update(matrixBrightnessEepromAdress, matrixBrightness);
}


// animates and displays the tab where the matrix's brightness is changed
void updateMatrixBrightnessDisplay() {
  lcd.clear();
  lcd.createChar(upAndDownArrowFormIndex, formsArray[upAndDownArrowFormIndex]);
  lcd.createChar(fullCellFormIndex, formsArray[fullCellFormIndex]);

  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.write(byte(upAndDownArrowFormIndex));
  
  lcd.setCursor(secondLcdColPos, firstLcdRowPos);
  lcd.print(F("SET BRIGHTNESS"));

  lcd.setCursor(secondLcdColPos, secondLcdRowPos);
  switch(matrixBrightness) {
    case 12:
      matrixBrightnessShowValue = 4;
      break;

    case 8:
      matrixBrightnessShowValue = 3;
      break;

    case 5:
      matrixBrightnessShowValue = 2;
      break;

    case 2:
      matrixBrightnessShowValue = 1;
      break;

    default: 
      matrixBrightnessShowValue = 5;
      break;
  }
  for (int index = 1; index <= matrixBrightnessShowValue; index++) {
    lcd.setCursor(index, secondLcdRowPos);
    lcd.write(byte(fullCellFormIndex));
  }

  // a 2x2 led square will be lit in the middle of the matrix
  for (int row = 3; row < 5; row++) {
    for (int col = 3; col < 5; col++) {
      lc.setLed(matrixIndex, row, col, true);
    }
  }
  
}


// for this function, updateLcdBrightnessValue and updateContrastValue() there were some values predefined 
// for the variables to take so that the values will not make the display unreadable
void updateMatrixBrightnessValue() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        lc.clearDisplay(matrixIndex);
        NewTone (buzzerPin, backToMenuButtonBuzzerTone);
        updateMatrixBrightnessInEeprom();
        updateSettingsDisplay();
        menuState = settingsMenuStateValue;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastMatrixBrightness = matrixBrightness;
  
  if (xValue < minThreshold) {
    if (matrixBrightness < 15) {
      switch(matrixBrightness) {
        case 2:
          matrixBrightness = 5;
          break;
        case 5:
          matrixBrightness = 8;
          break;
        case 8:
          matrixBrightness = 12;
          break;
        case 12:
          matrixBrightness = 15;
          break;
        default:
          break;
      }
    } 
  }

  if (xValue > maxThreshold) {
    if (matrixBrightness > 2) {
      switch(matrixBrightness) {
        case 15:
          matrixBrightness = 12;
          break;
        case 12:
          matrixBrightness = 8;
          break;
        case 8:
          matrixBrightness = 5;
          break;
        case 5:
          matrixBrightness = 2;
          break;
        default:
          break;
      }
    }
  }

  if (matrixBrightness != lastMatrixBrightness) {
    NewTone (buzzerPin, arrowBuzzerTone);
    matrixBrightnessChanged = true;
    DisplayAndSetMatrixBrightness();
  } 
}



void updateLcdBrightnessDisplay() {
  lcd.clear();
  lcd.createChar(upAndDownArrowFormIndex, formsArray[upAndDownArrowFormIndex]);
  lcd.createChar(fullCellFormIndex, formsArray[fullCellFormIndex]);

  lcd.setCursor(0, 1);
  lcd.write(byte(upAndDownArrowFormIndex));
  
  lcd.setCursor(1, 0);
  lcd.print(F("SET BRIGHTNESS"));

  lcd.setCursor(1, 1);
  switch(lcdBrightness) {
    case 80:
      lcdBrightnessShowValue = 4;
      break;

    case 60:
      lcdBrightnessShowValue = 3;
      break;

    case 35:
      lcdBrightnessShowValue = 2;
      break;

    case 10:
      lcdBrightnessShowValue = 1;
      break;

    default: 
      lcdBrightnessShowValue = 5;
      break;
  }
  for (int index = 1; index <= lcdBrightnessShowValue; index++) {
    lcd.setCursor(index, secondLcdRowPos);
    lcd.write(byte(fullCellFormIndex));
  }
  
}


void updateLcdBrightnessValue() {
  int xValue = analogRead(xPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        NewTone (buzzerPin, backToMenuButtonBuzzerTone);
        updateLcdBrightnessInEeprom();
        upOrDown = true;
        updateSettingsDisplay();
        menuState = settingsMenuStateValue;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastLcdBrightness = lcdBrightness;
  
  if (xValue < minThreshold) {
    if (lcdBrightness < highestLcdBrightnessValue) {
      switch(lcdBrightness) {
        case 10:
          lcdBrightness = 35;
          break;
        case 35:
          lcdBrightness = 60;
          break;
        case 60:
          lcdBrightness = 80;
          break;
        case 80:
          lcdBrightness = 100;
          break;
        default:
          break;
      }
    } 
  }

  if (xValue > maxThreshold) {
    if (lcdBrightness > lowestLcdBrightnessValue) {
      switch(lcdBrightness) {
        case 100:
          lcdBrightness = 80;
          break;
        case 80:
          lcdBrightness = 60;
          break;
        case 60:
          lcdBrightness = 35;
          break;
        case 35:
          lcdBrightness = 10;
          break;
        default:
          break;
      }
    }
  }

  if (lcdBrightness != lastLcdBrightness) {
    NewTone (buzzerPin, arrowBuzzerTone);
    lcdBrightnessChanged = true;
    DisplayAndSetLcdBrightness();
  } 
}



void updateContrastDisplay() {
  lcd.clear();
  lcd.createChar(upAndDownArrowFormIndex, formsArray[upAndDownArrowFormIndex]);
  lcd.createChar(fullCellFormIndex, formsArray[fullCellFormIndex]);

  lcd.setCursor(0, 1);
  lcd.write(byte(upAndDownArrowFormIndex));
  
  lcd.setCursor(1, 0);
  lcd.print(F("SET CONTRAST"));

  lcd.setCursor(1, 1);
  switch(contrast) {
    case 5:
      contrastShowValue = 4;
      break;

    case 35:
      contrastShowValue = 3;
      break;

    case 40:
      contrastShowValue = 2;
      break;

    case 45:
      contrastShowValue = 1;
      break;

    default: 
      contrastShowValue = 5;
      break;
  }
  for (int index = 1; index <= contrastShowValue; index++) {
    lcd.setCursor(index, secondLcdRowPos);
    lcd.write(byte(fullCellFormIndex));
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
        NewTone (buzzerPin, backToMenuButtonBuzzerTone);
        updateContrastInEeprom();
        upOrDown = false;
        updateSettingsDisplay();
        menuState = settingsMenuStateValue;
        
      } 
    }   
  }
  
  previousReading = reading;

  
  lastContrast = contrast;
  
  if (xValue > maxThreshold) {
    if (contrast < highestContrastValue) {
      switch(contrast) {
        case 0:
          contrast = 5;
          break;
        case 5:
          contrast = 35;
          break;
        case 35:
          contrast = 40;
          break;
        case 40:
          contrast = 45;
          break;
      }
    } 
  }

  if (xValue < minThreshold) {
    if (contrast > lowestContrastValue) {
      switch(contrast) {
        case 45:
          contrast = 40;
          break;
        case 40:
          contrast = 35;
          break;
        case 35:
          contrast = 5;
          break;
        case 5:
          contrast = 0;
          break;
        default:
          break;
      }
    }
  }

  if (contrast != lastContrast) {
    NewTone (buzzerPin, arrowBuzzerTone);
    contrastChanged = true;
    DisplayAndSetContrast();
  } 
}


void updateSettingsDisplay() {
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(0, firstLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(settings[xSettingsArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(secondLcdColPos, row - 1);
      lcd.print(settings[xSettingsArrowPos - row]);
    }
  } 
}

// simmilar to the function of the main menu hover arrow, it changes the position in the menu accordingly to the actions of the player
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
        if(xSettingsArrowPos == 0) {
          NewTone (buzzerPin, enterButtonBuzzerTone);
        } else {
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
        }
        switch(xSettingsArrowPos) {
          case settingsArrowBackToMenuPos:
            upOrDown = true;
            menuState = mainMenuStateValue;
            updateMenuDisplay();
            break;

          case settingsArrowLcdBrightnessPos:
            upOrDown = true;
            // lcd brightness function
            menuState = lcdBrightnessMEnuStateValue;
            updateLcdBrightnessDisplay();
            break;
            
          case settingsArrowContrastPos:
            upOrDown = true;
            // contrast function
            menuState = contrastMenuStateValue;
            updateContrastDisplay();
            break;

          case settingsArrowMatrixBrightnessPos:
            upOrDown = true;
            // matrix brightness function
            for (int row = 3; row < 5; row++) {
              for (int col = 3; col < 5; col++) {
                lc.setLed(0, row, col, true);
              }
            }
              
            menuState = matrixBrightnessMenuStateValue;
            updateMatrixBrightnessDisplay();
            break;

          case settingsArrowResetHighscoresPos:
            upOrDown = true;
            // matrix brightness function              
            menuState = resetHighscoresMenuStateValue;
            resetHighscoresDisplay();
            break;
            
          default:
            break;

        }
        if (xSettingsArrowPos == 0) {
          menuState = mainMenuStateValue;
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
    NewTone (buzzerPin, arrowBuzzerTone);
    settingsChanged = true;
    if (xSettingsArrowPos > xSettingsArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    
  }
  
}





void updateHighscoresDisplay() {
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(highscores[xHighscoresArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(secondLcdColPos, row - 1);
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
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
          menuState = mainMenuStateValue;
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
    NewTone (buzzerPin, arrowBuzzerTone);
    highscoresChanged = true;
    if (xHighscoresArrowPos > xHighscoresArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
    
  }
  
}



void updateAboutDisplay() {
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    for( int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(1, row);
      lcd.print(about[xAboutArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    for( int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(secondLcdColPos, row - 1);
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
          menuState = mainMenuStateValue;
          upOrDown = false;
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
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
    NewTone (buzzerPin, arrowBuzzerTone);
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
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
          menuState = mainMenuStateValue;
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
    NewTone (buzzerPin, arrowBuzzerTone);
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
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);

  lcd.setCursor(firstLcdColPos, secondLcdRowPos);
  lcd.write(byte(menuHoverArrowFormIndex));

  lcd.setCursor(secondLcdColPos, firstLcdRowPos);
  lcd.print(F("BACK TO MENU"));
  
  lcd.setCursor(secondLcdColPos, secondLcdRowPos);
  lcd.print(plot[1]);
  
  if (upOrDown == true) {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    for (int row = 0; row < noOfLcdRows; row++) {
      lcd.setCursor(secondLcdColPos, row);
      lcd.print(plot[xPlotArrowPos + row]);
    }
  } else {
    lcd.clear();
    lcd.setCursor(firstLcdColPos, firstLcdRowPos);
    lcd.write(byte(emptyCellFormIndex));
    lcd.setCursor(firstLcdColPos, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
    for (int row = noOfLcdRows - 1; row >= 0; row--) {
      lcd.setCursor(secondLcdColPos, row - 1);
      lcd.print(plot[xPlotArrowPos - row]);
    }
  } 
}


// displays the firs frame a player sees after entering the main menu
void enterMenu() {
  lcd.createChar(menuHoverArrowFormIndex, formsArray[menuHoverArrowFormIndex]);

  upOrDown = true;
  lcd.setCursor(firstLcdColPos, firstLcdRowPos);
  lcd.write(byte(menuHoverArrowFormIndex));
  lcd.print(F("START GAME"));
  lcd.setCursor(secondLcdColPos, secondLcdRowPos);
  lcd.print(F("HIGHSCORES"));

}


// makes sure the player did not press "RES SCOREBOARD" accidentally, so it asks for his/her permission to
// delete the old values on the scoreboard
void updateResetHighscores() {
  int yValue = analogRead(yPin);
  reading = digitalRead(swPin);

  if (reading  != previousReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    if (previousReading != reading){
      buttonState = reading;
      if (buttonState == LOW){
        if (yResetHighscoresArrowPos == 0) {
          menuState = settingsMenuStateValue;
          upOrDown = false;
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
          updateSettingsDisplay();
        } else {
          menuState = settingsMenuStateValue;
          yResetHighscoresArrowPos = 0;
          NewTone (buzzerPin, backToMenuButtonBuzzerTone);
          resetAllHighscoresToZero();
          updateSettingsDisplay();
        }
      } 
    }   
  }
  
  previousReading = reading;
  
  yResetHighscoresArrowLastPos = yResetHighscoresArrowPos;
  if (yValue > minThreshold) {
    if (yResetHighscoresArrowPos < resetHighscoresArrowArraySize - 1) {
      yResetHighscoresArrowPos++;
    } 
    else {
      yResetHighscoresArrowPos = 0;
    }
    
  }

  if (yValue < maxThreshold) {
    if (yResetHighscoresArrowPos > 0) {
      yResetHighscoresArrowPos--;
    }
    else {
      yResetHighscoresArrowPos = resetHighscoresArrowArraySize - 1;
    }
  }

  if (yResetHighscoresArrowPos != yResetHighscoresArrowLastPos) {
    NewTone (buzzerPin, arrowBuzzerTone);
    resetHighscoresChanged = true;
    if (yResetHighscoresArrowPos > yResetHighscoresArrowLastPos) {
      upOrDown = false;
    } else {
      upOrDown = true;
    }
  }
}


// it stores in eeprom the name of the player, on the first 5 positions and
// the highscores and names for each difficulty on the next 83 positions 
void updateHighscoresAndNameInEeprom(byte multiplyingIndex, int newTopValue) {
    const byte multiplyingConstant = 9;
    const byte startingValue = 5;
    const byte nameLength = 5;
    const byte scoreLength = 4;
    currentNameIndex = 0;
    
    
    for (byte eepromIndex = startingValue + (multiplyingIndex * multiplyingConstant); eepromIndex < startingValue + (multiplyingIndex * multiplyingConstant) + nameLength; eepromIndex++) {
      EEPROM.update(eepromIndex, playerNameCharIndex[currentNameIndex]);
      currentNameIndex++;
    }
    
    int dividingFactor = fitrstMultiplyingFactorForScore;
    for (byte eepromIndex = startingValue + (multiplyingIndex * multiplyingConstant) + nameLength; eepromIndex < startingValue + (multiplyingIndex * multiplyingConstant) + nameLength + scoreLength; eepromIndex++) {
      EEPROM.update(eepromIndex, (newTopValue / dividingFactor) % multiplyingAndDividingFactorGap);
      dividingFactor /= multiplyingAndDividingFactorGap;
    }
    
}


void readBrightnessAndContrastInEeprom() {
  lcdBrightness = EEPROM.read(lcdBrightnessEepromAdress);
  contrast =  EEPROM.read(contrastEepromAdress);
  matrixBrightness = EEPROM.read(matrixBrightnessEepromAdress);
  
}

void updateLcdBrightnessInEeprom() {
  EEPROM.update(lcdBrightnessEepromAdress, lcdBrightness);
}


void updateContrastInEeprom() {
  EEPROM.update(contrastEepromAdress, contrast);
}

// updates the display for the "CHANGE MENU" tab
void updateStartGameChangeNameDisplay() {
  lcd.clear();
  lcd.createChar(menuHoverArrowFormIndex, formsArray[switchStatesDuringNameChangeArrowDisplay]);
  lcd.createChar(checkFormIndex, formsArray[checkFormIndex]);
  lcd.createChar(emptyCellFormIndex, formsArray[emptyCellFormIndex]);
  
  lcd.setCursor(firstLcdColPos, firstLcdRowPos);
  lcd.print(F("    SET NAME    "));
  if (nameIndex <= playerNameCharIndexSize) {
    for (int index = firstLetterPosDisplay; index < lastLetterPosDisplay; index += letterDisplayInterval) {
      lcd.setCursor(index, secondLcdRowPos);
      lcd.write(byte(emptyCellFormIndex));
    }
    lcd.setCursor(nameIndex * nameIndexPosMultiplyer + letterDisplayInterval, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
  } else {
    lcd.setCursor(nameIndex * nameIndexPosMultiplyer, secondLcdRowPos);
    lcd.write(byte(menuHoverArrowFormIndex));
  }
  byte col = 3;
  for (int index = firstPlayerNameCharIndex; index < playerNameCharIndexSize; index++) {
    lcd.setCursor(col, secondLcdRowPos);
    col += letterDisplayInterval;
    lcd.print(alphabet[playerNameCharIndex[index]]);
  }
  lcd.setCursor(col, secondLcdRowPos);
  lcd.write(byte(checkFormIndex));
}

// not only reads the values from eeprom but also stores them in the variables used
// to display the tab in "HIGHSCORES"
void readAndSetHighscoresVariablesFromEeprom() {
  byte eepromAdress = eepromAdressForHighscores;
  String auxWrite = emptyString;
  byte currentTopIndex = currentTopIndexFirstValue;
  for (int index = 0; index < noOfHighscores; index++) {
    for (int nameAuxCounter = 0; nameAuxCounter < playerNameCharIndexSize; nameAuxCounter++) {
      auxWrite += alphabet[EEPROM.read(eepromAdress)];        
      eepromAdress++;
    }
    
    auxWrite += spaceBetweenNameAndScore;
    int multiplyingFactor = fitrstMultiplyingFactorForScore;
    for (int scoreAuxCounter = highscoresMenuScoreFirstPos; scoreAuxCounter < highscoresMenuScoreLastPos; scoreAuxCounter++) {
      auxWrite += String(EEPROM.read(eepromAdress));
      currentTop[currentTopIndex] += multiplyingFactor * int(EEPROM.read(eepromAdress));
      multiplyingFactor /= multiplyingAndDividingFactorGap;
      eepromAdress++;
    }
    currentTopIndex++;
    top[index] = auxWrite;
    auxWrite = "";
  }

  byte highscoresIndex = 2;
  byte topIndex = 0;
  for (int index = 0; index < noOfHighscoresForEachDiff; index++) {
    for (int scoresAuxIndex = 0; scoresAuxIndex < noOfHighscoresForEachDiff; scoresAuxIndex++) {
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

// sets highscores values and names in eeprom to 0 then updates them for this run also
void resetAllHighscoresToZero() {
  for (int index = eepromFirstHighscoresAdress; index <= eepromLastHighscoresAdress; index++) {
    EEPROM.update(index, 0);
  }
  readAndSetHighscoresVariablesFromEeprom();
}

void DisplayAndSetContrast() {  
  if (contrast >= lowestContrastValue && contrast <= highestContrastValue) {
    SoftPWMSetPercent(contrastPin, contrast);
  }
}

void DisplayAndSetLcdBrightness() {  
  if (lcdBrightness >= lowestLcdBrightnessValue && lcdBrightness <= highestLcdBrightnessValue) {
    SoftPWMSetPercent(lcdBrightnessPin, lcdBrightness);
  }
}

void DisplayAndSetMatrixBrightness() {
  lc.setIntensity(0, matrixBrightness);
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
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 6, 2, true);
  lc.setLed(0, 7, 2, true);
  lc.setLed(0, 7, 3, true);
  lc.setLed(0, 7, 4, true);
  lc.setLed(0, 7, 5, true);
  lc.setLed(0, 6, 5, true);
  lc.setLed(0, 6, 6, true);
  lc.setLed(0, 5, 6, true);
  
  NewTone (buzzerPin, NOTE_D6);
  delay (shortNoteDuration);
  NewTone (buzzerPin, NOTE_C6);
  delay (shortNoteDuration);
  NewTone (buzzerPin, NOTE_F6);
  delay (mediumNoteDuration);
  noNewTone (buzzerPin);
  if (difficulty < 4) {
    delay(900);
  } else {
    delay(200);
  }
}


//Sad Face
void sadFace() {
  int i = 0, j = 0;
  for (i = 1; i <= 3; i++) {
    for (j = 1; j <= 2; j++) {
      lc.setLed(0, i, j, true);
      lc.setLed(0, i, j + 4, true);
    }
  }
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 6, 6, true);
  NewTone (buzzerPin, NOTE_FS5);
  delay (shortNoteDuration);
  NewTone (buzzerPin, NOTE_D5);
  delay (shortNoteDuration);
  NewTone (buzzerPin, NOTE_AS4);
  delay (mediumNoteDuration);
  noNewTone (buzzerPin);
  if (difficulty < 4) {
    delay(900);
  } else {
    delay(200);
  }
}

//Dead Face
void deadFace() {
  int i = 0, j = 2;
  for (i = 0; i <= 2; i++) {
    lc.setLed(0, i + 1, i, true);
    lc.setLed(0, i + 1, i + 5, true);
    lc.setLed(0, j + 1, i, true);
    lc.setLed(0, j + 1, i + 5, true);
    j--;
  }
  lc.setLed(0, 6, 1, true);
  lc.setLed(0, 5, 2, true);
  lc.setLed(0, 5, 3, true);
  lc.setLed(0, 5, 4, true);
  lc.setLed(0, 5, 5, true);
  lc.setLed(0, 6, 6, true);
  
  NewTone (buzzerPin, NOTE_E4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_A4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_C5);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_B4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_A4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_C5);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_A4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_B4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_A4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_F4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_G4);
  delay (mediumNoteDuration);
  NewTone (buzzerPin, NOTE_E4);
  delay (longNoteDuration);
  
  noNewTone (buzzerPin);
  
}
