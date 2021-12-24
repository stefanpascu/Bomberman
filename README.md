# Bomberman
### Description
#### Playthrough
  The player must dodge the lighted leds as they change faster and faster, and also try to move as little as possible.
<br>
#### Menu
  The player can modify contrast and brightness displayed, can browse and reset old highscores and change the game's difficulty and his/her own name.
<br>
### Youtube Link: https://youtu.be/Kq-6B44mNdg
<br>

<img src="https://user-images.githubusercontent.com/41235115/147303565-d1d8c525-2a04-43aa-b992-c61254b40229.jpeg" data-canonical-src="https://user-images.githubusercontent.com/41235115/147303565-d1d8c525-2a04-43aa-b992-c61254b40229.jpeg" width="300" height="600" />

<br><br>

  ### Components: LCD, Joystick, Buzzer, Potentiometer and 8x8 Led Matrix.
  <details>
  
<summary>Descriptions</summary>
  
#### 1. LCD
  It is used to display the menu and all the functionalities it posseses. It also presents during the game the following: player's remaining lives, player's score and the number of moves he is allowed to perform.

<br>
  
#### 2. Joystick
  It controls the entirety of the user's actions together with its two potentiometers and button. It allows the player to navigate through the menu and move throughout the led matrix while playing

<br>

#### 3. Buzzer
  It signals the player's moves throughout the LCD and led matrix and also brings new sounds when the player is rewarded or punished.

<br>
  
#### 3. Potentiometer
  It controls the buzzers volume, unfortunately it does not use a mapped value so it is not optimised for all its rotation.

<br>
  
#### 4. Led Matrix
  It displays the game during playtime and prints levels, the player and different emojis to reflect the player's actions

  </details>
  
<br><br>

  ### Important Functions: prepareSetupOnStart(), 
  <details>
  
<summary></summary>
  
#### 1. prepareSetupOnStart()
```C++
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
```
This function represents an intro, displays the name of the game, the name of the creator and reads variables from EEPROM to set the Scoreboard, contrast, LCD brightness and Matrix brightness. It is used only once, after the game is restarted.

<br>

#### 2. duringTheMenuNavigation()
  
```C++
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
```
  
```C++     
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
```
                                                             
```C++
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
```
 
```C++
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
```

This function is one of the most important as it takes care of the entire menu. With the help from other smaller functions it is able to generate and display the user's navigation throughout the multitude of main and secondary branches. It mainly uses a "Switch-Case" for each menu state. For example, "resetHighscoresMenuStateValue" is a constant with the value of 12. If the variable menuState has its value it will only call the functions related to it. For most of the states, the content inside each case is similar to one another, but for setupGameOnStartStateValue and gameEndDisplayMenuStateValue, with the values of 7 and 10, it is relatively more complicated as they make the connection between the browseable LCD menu and the game itself which is played on the 8x8 led matrix. If menuState is equal to setupGameOnStartStateValue, the variables used during playtime will be initialised and the LCD will now display information related to the current game. If menuState  is equal to gameEndDisplayMenuStateValue, a final display with information will inform the player about his/her performance during the last playthrough and after a button press the player will return to the main menu.
  
<br>

  
#### 3. levelMatrixFormsGenerator()
```C++
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
```
  
"levelMatrixFormsGenerator", together with the previous explained function, is one of the most important functions as it generates the rounds of each game. It uses a random variable which determines the size of the generated squares inside the led matrix and tests if the player played well or bad, or if the player survived enough for a reward or lost all his hearts and thus the game. It also calculates the score and how fast the rounds change.

  <br>

#### 4. General update and display functins
  
```C++
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
```
  
```C++
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
```
These two functions are common throughout the entire code as they update the arrow in the menus and its position or display the current menu the user is browsing.
  
<br>
  
#### 5. readAndSetHighscoresVariablesFromEeprom()
  
```C++
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
```
                                                                            
"readAndSetHighscoresVariablesFromEeprom()" is a function that reads names and scores and other values from EEPROM and sets them into variables used in displaying the Highscores, Brightnesses, Contrast and during name changing.
  </details>
  
  
<br>
<br>

<p float="left">
  <img src="https://user-images.githubusercontent.com/41235115/147303639-bcac9ce0-3a92-45a9-bc71-c8dad39cb656.jpeg" width="300" />
  <img src="https://user-images.githubusercontent.com/41235115/147303658-8b5fab15-e795-464f-a24f-783c3864cdeb.jpeg" width="300" /> 
  <img src="https://user-images.githubusercontent.com/41235115/147303676-b729e914-4bbc-48e8-a1bd-960da8510944.jpeg" width="300" />
</p>
