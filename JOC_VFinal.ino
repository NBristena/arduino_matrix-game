#include <LiquidCrystal.h>
#include "LedControl.h"
LedControl lc=LedControl(12,11,10,1); 
LiquidCrystal lcd(8,9,5,4,3,2);

int upDown; //citeste valoarea analog de pe joystick pentru axa oY
int leftRight; //citeste valoarea analog de pe joystick pentru axa oX
int oY; // = randul pe care se afla jucatorul
int oX; // = coloana pe care se afla jucatorul
int button = 7; //pin-ul pentru buton
int i = 0; //variabila folosita pentru for-uri si verificari

//  Vectorii myCol si myRow sunt folositi pentru apelarea led-urilor de pe
//o anumita coloana si un anumit rand (ex: myCol[0] este coloana 0)
int myCol[] = {3, 5, 6, 0, 2, 7, 1, 4};
int myRow[] = {2, 5, 6, 0, 7, 3, 1, 4};

//  Vectorii colOn si colOff sunt folositi pentru aprinderea si stingerea
//pe matrice a ledurilor care reprezinta masinile
int colOn[] = {3, 3, 5, 5, 6, 6, 0, 0, 2, 2, 7, 7, 1, 1, 4, 4};
int colOff[] ={1, 1, 4, 4, 3, 3, 5, 5, 6, 6, 0, 0, 2, 2, 7, 7};

//Vectorii carsRow retin pozitiile in care sunt masini pe fiecare rand
int carsRow2[] = {0, 0, 0, 0, 0, 0, 0, 0};
int carsRow3[] = {0, 0, 0, 0, 0, 0, 0, 0};
int carsRow4[] = {0, 0, 0, 0, 0, 0, 0, 0};
int carsRow5[] = {0, 0, 0, 0, 0, 0, 0, 0};
int carsRow6[] = {0, 0, 0, 0, 0, 0, 0, 0};

char startMessage[] = "Press button to start the game!";
char gameName[] = "Game: Cross the road";
char winMessage[] = "Congrats, you   beat the game!";
String menuSelect[] = {"Menu", "Pick a level", "Highscores", "Exit"};
String levelSelect[] = {"Pick a level", "Level 1", "Level 2", "Level 3", "Level 4", "Back to menu"};
String highscoreShow[] = {"Highscores", "Lvl 1: ", "Lvl 2: ", "Lvl 3: ", "Lvl 4: ", "Reset h-scores", "Back to menu  "};
String optionExit[] = {"Are you sure?", "Yes, exit", "No, go back"};
String optionLoser[] = {"You lost!", "Retry", "Back to menu"};
String optionWinner[] = {"Your score: ", "Next level", "Back to menu"};

int positionInMenu = 1; //positia unei optiuni intr-un meniu
int joystickDirection = 0; //memoreaza miscarea joystick-ului 
                           //1 = sus, -1 = jos
                           //2 = dreapta, -2 = stanga
int lastJoystickDirection = -1; //memoreaza ultima miscare a joystick-ului
int buttonState = 1;
int lastButtonState = 1;

int gameOn = 0; //retine daca jocul este pornit sau oprit
int menu = -1; //retine meniul in care esti (ex: principal, alege nivel, iesi din joc)
int lvl = 0, life = 3;
int score[] = {0, 0, 0, 0, 0}, highscore[] = {0, 0, 0, 0, 0};

unsigned long previousTime = 0, currentTime;
unsigned long startTime; //retine milisecunda la care incepe un nivel
unsigned long endTime; //retine milisecunda la care se termina un nivel

/************************************************************
 * Functiile display afiseaza anumite mesaje pe ecranul LCD *
 ************************************************************/
void displayStartMessage(){
   lcd.setCursor(1, 0);
   for (int i = 0; i < 16; i++){
      lcd.print(startMessage[i]);
      delay(10);
   }
   lcd.setCursor(1, 1);
   for (int i = 16; i < 32; i++){
      lcd.print(startMessage[i]);
      delay(10);
   }
}
void displayGameName(){
   lcd.setCursor(5, 0);
   for (int i = 0; i < 6; i++){
      lcd.print(gameName[i]);
      delay(100);
   }
   lcd.setCursor(1, 1);
   for (int i = 6; i < 20; i++){
      lcd.print(gameName[i]);
      delay(100);
   }
   delay(500); lcd.clear();
}
void displayWinMessage(){
   lcd.setCursor(1, 0);
   for (int i = 0; i < 15; i++){
      lcd.print(winMessage[i]);
      delay(100);
   }
   lcd.setCursor(1, 1);
   for (int i = 16; i < 30; i++){
      lcd.print(winMessage[i]);
      delay(100);
   }
   delay(1000); lcd.clear();
}

/****************************************************************************
 * Functiile show arata anumite mesaje,litere sau semne pe matrice          *
 * ex: M - menu, L - pick level, X - nu mai ai vieti, ON - s-a aprins jocul *
 ****************************************************************************/
void showOff(){
   i = 0;
   while(i < 8){
      if(i != 4 && i != 5 && i != 7)           lc.setLed(0,myCol[i],myRow[2],true);
      if(i != 1 && i != 5)                     lc.setLed(0,myCol[i],myRow[3],true);
      if(i != 1 && i != 4 && i != 5 && i != 7) lc.setLed(0,myCol[i],myRow[4],true);
      if(i != 5)                               lc.setLed(0,myCol[i],myRow[5],true);
      i++;
   }
}  
void showOn(){
   i = 0;
   while(i < 8){
      if(i != 3 && i != 5 && i != 6) lc.setLed(0,myCol[i],myRow[2],true);
      if(i != 1 && i != 3 && i != 5) lc.setLed(0,myCol[i],myRow[3],true);
      if(i != 1 && i != 3 && i != 6) lc.setLed(0,myCol[i],myRow[4],true);
      if(i != 3 && i != 5 && i != 6) lc.setLed(0,myCol[i],myRow[5],true);
      i++;
   }
}
void showM(){
   i = 0;
   while(i < 8){
      if(i > 0 && i < 7) lc.setLed(0,myCol[2],myRow[i],true);
      if(i == 5)         lc.setLed(0,myCol[3],myRow[i],true);
      if(i == 4)         lc.setLed(0,myCol[4],myRow[i],true);
      if(i == 5)         lc.setLed(0,myCol[5],myRow[i],true);
      if(i > 0 && i < 7) lc.setLed(0,myCol[6],myRow[i],true);
      i++;
   }
}    
void showL(){
   i = 0;
   while(i < 8){
      if(i > 0 && i < 7) lc.setLed(0,myCol[2],myRow[i],true);
      if(i == 1){        lc.setLed(0,myCol[3],myRow[i],true);
                         lc.setLed(0,myCol[4],myRow[i],true);
                         lc.setLed(0,myCol[5],myRow[i],true);
      }
      i++;
   }
}   
void showHS(){
   i = 0;
   while(i < 8){
      if(i != 0){ lc.setLed(0,myCol[0],myRow[i],true);
                  lc.setLed(0,myCol[3],myRow[i],true);
      }
      if(i == 4){ lc.setLed(0,myCol[1],myRow[i],true);
                  lc.setLed(0,myCol[2],myRow[i],true);
      }
      if(i == 2 || i == 5 || i == 6)  lc.setLed(0,myCol[4],myRow[i],true);
      if(i == 1 || i == 4 || i == 7){ lc.setLed(0,myCol[5],myRow[i],true);
                                      lc.setLed(0,myCol[6],myRow[i],true);
      }
      if(i == 2 || i == 3 || i == 6)  lc.setLed(0,myCol[7],myRow[i],true);
      i++;
   }
} 
void showX(){
   i = 0;
   while(i < 8){
      if(i == 1 || i == 6){ lc.setLed(0,myCol[i],myRow[1],true);
                            lc.setLed(0,myCol[i],myRow[6],true);
      }
      if(i == 2 || i == 5){ lc.setLed(0,myCol[i],myRow[2],true);
                            lc.setLed(0,myCol[i],myRow[5],true);
      }
      if(i == 3 || i == 4){ lc.setLed(0,myCol[i],myRow[3],true);
                            lc.setLed(0,myCol[i],myRow[4],true);
      }                                                                                                              
      i++;
   }
} 
void showV(){
   i = 0;
   while(i < 8){
      if(i == 2)           lc.setLed(0,myCol[i],myRow[1],true);
      if(i == 1 || i == 3) lc.setLed(0,myCol[i],myRow[2],true);
      if(i == 0 || i == 4) lc.setLed(0,myCol[i],myRow[3],true);
      if(i == 5)           lc.setLed(0,myCol[i],myRow[4],true);
      if(i == 6)           lc.setLed(0,myCol[i],myRow[5],true);
      if(i == 7)           lc.setLed(0,myCol[i],myRow[6],true);
      i++;
   }
} 

/***************************************************************
 * Functia selectMenu asigura afisarea meniului pe ecranul LCD *
 * si accesul jucatorului la fiecare optiune din meniu         *
 ***************************************************************/
void selectMenu(){
   lcd.clear(); 
   lc.clearDisplay(0);
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(menu == 0){
      delay(200); showM();
      upDown = analogRead(A1);
      buttonState = digitalRead(button);
      joystickDirection = 0;
      if(upDown > 750) {joystickDirection = 1;}
      if(upDown < 250) {joystickDirection = -1;}
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 3) positionInMenu = 3;
          
         lcd.clear();
         lcd.setCursor(2, 0);
         lcd.print(menuSelect[0]);
         lcd.setCursor(1, 1);
         lcd.print("> ");
         lcd.print(menuSelect[positionInMenu]);
      }
      
      if (buttonState != lastButtonState){
         lc.clearDisplay(0);
         switch(positionInMenu){
            case 1: menu = 1; break;
            case 2: menu = 2; break;
            case 3: menu = 3; break;
         }
      }
   }
}

/*********************************************************************************
 * Functia pickLevel asigura afisarea listei cu nivelele pe ecranul LCD, accesul *
 * jucatorului la orice nivel sau optiunea de a se intoarce in meniul principal  *
 *********************************************************************************/
void pickLevel(){
   lcd.clear();
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(menu == 1 && lvl == 0){
      delay(200); showL();
      upDown = analogRead(A1);
      buttonState = digitalRead(button); 
      joystickDirection = 0;
      if(upDown > 750) joystickDirection = 1;
      if(upDown < 250) joystickDirection = -1;
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 5) positionInMenu = 5;
          
         lcd.clear();
         lcd.setCursor(1, 0);
         lcd.print(levelSelect[0]);
         lcd.setCursor(1, 1);
         lcd.print("> ");
         lcd.print(levelSelect[positionInMenu]);
      }
      if (buttonState != lastButtonState){
         menu = -1; lc.clearDisplay(0);
         oY = 0, oX = 4; previousTime = 0;
         switch(positionInMenu){
            case 1: lvl = 1; break;
            case 2: lvl = 2; break;
            case 3: lvl = 3; break;
            case 4: lvl = 4; break;
            case 5: menu = 0; lc.clearDisplay(0); break;
         }
      }
   }      
}

/******************************************************************************
 * Functia showHighscore asigura afisarea listei cu highscor-urile pe nivele, *
 * optiunea de a reseta highscor-urile si cea de a reveni in meniul principal *
 ******************************************************************************/
void showHighscore(){
   lcd.clear();
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(menu == 2){
      delay(200); showHS();
      upDown = analogRead(A1);
      buttonState = digitalRead(button); 
      joystickDirection = 0;
      if(upDown > 750) joystickDirection = 1;
      if(upDown < 250) joystickDirection = -1;
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 6) positionInMenu = 6;
          
         lcd.clear();
         lcd.setCursor(1, 0);
         lcd.print(highscoreShow[0]);
         lcd.setCursor(0, 1);
         lcd.print("> ");
         lcd.print(highscoreShow[positionInMenu]);
         if(positionInMenu != 5) lcd.print(highscore[positionInMenu]);
      }
      if (buttonState != lastButtonState){
         if(positionInMenu == 5){
            highscore[1] = 0;
            highscore[2] = 0;
            highscore[3] = 0;
            highscore[4] = 0;
            menu = 0;
         }
         if(positionInMenu == 6){
            menu = 0; 
            lc.clearDisplay(0);
         }
      }
   }      
}

/******************************************************************************
 * Functia exitOption ofera optiunea de a opri jocul si implicit de a reveni  *
 * la starea initiala in care se cere apasarea butonului pentru a porni jocul *
 ******************************************************************************/
void exitOption(){
   lcd.clear();
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(menu == 3){
      delay(200); showOn();
      upDown = analogRead(A1);
      buttonState = digitalRead(button); 
      joystickDirection = 0;
      if(upDown > 750) joystickDirection = 1;
      if(upDown < 250) joystickDirection = -1;
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 2) positionInMenu = 2;
          
         lcd.clear();
         lcd.setCursor(1, 0);
         lcd.print(optionExit[0]);
         lcd.setCursor(1, 1);
         lcd.print("> ");
         lcd.print(optionExit[positionInMenu]);
      }
      if (buttonState != lastButtonState){
         switch(positionInMenu){
            case 1: lc.clearDisplay(0); lcd.clear(); displayStartMessage();
                    menu = -1; gameOn = 0; break;
            case 2: menu = 0; break;
         }
      }
   }      
}

void startLevel(){
   switch(lvl){
      case 1: lcd.print(" Level 1"); lcd.setCursor(1,1); lcd.print("Vieti ramase: ");
              lcd.print(life); lvl1(); checkLvl1(); break;
      case 2: lcd.print(" Level 2"); lcd.setCursor(1,1); lcd.print("Vieti ramase: ");
              lcd.print(life); lvl2(); checkLvl2(); break;
      case 3: lcd.print(" Level 3"); lcd.setCursor(1,1); lcd.print("Vieti ramase: ");
              lcd.print(life); lvl3(); checkLvl3(); break;
      case 4: lcd.print(" Level 4"); lcd.setCursor(1,1); lcd.print("Vieti ramase: ");
              lcd.print(life); lvl4(); checkLvl4(); break;
   }
}

void playerMovement(){
   lc.setLed(0,myCol[oX],myRow[oY],true);
   
   upDown = analogRead(A1);
   //leftRight = analogRead(A0);
   joystickDirection = 0;
   if(upDown > 750) joystickDirection = -1;
   if(upDown < 268) joystickDirection = 1;
   //if(leftRight > 750) joystickDirection = -2;
   //if(leftRight < 268) joystickDirection = 2;
   
   if(joystickDirection != lastJoystickDirection){
      lastJoystickDirection = joystickDirection ;
      lc.setLed(0,myCol[oX],myRow[oY],false);
      if(joystickDirection == 1 || joystickDirection == -1)
         oY += joystickDirection; 
      //if(joystickDirection == 2 || joystickDirection == -2)
      //   oX += (joystickDirection / 2); 
      
      if(oY < 0) oY = 0;
      if(oX > 7) oX = 7; 
      if(oX < 0) oX = 0;
      if(oY == 7){
         delay(300); 
         life += 10;
         endTime = currentTime;
      } 
      lc.setLed(0,myCol[oX],myRow[oY],true);
   }
}

void loserOption(){
   lcd.clear();
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(life == 0){
      showX();
      upDown = analogRead(A1);
      buttonState = digitalRead(button); 
      joystickDirection = 0;
      if(upDown > 750) joystickDirection = 1;
      if(upDown < 250) joystickDirection = -1;
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 2) positionInMenu = 2;
          
         lcd.clear();
         lcd.setCursor(1, 0);
         lcd.print(optionLoser[0]);
         lcd.setCursor(1, 1);
         lcd.print("> ");
         lcd.print(optionLoser[positionInMenu]);
      }
      if (buttonState != lastButtonState){
         switch(positionInMenu){
            case 1: life = 3; lc.clearDisplay(0); previousTime = 0; break;
            case 2: life = 3; menu = 0; lvl = 0; break;
         }
      }
   }      
}

void winnerOption(){
   lcd.clear();
   positionInMenu = 1;
   lastJoystickDirection = -1;
   while(life > 10){
      showV();
      upDown = analogRead(A1);
      buttonState = digitalRead(button); 
      joystickDirection = 0;
      if(upDown > 750) joystickDirection = 1;
      if(upDown < 250) joystickDirection = -1;
  
      if (joystickDirection != lastJoystickDirection){
         lastJoystickDirection = joystickDirection ;
         positionInMenu += joystickDirection;
         if (positionInMenu < 1) positionInMenu = 1;
         if (positionInMenu > 2) positionInMenu = 2;
          
         lcd.clear();
         lcd.setCursor(1, 0);
         if(highscore[1] != 0 && highscore[2] != 0 && highscore[3] != 0 && highscore[4] != 0){
            lcd.print(optionWinner[0]);
            lcd.print(score[lvl]);
            delay(1000);
            displayWinMessage();
            delay(200);
            menu = 0; life = 3; lvl = 0;
         }
         else{
            if(lvl == 4){
               positionInMenu = 2;
               lcd.print(optionWinner[0]);
               lcd.print(score[lvl]);
               lcd.setCursor(1, 1);
               lcd.print("> ");
               lcd.print(optionWinner[positionInMenu]);
            }
            else{
               lcd.print(optionWinner[0]);
               lcd.print(score[lvl]);
               lcd.setCursor(1, 1);
               lcd.print("> ");
               lcd.print(optionWinner[positionInMenu]);
            }
         }
      }
      if (buttonState != lastButtonState){
         switch(positionInMenu){
            case 1: life = 3; lvl +=1; oY = 0, oX = 4; previousTime = 0;
                  lc.clearDisplay(0); break;
            case 2: life = 3; menu = 0; lvl = 0; break;
         }
      }
   }      
}

/********************************************************
 * Scorul este calculat in functie de numarul de vieti  *
 * ramase si timpul necesar pentru castigarea nivelului *
 ********************************************************/
void calculateScore(){
   score[lvl] = life - 10;
   if(endTime - startTime < 10000) score[lvl] *= 100;
   else if(endTime - startTime < 15000) score[lvl] *= 75;
   else if(endTime - startTime < 20000) score[lvl] *= 50;
   else score[lvl] *= 25;
   
   if(score[lvl] > highscore[lvl]) highscore[lvl] = score[lvl];
}

/********************************************************************
* Functiile carsLvl creeaza masinile pe matrice prin lc.setLed() si *
* memoreaza positia masinilor  pe rand in vectorii carsRow[]        *
*********************************************************************/
void carsLvl1(){
   if(i == 16) i = 0;
   
//ROW 2 ON
/*CAR 1*/
   lc.setLed(0, colOn[i], myRow[2], true); 
   carsRow2[i / 2] = 1;
/*CAR 2*/
   if(i < 8){
      lc.setLed(0, colOn[i + 8], myRow[2], true); 
      carsRow2[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[2], true); 
      carsRow2[(i - 8) / 2] = 1;
   }
   
//ROW 4 ON
/*CAR 1*/
   if(i < 14){
      lc.setLed(0, colOn[i + 2], myRow[4], true); 
      carsRow4[(i + 2) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 14], myRow[4], true); 
      carsRow4[(i - 14) / 2] = 1;
   }
/*CAR 2*/
   if(i < 8){
      lc.setLed(0, colOn[i + 8],myRow[4], true); 
      carsRow4[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[4], true); 
      carsRow4[(i - 8) / 2] = 1;
   }
/*CAR 3*/
   if(i < 4){
      lc.setLed(0, colOn[i + 12],myRow[4], true); 
      carsRow4[(i + 12) / 2] = 1;
   }
   else {
      lc.setLed(0, colOn[i - 4], myRow[4], true); 
      carsRow4[(i - 4) / 2] = 1;
   }
   
//ROW 6 ON
/*CAR 1*/
   lc.setLed(0, colOn[i], myRow[6], true); 
   carsRow6[i / 2] = 1;
/*CAR 2*/
   if(i < 6) {
      lc.setLed(0, colOn[i + 10],myRow[6], true); 
      carsRow6[(i + 10) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 6], myRow[6], true); 
      carsRow6[(i - 6) / 2] = 1;
   }
   
//ROW 2 OFF
/*CAR 1*/
   lc.setLed(0, colOff[i], myRow[2], false);
   if(i < 4) carsRow2[(i + 12) / 2] = 0;
   else  carsRow2[(i - 4) / 2] = 0;
/*CAR 2*/   
   if(i < 8) lc.setLed(0, colOff[i + 8], myRow[2], false); 
   else lc.setLed(0, colOff[i - 8], myRow[2], false);
   if(i < 12) carsRow2[(i + 4) / 2] = 0;
   else carsRow2[(i - 12) / 2] = 0;
   
//ROW 4 OFF
/*CAR 1*/
   if(i < 12) lc.setLed(0, colOff[i + 4], myRow[4], false);
   else lc.setLed(0, colOff[i - 12], myRow[4], false);
   if(i < 14) carsRow4[i / 2] = 0;
   else carsRow4[7] = 0;
/*CAR 2*/      
   if(i < 8) lc.setLed(0, colOff[i + 8], myRow[4], false);
   else lc.setLed(0, colOff[i - 8], myRow[4], false);
   if (i < 12) carsRow4[(i + 4) / 2] = 0;
   else carsRow4[(i - 12) / 2] = 0;
/*CAR 3*/      
   if(i > 1) lc.setLed(0, colOff[i - 2], myRow[4], false);
   else lc.setLed(0, colOff[i + 14], myRow[4], false);
   if(i < 6) carsRow4[(i + 10) / 2] = 0;
   else carsRow4[(i - 6) / 2] = 0;

//ROW 6 OFF
/*CAR 1*/
   if(i < 14) lc.setLed(0, colOff[i + 2], myRow[6], false);
   else lc.setLed(0, colOff[i - 14], myRow[6], false);
   if(i > 1) carsRow6[(i - 2) / 2] = 0;
   else carsRow6[7] = 0;
/*CAR 2*/
   if(i < 6) lc.setLed(0, colOff[i + 10], myRow[6], false);
   else lc.setLed(0, colOff[i - 6], myRow[6], false);
   if(i < 10) carsRow6[(i + 6) / 2] = 0;
   else carsRow6[(i - 10) / 2] = 0;
   
   i++;
}

void carsLvl2(){
  if(i == 16) i = 0;
  
//ROW 2 ON
/*CAR 1*/
   lc.setLed(0, colOn[i], myRow[2], true); 
   carsRow2[i / 2] = 1;
/*CAR 2*/   
   if(i < 8){
      lc.setLed(0, colOn[i + 8], myRow[2], true); 
      carsRow2[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[2], true); 
      carsRow2[(i - 8) / 2] = 1;
   }
   
//ROW 4 ON
/*CAR 1*/
   if(i < 10){
      lc.setLed(0, colOn[i + 6], myRow[4], true); 
      carsRow4[(i + 6) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 10], myRow[4], true); 
      carsRow4[(i - 10) / 2] = 1;
   }
/*CAR 2*/
   if(i < 4){
      lc.setLed(0, colOn[i + 12],myRow[4], true); 
      carsRow4[(i + 12) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 4], myRow[4], true); 
      carsRow4[(i - 4) / 2] = 1;
   }

//ROW 5 ON
/*CAR 1*/
   if(i < 12){
      lc.setLed(0, colOn[i + 4], myRow[5], true); 
      carsRow5[(i + 4) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 12], myRow[5], true); 
      carsRow5[(i - 12) / 2] = 1;
   }

//ROW 2 OFF
/*CAR 1*/
   lc.setLed(0, colOff[i], myRow[2], false);
   if(i < 4) carsRow2[(i + 12) / 2] = 0;
   else carsRow2[(i - 4) / 2] = 0;
/*CAR 2*/
   if(i < 8) lc.setLed(0, colOff[i + 8], myRow[2], false);
   else lc.setLed(0, colOff[i - 8], myRow[2], false);
   if(i < 12) carsRow2[(i + 4) / 2] = 0;
   else carsRow2[(i - 12) / 2] = 0;
   
//ROW 4 OFF
/*CAR 1*/
   if(i < 14) lc.setLed(0, colOff[i + 2], myRow[4], false);
   else lc.setLed(0, colOff[i - 14], myRow[4], false);
   if (i > 1) carsRow4[(i - 2) / 2] = 0;
   carsRow4[7] = 0;
/*CAR 2*/
   if(i > 1) lc.setLed(0, colOff[i - 2], myRow[4], false);
   else lc.setLed(0, colOff[i + 14], myRow[4], false);
   if(i < 6) carsRow4[(i + 10) / 2] = 0;
   else carsRow4[(i - 6) / 2] = 0;

//ROW 5 OFF
/*CAR 1*/
   if(i > 1) lc.setLed(0, colOff[i - 2], myRow[5], false);
   else lc.setLed(0, colOff[i + 14], myRow[5], false);
   if(i < 6) carsRow5[(i + 10) / 2] = 0;
   else carsRow5[(i - 6) / 2] = 0;
   
   i++;
}

void carsLvl3(){
   if(i == 16) i = 0;
  
//ROW 2 ON
/*CAR 1*/
   if(i < 10){
      lc.setLed(0, colOn[i + 6], myRow[2], true); 
      carsRow2[(i + 6) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 10], myRow[2], true); 
      carsRow2[(i - 10) / 2] = 1;
   }
/*CAR 2*/
   if(i < 4){
      lc.setLed(0, colOn[i + 12], myRow[2], true); 
      carsRow2[(i + 12) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 4], myRow[2], true); 
      carsRow2[(i - 4) / 2] = 1;
   }

//ROW 3 ON      
/*CAR 1*/
   if(i < 12){
      lc.setLed(0, colOn[i + 4], myRow[3], true); 
      carsRow3[(i + 4) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 12], myRow[3], true); 
      carsRow3[(i - 12) / 2] = 1;
   }
/*CAR 2*/
   if(i < 4){
      lc.setLed(0, colOn[i + 12],myRow[3], true); 
      carsRow3[(i + 12) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 4], myRow[3], true); 
      carsRow3[(i - 4) / 2] = 1;
   }

//ROW 4 ON      
/*CAR 1*/
   lc.setLed(0, colOn[i], myRow[4], true); 
   carsRow4[i / 2] = 1;
/*CAR 2*/
   if(i < 6){
      lc.setLed(0, colOn[i + 10], myRow[4], true); 
      carsRow4[(i + 10) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 6], myRow[4], true); 
      carsRow4[(i - 6) / 2] = 1;
   }

//ROW 6 ON  
/*CAR 1*/    
   lc.setLed(0, colOn[i], myRow[6], true); 
   carsRow6[i / 2] = 1;
/*CAR 2*/      
   if(i < 12){
      lc.setLed(0, colOn[i + 4], myRow[6], true); 
      carsRow6[(i + 4) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 12], myRow[6], true); 
      carsRow6[(i - 12) / 2] = 1;
   }
/*CAR 3*/      
   if(i < 8){
      lc.setLed(0, colOn[i + 8], myRow[6], true); 
      carsRow6[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[6], true); 
      carsRow6[(i - 8) / 2] = 1;
   }
/*CAR 4*/      
   if(i < 4){
      lc.setLed(0, colOn[i + 12], myRow[6], true); 
      carsRow6[(i + 12) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 4], myRow[6], true); 
      carsRow6[(i - 4) / 2] = 1;
   }

//ROW 2 OFF
/*CAR 1*/
   if(i < 12) lc.setLed(0, colOff[i + 4], myRow[2], false);
   else lc.setLed(0, colOff[i - 12], myRow[2], false);
   carsRow2[i / 2] = 0;
/*CAR 2*/
   if(i > 1) lc.setLed(0, colOff[i - 2], myRow[2], false);
   else lc.setLed(0, colOff[i + 14], myRow[2], false);
   if(i < 6) carsRow2[(i + 10) / 2] = 0;
   else carsRow2[(i - 6) / 2] = 0;

//ROW 3 OFF
/*CAR 1*/
   if(i < 12) lc.setLed(0, colOff[i + 4], myRow[3], false);
   else lc.setLed(0, colOff[i - 12], myRow[3], false);
   carsRow3[i / 2] = 0;
/*CAR 2*/
   if(i < 4) lc.setLed(0, colOff[i + 12], myRow[3], false);
   else lc.setLed(0, colOff[i - 4], myRow[3], false);
   if(i < 8) carsRow3[(i + 8) / 2] = 0;
   else carsRow3[(i - 8) / 2] = 0;

//ROW 4 OFF      
/*CAR 1*/
   lc.setLed(0, colOff[i], myRow[4], false);
   if(i < 4) carsRow4[(i + 12) / 2] = 0;
   else carsRow4[(i - 4) / 2] = 0;
/*CAR 2*/
   if(i < 6) lc.setLed(0, colOff[i + 10], myRow[4], false);
   else lc.setLed(0, colOff[i - 6], myRow[4], false);
   if(i < 10) carsRow4[(i + 6) / 2] = 0;
   else carsRow4[(i - 10) / 2] = 0;
//ROW 6 OFF      
/*CAR 1*/
   if(i < 14) lc.setLed(0, colOff[i + 2], myRow[6], false);
   else lc.setLed(0, colOff[i - 14], myRow[6], false);
   if(i < 2) carsRow6[(i + 14) / 2] = 0;
   else carsRow6[(i - 2) / 2] = 0;
/*CAR 2*/      
   if(i < 10) lc.setLed(0, colOff[i + 6], myRow[6], false);
   else lc.setLed(0, colOff[i - 10], myRow[6], false);
   if(i < 14) carsRow6[(i + 2) / 2] = 0;
   else carsRow6[(i - 14) / 2] = 0;
/*CAR 3*/      
   if(i < 6) lc.setLed(0, colOff[i + 10], myRow[6], false);
   else lc.setLed(0, colOff[i - 6], myRow[6], false);
   if(i < 10) carsRow6[(i + 6) / 2] = 0;
   else carsRow6[(i - 10) / 2] = 0;
/*CAR 4*/      
   if(i < 2) lc.setLed(0, colOff[i + 14], myRow[6], false);
   else lc.setLed(0, colOff[i - 2], myRow[6], false);
   if(i < 6) carsRow6[(i + 10) / 2] = 0;
   else carsRow6[(i - 6) / 2] = 0;
   
   i++;
}

void carsLvl4(){
   if(i == 16) i = 0;
   
//ROW 2 ON
/*CAR 1*/
   if(i < 14)
     {lc.setLed(0, colOn[i + 2], myRow[2], true); 
     carsRow2[(i + 2) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 14], myRow[2], true); 
      carsRow2[(i - 14) / 2] = 1;
   }
/*CAR 2*/
   if(i < 10){
      lc.setLed(0, colOn[i + 6], myRow[2], true); 
      carsRow2[(i + 6) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 10], myRow[2], true); 
      carsRow2[(i - 10) / 2] = 1;
   }
/*CAR 3*/
   if(i < 4){
      lc.setLed(0, colOn[i + 12], myRow[2], true); 
      carsRow2[(i + 12) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 4], myRow[2], true); 
      carsRow2[(i - 4) / 2] = 1;
   }

//ROW 3 ON
/*CAR 1*/
   if(i < 12){
      lc.setLed(0, colOn[i + 4], myRow[3], true); 
      carsRow3[(i + 4) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 12], myRow[3], true); 
      carsRow3[(i - 12) / 2] = 1;
   }
/*CAR 2*/
   if(i < 6){
      lc.setLed(0, colOn[i + 10],myRow[3], true); 
      carsRow3[(i + 10) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 6], myRow[3], true); 
      carsRow3[(i - 6) / 2] = 1;
   }

//ROW 5 ON
/*CAR 1*/
   lc.setLed(0, colOn[i], myRow[5], true); 
   carsRow5[i / 2] = 1;
/*CAR 2*/
   if(i < 8){
      lc.setLed(0, colOn[i + 8], myRow[5], true); 
      carsRow5[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[5], true); 
      carsRow5[(i - 8) / 2] = 1;
   }

//ROW 6 ON
/*CAR 1*/
   if(i < 8){
      lc.setLed(0, colOn[i + 8], myRow[6], true); 
      carsRow6[(i + 8) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 8], myRow[6], true); 
      carsRow6[(i - 8) / 2] = 1;
   }
/*CAR 2*/
   if(i < 2){
      lc.setLed(0, colOn[i + 14], myRow[6], true); 
      carsRow6[(i + 14) / 2] = 1;
   }
   else{
      lc.setLed(0, colOn[i - 2], myRow[6], true); 
      carsRow6[(i - 2) / 2] = 1;
   }

//ROW 2 OFF
/*CAR 1*/
   if(i < 14) lc.setLed(0, colOff[i + 2], myRow[2], false);
   else lc.setLed(0, colOff[i - 14], myRow[2], false);
   if(i < 2) carsRow2[(i + 14) / 2] = 0;
   else carsRow2[(i - 2) / 2] = 0;
/*CAR 2*/
   if(i < 8) lc.setLed(0, colOff[i + 8], myRow[2], false);
   else lc.setLed(0, colOff[i - 8], myRow[2], false);
   if(i < 12) carsRow2[(i + 4) / 2] = 0;
   else carsRow2[(i - 12) / 2] = 0;
/*CAR 3*/
   if(i < 4) lc.setLed(0, colOff[i + 12], myRow[2], false);
   else lc.setLed(0, colOff[i - 4], myRow[2], false);
   if(i < 8) carsRow2[(i + 8) / 2] = 0;
   else carsRow2[(i - 8) / 2] = 0;
   
//ROW 3 OFF
/*CAR 1*/
   if(i < 14) lc.setLed(0, colOff[i + 2], myRow[3], false);
   else lc.setLed(0, colOff[i - 14], myRow[3], false);
   if(i < 2) carsRow3[(i + 14) / 2] = 0;
   else carsRow3[(i - 2) / 2] = 0;
/*CAR 2*/
   if(i < 6) lc.setLed(0, colOff[i + 10], myRow[3], false);
   else lc.setLed(0, colOff[i - 6], myRow[3], false);
   if(i < 10) carsRow3[(i + 6) / 2] = 0;
   else carsRow3[(i - 10) / 2] = 0;

//ROW 5 OFF
/*CAR 1*/
   lc.setLed(0, colOff[i], myRow[5], false);
   if(i < 4) carsRow5[(i + 12) / 2] = 0;
   else carsRow5[(i - 4) / 2] = 0;
/*CAR 2*/
   if(i < 10) lc.setLed(0, colOff[i + 6], myRow[5], false);
   else lc.setLed(0, colOff[i - 10], myRow[5], false);
   if(i < 14) carsRow5[(i + 2) / 2] = 0;
   else carsRow5[(i - 14) / 2] = 0;

//ROW 6 OFF
/*CAR 1*/
   if(i < 12) lc.setLed(0, colOff[i + 4], myRow[6], false);
   else lc.setLed(0, colOff[i - 12], myRow[6], false);
   carsRow6[i / 2] = 0;
/*CAR 2*/
   if(i < 2) lc.setLed(0, colOff[i + 14], myRow[6], false);
   else lc.setLed(0, colOff[i - 2], myRow[6], false);
   if(i < 6) carsRow6[(i + 10) / 2] = 0;
   else carsRow6[(i - 6) / 2] = 0;
   
   i++;
}

/****************************************************************************
 * Functiile lvl genereaza miscarea masinilor la un anumit interval de timp *
 ****************************************************************************/
void lvl1(){
   currentTime = millis();
   if(previousTime == 0) startTime = currentTime;
   if(currentTime - previousTime >= 250){
      previousTime = currentTime;
      carsLvl1();
   }
}
void lvl2(){
   currentTime = millis();
   if(previousTime == 0) startTime = currentTime;
   if(currentTime - previousTime >= 250){
      previousTime = currentTime;
      carsLvl2();
   }
}
void lvl3(){
   currentTime = millis();
   if(previousTime == 0) startTime = currentTime;
   if(currentTime - previousTime >= 205){
      previousTime = currentTime;
      carsLvl3();
   }
}
void lvl4(){
   currentTime = millis();
   if(previousTime == 0) startTime = currentTime;
   if(currentTime - previousTime >= 150){
      previousTime = currentTime;
      carsLvl4();
   }
}

/***************************************************************************
 * Functiile checkLvl verifica daca playerul a fost calcat de masina si in *
 * caz afirmativ se scade o viata si playerul revine la pozitia initiala   *                                  *
 ***************************************************************************/
void checkLvl1(){     
   if(oY == 2 && carsRow2[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 4 && carsRow4[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 6 && carsRow6[oX] == 1) {oY = 0; life -= 1;}
}
void checkLvl2(){
   if(oY == 2 && carsRow2[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 4 && carsRow4[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 5 && carsRow5[oX] == 1) {oY = 0; life -= 1;}
}
void checkLvl3(){
   if(oY == 2 && carsRow2[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 3 && carsRow3[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 4 && carsRow4[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 6 && carsRow6[oX] == 1) {oY = 0; life -= 1;}
}
void checkLvl4(){
   if(oY == 2 && carsRow2[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 3 && carsRow3[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 5 && carsRow5[oX] == 1) {oY = 0; life -= 1;}
   if(oY == 6 && carsRow6[oX] == 1) {oY = 0; life -= 1;}
}

void setup(){
  lc.shutdown(0,false);// porneste matricea
  lc.setIntensity(0,1);// setare luminozitate (0~15)
  lc.clearDisplay(0);// inchide toate led-urile
  pinMode(button, INPUT); // button este input
  digitalWrite(button, HIGH); // se initializeaza button ca pornit
  lcd.begin(16,2);// porneste ecranul cu 16 coloane si 2 linii
  lcd.clear();// golire ecran  
}

void loop(){
   while(gameOn == 0){
      delay(200);
      showOff();
      buttonState = digitalRead(button);
      if (buttonState != lastButtonState){
         i = 0; lc.clearDisplay(0);
         showOn();
         lcd.clear();
         gameOn = 1; 
         menu = 0; 
         displayGameName();
      }
   }
  
   if (gameOn == 1){
      if(menu == 0) selectMenu();
      if(menu == 1) pickLevel();
      if(menu == 2) showHighscore();
      if(menu == 3) exitOption();
      if(lvl != 0 && life != 0){
         lcd.clear();
         startLevel();
         playerMovement();
      }
      
      if(life == 0){
         lc.clearDisplay(0);
         loserOption();
      }
      
      if(life > 10){
         lc.clearDisplay(0);
         calculateScore();
         winnerOption();
      }
   } 
}
