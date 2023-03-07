#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

/* PHYSISCHEM ARDUINO
#define IR_BUTTON_0 22
#define IR_BUTTON_1 12
#define IR_BUTTON_2 24
#define IR_BUTTON_3 94
#define IR_BUTTON_4 8
#define IR_BUTTON_5 28
#define IR_BUTTON_6 90
#define IR_BUTTON_7 66
#define IR_BUTTON_8 82
#define IR_BUTTON_9 74
#define IR_BUTTON_PLAYPAUSE // ZAHL AN PHYSISCHEM ARDUINO TESTEN
#define IR_BUTTON_REPT // ZAHL AN PHYSISCHEM ARDUINO TESTEN
#define IR_BUTTON_POWER // ZAHL AN PHYSISCHEM ARDUINO TESTEN
*/
#define IR_BUTTON_0 12
#define IR_BUTTON_1 16
#define IR_BUTTON_2 17
#define IR_BUTTON_3 18
#define IR_BUTTON_4 20
#define IR_BUTTON_5 21
#define IR_BUTTON_6 22
#define IR_BUTTON_7 24
#define IR_BUTTON_8 25
#define IR_BUTTON_9 26
#define IR_BUTTON_PLAYPAUSE 5
#define IR_BUTTON_REPT 14
#define IR_BUTTON_POWER 0

// PHYSISCHEM ARDUINO
//LiquidCrystal_I2C lcd1(0x27, 16, 2);  //Hier wird das erste Display benannt (Adresse/Zeichen pro Zeile/Anzahl Zeilen). In unserem Fall „lcd1“. Die Adresse des I²C Displays kann je nach Modul variieren.
//LiquidCrystal_I2C lcd2(0x26, 16, 2);  //Hier wird das zweite LCD benannt, hier "lcd2".
LiquidCrystal_I2C lcd1(32, 16, 2);
LiquidCrystal_I2C lcd2(33, 16, 2);

int startScreen();
void generateGame(int difficulty);
void generateSingleTask(int position, int difficulty);
void playSingleTask(int mathProblemIndex);
int endScreen(int time);

const short int numberTasks = 10;
const short int displayLength = 16;

char arrTask[numberTasks][displayLength] = {};
char arrSolution[numberTasks][displayLength] = {};
char playerSolution[numberTasks][displayLength] = {};

void setup()
{
  lcd1.init(); //Im Setup wird das LCD1 gestartet
  lcd2.init(); //Im Setup wird das LCD2 gestartet
  lcd1.backlight(); //Hintergrundbeleuchtung von LCD1 einschalten (0 schaltet die Beleuchtung aus).
  lcd2.backlight(); //Hintergrundbeleuchtung von LCD2 einschalten (0 schaltet die Beleuchtung aus).
  Serial.begin(9600);
  pinMode (13, OUTPUT);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
  //before playing
  int difficulty = startScreen();
  generateGame(difficulty);
  // while playing
  unsigned int timerStart = millis();
  for (int i = 0; i < 10; i++) {
    playSingleTask(i);
  }
  unsigned int timerEnd = millis();
  unsigned int time = timerEnd - timerStart;
  int timeInS = (int)time / 1000;
  //after playing
  if (endScreen(timeInS)) {
    // ARDUINO BEENDEN
  }
}

// fires before the game beginns and shows the startscreen and difficulty selection
int startScreen()
{
  byte difficulty = 4;
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print("KOPFRECHENSPIEL!");
  lcd1.setCursor(0,1);
  lcd1.print("----------------");
  lcd2.setCursor(0,0);
  lcd2.print("Press any button");
  lcd2.setCursor(0,1);
  lcd2.print("to continue!");
  while(1) {
    if (IrReceiver.decode()) {
      IrReceiver.resume();
      lcd1.clear();
  	  lcd2.clear();
      break;
    }    
  }
  lcd1.setCursor(0,0);
  lcd1.print("Select one and");
  lcd1.setCursor(0,1);
  lcd1.print("press [PLAY]");
  lcd2.setCursor(0,0);
  lcd2.print("1(Easy)- 3(Hard):");
  lcd2.setCursor(0,1);
  while (1) {   
    if (IrReceiver.decode()) {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print("1(Easy)- 3(Hard):");
      lcd2.setCursor(0,1);
      IrReceiver.resume();
      byte command = IrReceiver.decodedIRData.command;
      switch (command) {
        case IR_BUTTON_1: 
          difficulty = 1;
          lcd2.print("diff: 1  [PLAY]?");
          break;
        case IR_BUTTON_2:
          difficulty = 2;
          lcd2.print("diff: 2  [PLAY]?");
          break;
        case IR_BUTTON_3: 
          difficulty = 3;
          lcd2.print("diff: 3  [PLAY]?");   
          break;               
        case IR_BUTTON_PLAYPAUSE:
          if (difficulty != 0) {
            return difficulty;
          }
          lcd2.print("Wrong input!");
          break;
        default:
          lcd2.print("Wrong input!");
          break;
      }
    }
  }
}

// Generates the 10 Math Problems
void generateGame(int difficulty)
{	
  for (int i = 0; i < 10; i++) {
      generateSingleTask(i, difficulty);
  }
}

// Generates a single MathProblem
void generateSingleTask(int i, int difficulty) 
{
  byte operant = 0;
  short int partOneInt = 0;
  short int partTwoInt = 0;
  short int resultInt = 0;
  char partOneString[8] = {};
  char partTwoString[8] = {};
  char resultString[8] = {};
  char task[displayLength] = {};
  
  randomSeed(analogRead(A0));
  
  if(difficulty == 1) {
  	operant = random(1,3);
  } else {
  	operant = random(1,5);
  }
  
  switch (operant) {
    // Plus
    case 1:
    	if (difficulty == 1) {
        	partOneInt = random(1,99);
    		partTwoInt = random(1,99);
        } else if (difficulty == 2) {
        	partOneInt = random(99,999);
    		partTwoInt = random(99,999);
        } 
        else if (difficulty == 3) {
        	partOneInt = random(999,9999);
    		partTwoInt = random(999,9999);
        }
    
    	resultInt = partOneInt + partTwoInt;
    	sprintf(resultString, "%d", resultInt);
        strcpy(arrSolution[i], resultString);
    
    	sprintf(partOneString, "%d", partOneInt);
        sprintf(partTwoString, "%d", partTwoInt);
    
    	strcat(task, partOneString);
        strcat(task, " + ");
    	strcat(task, partTwoString);
        
   		strcpy(arrTask[i], task);
    	break;
    // Minus
    case 2:
    	if (difficulty == 1) {
        	partOneInt = random(1,99);
            do
            {
              partTwoInt = random(1,99);
            } while (partOneInt < partTwoInt);
        } else if (difficulty == 2) {
        	partOneInt = random(99,999);
            do
            {
              partTwoInt = random(99,999);
            } while (partOneInt < partTwoInt);
        } else if (difficulty == 3) {
        	partOneInt = random(999,9999);
            do
            {
              partTwoInt = random(999,9999);
            } while (partOneInt < partTwoInt);
        }
    
    	resultInt = partOneInt - partTwoInt;
    	sprintf(resultString, "%d", resultInt);
        strcpy(arrSolution[i], resultString);
    
    	sprintf(partOneString, "%d", partOneInt);
        sprintf(partTwoString, "%d", partTwoInt);
    
    	strcat(task, partOneString);
        strcat(task, " - ");
    	strcat(task, partTwoString);
        
   		strcpy(arrTask[i], task);
    	break;
    // Mal
    case 3:
    	if (difficulty == 2) {
        	partOneInt = random(2,11);
            partTwoInt = random(10,51);
        } else if (difficulty == 3) {
        	partOneInt = random(10,99);
          	partTwoInt = random(10,99);
        }
    	
    	resultInt = partOneInt * partTwoInt;
    	sprintf(resultString, "%d", resultInt);
        strcpy(arrSolution[i], resultString);
    
    	sprintf(partOneString, "%d", partOneInt);
        sprintf(partTwoString, "%d", partTwoInt);
    
    	strcat(task, partOneString);
        strcat(task, " * ");
    	strcat(task, partTwoString);
        
   		strcpy(arrTask[i], task);
    break;
    // Geteilt
    case 4:
    	if (difficulty == 2) {
          	partOneInt = random(99,300);
          	partTwoInt = random(3,10);
          	do {
              partOneInt--;
            } while (
              partOneInt < partTwoInt || 
              partOneInt % partTwoInt != 0
            );
        } 
        else if (difficulty == 3) {
          	partOneInt = random(999,3000);
          	partTwoInt = random(11,30);
          	do {
              partOneInt--;
            } while (
              partOneInt < partTwoInt || 
              partOneInt % partTwoInt != 0
            );
        }
    	
    	resultInt = partOneInt / partTwoInt;
    	sprintf(resultString, "%d", resultInt);
        strcpy(arrSolution[i], resultString);
    
    	sprintf(partOneString, "%d", partOneInt);
        sprintf(partTwoString, "%d", partTwoInt);
    
    	strcat(task, partOneString);
        strcat(task, " / ");
    	strcat(task, partTwoString);
        
   		strcpy(arrTask[i], task);
        break;
    default:
    	break;
  }
}

// lets player play single math Problem from Array at index
void playSingleTask(int mathProblemIndex)
{
  char solutionPlayer[displayLength] = {};
  short int empty = 1;
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print("confirm?[PLAY]");
  lcd1.setCursor(0,1);
  lcd1.print(arrTask[mathProblemIndex]);
  byte index = 0;
  while (1) {
    if (IrReceiver.decode()) {      
      IrReceiver.resume();
      byte command = IrReceiver.decodedIRData.command;
      // check if array empty
      if (command == IR_BUTTON_PLAYPAUSE && empty == 1) {
        command = -1;
      } else {
        empty = 0;
      }      
      switch (command) {
        case IR_BUTTON_0:
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "0");
          lcd2.setCursor(15 - index,1);
          lcd2.print("0");
          index++;
          break;
        case IR_BUTTON_1: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "1");
          lcd2.setCursor(15 - index,1);
          lcd2.print("1");
          index++;
          break;
        case IR_BUTTON_2:
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "2");
          lcd2.setCursor(15 - index,1);
          lcd2.print("2");
          index++;
          break;
        case IR_BUTTON_3: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "3");
          lcd2.setCursor(15 - index,1);
          lcd2.print("3");
          index++;      
          break;
        case IR_BUTTON_4: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "4");
          lcd2.setCursor(15 - index,1);
          lcd2.print("4");
          index++;
          break;
        case IR_BUTTON_5: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "5");
          lcd2.setCursor(15 - index,1);
          lcd2.print("5");
          index++;
          break;   
        case IR_BUTTON_6: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "6");
          lcd2.setCursor(15 - index,1);
          lcd2.print("6");
          index++;
          break;
        case IR_BUTTON_7: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "7");
          lcd2.setCursor(15 - index,1);
          lcd2.print("7");
          index++;
          break;
        case IR_BUTTON_8: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "8");
          lcd2.setCursor(15 - index,1);
          lcd2.print("8");
          index++;
          break;
        case IR_BUTTON_9: 
          if (index >= 16) {
            break;
          }
          strcat(solutionPlayer, "9");
          lcd2.setCursor(15 - index,1);
          lcd2.print("9");
          index++;
          break;
        case IR_BUTTON_REPT:
          index = 0;
          empty = 1;
          lcd2.clear();
          break;
        case IR_BUTTON_PLAYPAUSE:           
          return;
        default:
          break;
      }
    }
  }
  
}

// Generates the endscreen with results
int endScreen(int time)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print("Time(s):");
  lcd1.setCursor(9,0);
  char timeS[8] = {};
  sprintf(timeS, "%d", time);
  lcd1.print(timeS);
  lcd1.setCursor(0,1);
  lcd1.print("Continue?[ANY]");
  // check and show results
  lcd2.setCursor(0,0);
  lcd2.print("Res: C=(0)/f=(X)");
  for (int i = 0; i < 10; i++) {
    lcd2.setCursor(i,1);
    if (playerSolution[i] == arrSolution[i]) {
      lcd2.print("0");
    } else {
      lcd2.print("X");
    }
  }
  while (1) {
    if (IrReceiver.decode()) {
      IrReceiver.resume();
      break;
    }
  }
  
  // Endcard
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print("KOPFRECHENSPIEL!");
  lcd1.setCursor(0,1);
  lcd1.print("----------------");
  lcd2.setCursor(0,0);
  lcd2.print("Play Again?");
  lcd2.setCursor(0,1);
  lcd2.print("Y[PLAY]/N[POWER]");
  while (1) {
    if (IrReceiver.decode()) {
      IrReceiver.resume();
  	  byte command = IrReceiver.decodedIRData.command;
      if (command == IR_BUTTON_PLAYPAUSE) {
        return 1;
      } else if (command == IR_BUTTON_POWER) {
        return 0;
      }
    }
  }
}

//schaltplan ergaenzen
//praesi
//(highscore)
//(eingabe name)
