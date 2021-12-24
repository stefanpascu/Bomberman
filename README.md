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

This function represents an intro, displays the name of the game, the name of the creator and reads variables from EEPROM to set the Scoreboard, contrast, LCD brightness and Matrix brightness. It is used only once, after the game is restarted.

<br>

#### 2. duringTheMenuNavigation()
  


This function is one of the most important as it takes care of the entire menu. With the help from other smaller functions it is able to generate and display the user's navigation throughout the multitude of main and secondary branches. It mainly uses a "Switch-Case" for each menu state. For example, "resetHighscoresMenuStateValue" is a constant with the value of 12. If the variable menuState has its value it will only call the functions related to it. For most of the states, the content inside each case is similar to one another, but for setupGameOnStartStateValue and gameEndDisplayMenuStateValue, with the values of 7 and 10, it is relatively more complicated as they make the connection between the browseable LCD menu and the game itself which is played on the 8x8 led matrix. If menuState is equal to setupGameOnStartStateValue, the variables used during playtime will be initialised and the LCD will now display information related to the current game. If menuState  is equal to gameEndDisplayMenuStateValue, a final display with information will inform the player about his/her performance during the last playthrough and after a button press the player will return to the main menu.
  
<br>

  
#### 3. levelMatrixFormsGenerator()

These two functions are common throughout the entire code as they update the arrow in the menus and its position or display the current menu the user is browsing.
  
<br>
  
#### 5. readAndSetHighscoresVariablesFromEeprom()
  

                                                                            
"readAndSetHighscoresVariablesFromEeprom()" is a function that reads names and scores and other values from EEPROM and sets them into variables used in displaying the Highscores, Brightnesses, Contrast and during name changing.
  </details>
  
  
<br>
<br>

<p float="left">
  <img src="https://user-images.githubusercontent.com/41235115/147303639-bcac9ce0-3a92-45a9-bc71-c8dad39cb656.jpeg" width="300" />
  <img src="https://user-images.githubusercontent.com/41235115/147303658-8b5fab15-e795-464f-a24f-783c3864cdeb.jpeg" width="300" /> 
  <img src="https://user-images.githubusercontent.com/41235115/147303676-b729e914-4bbc-48e8-a1bd-960da8510944.jpeg" width="300" />
</p>
