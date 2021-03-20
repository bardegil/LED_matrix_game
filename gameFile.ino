#include "LedControl.h"
#include "button_response.h"

/*Things thst needs to be worked on!
 * 
 * Created by Bård Egil Eilertsen
 *            12.11.2019 to 13.11.2019
 * 
 * All known bugs are taken care of
 * Missing interrups for button press
 */
void greenButton();

LedControl lc=LedControl(12,10,11,4);  // Pins: DIN,CLK,CS, # of Display connected
button_response my_green_btn(3,greenButton);
/*
 * Variables
 */

unsigned long delayTime=200;  // Delay between Frames
int maxLevel = 15;
int level = 0;
int panel = 0;
int maxLife = 4;
int life = maxLife;
int collum = 0;
int results[16][16]; // save the results. all results are convertet to right moving animation
int green = 1;
int red = 1;
bool right = true;
bool newLevel = true;
byte frame[] = { B00000000, 
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000
                 };

/*
 * Functions
 * nextFrame - Animation function
 * greenButton - Game mechanic. What happens when the green button is pressed
 */



void nextFrame(){
  if(newLevel){
      for(int i = 0; i < life; i++){
        frame[7-i] = frame[7-i] + 3 * (0.1+pow(2,2*level));
      }
      collum = 7-life;
      right = true;
      newLevel = false;
      
    } else if (!newLevel && right){
      
      frame[collum] = frame[collum] + 3 * (0.1+pow(2,2*level));// pow() is a floating point function, so I've probably getting something like 1.99999, which is truncated to 1 when assigned to an integer value.
      if(level==0 or ((collum+life-1) <= results[level-1][0]) && (results[level-1][0] <= collum)){
        frame[collum+life] = 0;
      } else if (level !=0) {
        frame[collum+life] = frame[collum+life] - 3 * (pow(2,2*level));
        }
      collum--;
        if (collum == -1){
          collum = life;
          right = false;
          }
          
    } else if(!newLevel && !right){
      
      frame[collum] = frame[collum] + 3 * (0.1+pow(2,2*level));
      if(level==0 or ((collum+life-1) <= results[level-1][0]) && (results[level-1][0] <= collum)){
        frame[collum-life] = 0;
      } else if (level !=0) {
        frame[collum-life] = frame[collum-life] - 3 * (pow(2,2*level));
        }
        collum++;
          if (collum == 8){
            collum = 7 - life;
            right = true;
          }
      }

  if(panel == 0){
    for (int i = 0; i < 8; i++){
      lc.setRow(panel,i,frame[i]);
      }
  } else if(panel == 1){
    for (int i = 0; i < 8; i++){
      lc.setRow(panel,i,frame[i]);
      }
  } else if(panel == 2){
    for (int i = 0; i < 8; i++){
      lc.setRow(panel,i,frame[i]);
      }
  } else if(panel == 3){
    for (int i = 0; i < 8; i++){
      lc.setRow(panel,i,frame[i]);
      }
  }
  return;
}

void greenButton(){
     if(panel > 0 or level > 0){
          if(results[level + 4*panel][0] == results[level + 4*panel-1][0]){ // Betyr alle tilfeller hvor man ligger til venstre
          } else if(results[level + 4*panel][0] > results[level + 4*panel-1][0]){ //Til venstre for den under
            if((results[level + 4*panel][0] - results[level + 4*panel-1][0])==1){
              frame[results[level + 4*panel][0] + life - 1] = frame[results[level + 4*panel][0] + life -1] - 3 * (pow(2,2*level));
              results[level + 4*panel][1] = results[level + 4*panel][1] - 1; 
              life = life - 1;  
              if(life<=0){
                return;
              }
            }else if((results[level + 4*panel][0] - results[level + 4*panel-1][0])==2){
                  frame[results[level + 4*panel][0] + life - 2] = frame[results[level + 4*panel][0] + life - 2] - 3 * (pow(2,2*level));
                  frame[results[level + 4*panel][0] + life - 1] = frame[results[level + 4*panel][0] + life - 1] - 3 * (pow(2,2*level));
                  results[level+ 4*panel][1] = results[level+ 4*panel][1] - 2;  
                  life = life - 2; 
                if(life<=0){
                  return;
                }            
            }else if((results[level + 4*panel][0] - results[level + 4*panel-1][0])==3){  
                  frame[results[level + 4*panel][0] + life - 3] = frame[results[level + 4*panel][0] + life - 3] - 3 * (pow(2,2*level));
                  frame[results[level + 4*panel][0] + life - 2] = frame[results[level + 4*panel][0] + life - 2] - 3 * (pow(2,2*level));
                  frame[results[level + 4*panel][0] + life - 1] = frame[results[level + 4*panel][0] + life - 1] - 3 * (pow(2,2*level));
                  results[level + 4*panel][1] = results[level + 4*panel][1] - 3;  
                  life = life - 3;      
                if(life<=0){
                  return;
                }                   
            }else if((results[level + 4*panel][0] - results[level + 4*panel-1][0]) > 3){
                  life = 0;  
                  return;
            }
          } else if(results[level + 4*panel][0] < results[level + 4*panel-1][0]){//Til høyre for den under      
            if((results[level + 4*panel-1][0] - results[level + 4*panel][0])==1){
              frame[results[level + 4*panel][0]] = frame[results[level + 4*panel][0]] - 3 * (pow(2,2*level));
              results[level + 4*panel][0] = results[level + 4*panel][0] + 1;  
              results[level + 4*panel][1] = results[level + 4*panel][1] - 1; 
              life = life - 1;   
              if(life<=0){
                return;
              }     
            }else if((results[level + 4*panel-1][0] - results[level + 4*panel][0])==2){
              frame[results[level + 4*panel][0]+1] = frame[results[level + 4*panel][0]+1] - 3 * (pow(2,2*level));
              frame[results[level + 4*panel][0]] = frame[results[level + 4*panel][0]] - 3 * (pow(2,2*level));
              results[level + 4*panel][0] = results[level + 4*panel][0] + 2;
              results[level + 4*panel][1] = results[level + 4*panel][1] - 2; 
              life = life - 2;
              if(life<=0){
                return;
              }       
            }else if((results[level + 4*panel-1][0] - results[level + 4*panel][0])==3){
              frame[results[level + 4*panel][0]+2] = frame[results[level + 4*panel][0]+2] - 3 * (pow(2,2*level));
              frame[results[level + 4*panel][0]+1] = frame[results[level + 4*panel][0]+1] - 3 * (pow(2,2*level));
              frame[results[level + 4*panel][0]] = frame[results[level + 4*panel][0]] - 3 * (pow(2,2*level));
              results[level + 4*panel][0] = results[level + 4*panel][0] + 3;   
              results[level + 4*panel][1] = results[level + 4*panel][1] - 3; 
              life = life - 3;    
              if(life<=0){
                return;
              }            
            } else if((results[level + 4*panel][0] - results[level + 4*panel  -1][0]) > 3){
                  life = 0; 
                  return; 
            } 
          }
        }
          if(panel == 0){
            for (int i = 0; i < 8; i++){
              lc.setRow(panel,i,frame[i]);
            }
          } else if(panel == 1){
            for (int i = 0; i < 8; i++){
              lc.setRow(panel,i,frame[i]);
            }
          } else if(panel == 2){
            for (int i = 0; i < 8; i++){
              lc.setRow(panel,i,frame[i]);
            }
          } else if(panel == 3){
            for (int i = 0; i < 8; i++){
              lc.setRow(panel,i,frame[i]);
            }
          }
  }
  
/*void buttonRed() { This is interrupt. Will need this
  int red = digitalRead(2);
}

void buttonGreen() {
  int green = digitalRead(3);
}*/
  
void setup()
{

  Serial.begin (9600); // baud rate set
  
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

/*  attachInterrupt(digitalPinToInterrupt(2), buttonRed, FALLING);
    attachInterrupt(digitalPinToInterrupt(3), buttonGreen, FALLING);
*/
  for (int i = 0; i < 4; i++){
  lc.shutdown(i,false);  // Wake up displays
  lc.setIntensity(i,2);  // Set intensity levels
  lc.clearDisplay(i);  // Clear Displays
  }
}

void loop()
{
  for(int i = 0; i < maxLevel; i++){
    int red = digitalRead(2);
    int green = digitalRead(3);
    while(green == 1 && red == 1){
      red = digitalRead(2);
      green = digitalRead(3);
      delay(delayTime - 12*(level+4*panel));
      nextFrame();
      }
      
      if(green == 0 && life > 0){ // next level
        if(right){//Saves the result. Saves collum and life assuming it goes right.
          results[level + 4*panel][0] = collum + 1;
          results[level + 4*panel][1] = life;
        } else if(!right){
          results[level + 4*panel][0] = collum-life;
          results[level + 4*panel][1] = life;
        }

          greenButton();

        level++;
        newLevel = true;
        if(level == 4){
          panel++;
          level=0;
          frame[0] = B00000000;
          frame[1] = B00000000;
          frame[2] = B00000000;
          frame[3] = B00000000;
          frame[4] = B00000000;
          frame[5] = B00000000;
          frame[6] = B00000000;
          frame[7] = B00000000;
          }
        while(green == 0){
          green = digitalRead(3);
          delay(10);
          }
      }
      
      if(red == 0 or panel == 4){//Restart the game
        i = 0;
        level = 0;
        panel = 0;
        life = maxLife;
        collum = 0;
        right = true;
        newLevel = true;
        for(int i = 0; i < 7; i++){
          frame[i] = 0;
        }
        for (int i = 0; i < 4; i++){
          lc.shutdown(i,false);  // Wake up displays
          lc.setIntensity(i,2);  // Set intensity levels
          lc.clearDisplay(i);  // Clear Displays
        }
        delay(delayTime);
      }
  }
}
