#include <Wire.h>  // Wire Bibliothek hochladen
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek hochladen
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

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

LiquidCrystal_I2C lcd1(0x27, 16, 2);  //Hier wird das erste Display benannt (Adresse/Zeichen pro Zeile/Anzahl Zeilen). In unserem Fall „lcd1“. Die Adresse des I²C Displays kann je nach Modul variieren.
LiquidCrystal_I2C lcd2(0x26, 16, 2);  //Hier wird das zweite LCD benannt, hier "lcd2".
LiquidCrystal_I2C lcd3(0x25, 16, 2);

void generateArrTask(int difficulty);
void generateSingleTask(int position, int difficulty);

const int numberTasks = 10;
const int displayLength = 16;

char arrTask[numberTasks][displayLength] = {};
char arrSolution[numberTasks][displayLength] = {};

void setup()
{
  Serial.begin(9600);
  // Serial.println(xxx[1]);
  generateArrTask(3);
  delay(1000);
  
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
  // randomNumber=random(100,500);
  // Serial.println(randomNumber);
  // delay(1000);
}

void generateArrTask(int difficulty)
{	
  switch(difficulty) {
  	case 1:
    	for(int i = 0; i < 10; i++) {
    		int operant = 0;
    		generateSingleTask(i, difficulty);
        }
    	break;
    case 2:
        for(int i = 0; i < 10; i++) {
           	int operant = 0;
           	generateSingleTask(i, difficulty);
        }
    	break;
    case 3:
    	for(int i = 0; i < 10; i++) {
           	int operant = 0;
           	generateSingleTask(i, difficulty);
        }
    	break;
    default:
    	Serial.println("Das geht nicht!");
    	break;
  }
  for(int i = 0; i < 10; i++) {
    Serial.println(arrTask[i]);
  }
  
  for(int i = 0; i < 10; i++) {
    Serial.println(arrSolution[i]);
  }
}

void generateSingleTask(int i, int difficulty) 
{
  int operant = 0;
  int partOneInt = 0;
  int partTwoInt = 0;
  int resultInt = 0;
  char partOneString[8] = {};
  char partTwoString[8] = {};
  char resultString[8] = {};
  char task[displayLength] = {};
  
  if(difficulty == 1) {
  	operant = random(1,3);
  } else {
  	operant = random(1,5);
  }
  
  switch(operant) {
    // Plus
    case 1:
    	if(difficulty == 1) {
        	partOneInt = random(1,99);
    		partTwoInt = random(1,99);
        } else if(difficulty == 2) {
        	partOneInt = random(99,999);
    		partTwoInt = random(99,999);
        } else if(difficulty == 3) {
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
    	if(difficulty == 1) {
        	partOneInt = random(1,99);
            do{
              partTwoInt = random(1,99);
            } while(partOneInt < partTwoInt);
        } else if(difficulty == 2) {
        	partOneInt = random(99,999);
            do{
              partTwoInt = random(99,999);
            } while(partOneInt < partTwoInt);
        } else if(difficulty == 3) {
        	partOneInt = random(999,9999);
            do{
              partTwoInt = random(999,9999);
            } while(partOneInt < partTwoInt);
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
    	if(difficulty == 2) {
        	partOneInt = random(2,11);
            partTwoInt = random(10,51);
        } else if(difficulty == 3) {
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
    	if(difficulty == 2) {
          	partOneInt = random(99,300);
          	partTwoInt = random(3,10);
          	do{
              partOneInt--;
            } while(
              partOneInt < partTwoInt || 
              partOneInt % partTwoInt != 0
            );
        } else if(difficulty == 3) {
          	partOneInt = random(999,3000);
          	partTwoInt = random(11,30);
          	do{
              partOneInt--;
            } while(
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
//eingabe verarbeitung und ueberpruefung
//schaltplan ergaenzen
//praesi
//zeit
//richtig falsch anzeige
//(highscore)
//(eingabe name)
