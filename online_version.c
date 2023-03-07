#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

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
#define NUM_TASKS 10

LiquidCrystal_I2C lcd1(32, 16, 2);
LiquidCrystal_I2C lcd2(33, 16, 2);

int startScreen();
void generateGame(int difficulty);
void generateSingleTask(int position, int difficulty);
void playSingleTask(int mathProblemIndex);
void buildTask(int partOneInt, int partTwoInt, char operant, int i);
int handleDigitButtonPress(int digit, char* solutionPlayer, int index, LiquidCrystal_I2C& lcd2);
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
  const int difficulty = startScreen();
  generateGame(difficulty);

  // while playing
  const unsigned long timerStart = millis();
  for (int i = 0; i < NUM_TASKS; i++) {
    playSingleTask(i);
  }
  const unsigned long timerEnd = millis();
  const unsigned int timeInMs = timerEnd - timerStart;

  //after playing
  if (endScreen(timeInMs/1000)) {
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
  lcd1.print(F("KOPFRECHENSPIEL!"));
  lcd1.setCursor(0,1);
  lcd1.print(F("----------------"));
  lcd2.setCursor(0,0);
  lcd2.print(F("Press any button"));
  lcd2.setCursor(0,1);
  lcd2.print(F("to continue!"));

  // Wait for any IR button press to continue
  while(!IrReceiver.decode());
  IrReceiver.resume();

  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print(F("Select difficulty"));
  lcd1.setCursor(0,1);
  lcd1.print(F("and press [PLAY]"));
  lcd2.setCursor(0,0);
  lcd2.print(F("1(Easy) - 3(Hard):"));
  lcd2.setCursor(0,1);

  // Loop until valid difficulty is selected
  while (true) {
    if (IrReceiver.decode()) {
      byte command = IrReceiver.decodedIRData.command;
      switch (command) {
        case IR_BUTTON_1: 
          difficulty = 1;
          lcd2.clear();
          lcd2.setCursor(0,0);
          lcd2.print(F("diff: 1  [PLAY]?"));
          break;
        case IR_BUTTON_2:
          difficulty = 2;
          lcd2.clear();
          lcd2.setCursor(0,0);
          lcd2.print(F("diff: 2  [PLAY]?"));
          break;
        case IR_BUTTON_3: 
          difficulty = 3;
          lcd2.clear();
          lcd2.setCursor(0,0);
          lcd2.print(F("diff: 3  [PLAY]?"));
          break;
        case IR_BUTTON_PLAYPAUSE:
          if (difficulty != 4) {
            return difficulty;
          }
          lcd2.clear();
          lcd2.setCursor(0,0);
          lcd2.print(F("Select difficulty"));
          lcd2.setCursor(0,1);
          lcd2.print(F("and press [PLAY]"));
          break;
        default:
          break;
      }
      IrReceiver.resume();
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
    
    	buildTask(partOneInt, partTwoInt, '+', i);
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
    
    	buildTask(partOneInt, partTwoInt, '-', i);	
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
    
    	buildTask(partOneInt, partTwoInt, '*', i);
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
    
    	buildTask(partOneInt, partTwoInt, '/', i);
        break;
    default:
    	break;
  }
}

void buildTask(int partOneInt, int partTwoInt, char operant, int i)
{
  	short int resultInt = 0;
  	char partOneString[8] = {};
  	char partTwoString[8] = {};
  	char resultString[8] = {};
  	char task[displayLength] = {};
  
  	switch(operant) {
      case '+':
  	  	resultInt = partOneInt + partTwoInt;
  	  	break;
  	  case '-':
  	  	resultInt = partOneInt - partTwoInt;
  	  	break;
  	  case '*':
  	  	resultInt = partOneInt * partTwoInt;
  	  	break;
  	  case '/':
  	  	resultInt = partOneInt / partTwoInt;
  	  	break;
    }
  
  	snprintf(resultString, sizeof(resultString), "%d", resultInt);
    strcpy(arrSolution[i], resultString);
    
   	snprintf(partOneString, sizeof(partOneString), "%d", partOneInt);
    snprintf(partTwoString, sizeof(partTwoString), "%d", partTwoInt);
    
    snprintf(task, sizeof(task), "%s %c %s", partOneString, operant, partTwoString);
        
   	strcpy(arrTask[i], task);
}

// lets player play single math Problem from Array at index
void playSingleTask(int mathProblemIndex)
{
  char solutionPlayer[displayLength] = {};
  uint8_t empty = 1;
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print(F("confirm?[PLAY]"));
  lcd1.setCursor(0,1);
  lcd1.print(arrTask[mathProblemIndex]);
  int index = 0;
  
  while(true) {  
  
  while (!IrReceiver.decode()) {
    // wait for IR signal
  }
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
          index = handleDigitButtonPress(0, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_1: 
          index = handleDigitButtonPress(1, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_2:
          index = handleDigitButtonPress(2, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_3: 
          index = handleDigitButtonPress(3, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_4: 
          index = handleDigitButtonPress(4, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_5: 
          index = handleDigitButtonPress(5, solutionPlayer, index, lcd2);
          break;   
        case IR_BUTTON_6: 
          index = handleDigitButtonPress(6, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_7: 
          index = handleDigitButtonPress(7, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_8: 
          index = handleDigitButtonPress(8, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_9: 
          index = handleDigitButtonPress(9, solutionPlayer, index, lcd2);
          break;
        case IR_BUTTON_REPT:
    	  solutionPlayer[displayLength] = {};
          index = 0;
          empty = 1;
          lcd2.clear();
          break;
        case IR_BUTTON_PLAYPAUSE:  
          return;
        default:
          break;
  }
      lcd2.setCursor(0,1);
      lcd2.print(solutionPlayer);
 }
}

// Handle Input
int handleDigitButtonPress(int digit, char* solutionPlayer, int index, LiquidCrystal_I2C& lcd2) {
  if (index >= 16) {
    return index;
  }
  char digitChar = '0' + digit;
  strcat(solutionPlayer, &digitChar);
  lcd2.setCursor(15 - index, 1);
  lcd2.print(solutionPlayer[index]);
  index++;
  return index;
}

// Generates the endscreen with results
int endScreen(int time)
{
  lcd1.clear();
  lcd2.clear();
  lcd1.setCursor(0,0);
  lcd1.print(F("Time(s):"));
  lcd1.setCursor(9,0);
  char timeS[8] = {};
  sprintf(timeS, "%d", time);
  lcd1.print(timeS);
  lcd1.setCursor(0,1);
  lcd1.print(F("Continue?[ANY]"));
  // check and show results
  lcd2.setCursor(0,0);
  lcd2.print(F("Res: C=(0)/f=(X)"));
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
  lcd1.print(F("KOPFRECHENSPIEL!"));
  lcd1.setCursor(0,1);
  lcd1.print(F("----------------"));
  lcd2.setCursor(0,0);
  lcd2.print(F("Play Again?"));
  lcd2.setCursor(0,1);
  lcd2.print(F("Y[PLAY]/N[POWER]"));
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
