#include <HX711.h>
#include <LiquidCrystal.h>
#include "coffeeParameters.h"

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define DT A3
#define SCK A2
#define sw_1 9 //start button
#define sw_2 8 //reset switch
#define sw_3 10 //next stage button
#define red 3 //red led
#define green 2 //green led
#define blue 1 //blue led
//Initialize lights soon
#define scale_val ((0.212389816)/79.96728456)

HX711 scale;

float value;
int flag_1;
int flag_2;
int flag_3;
int flag_4;
unsigned long startingTime;
unsigned long previousTime;
unsigned long currentTime;
coffeeParameters currentBrew;

void setup() {
  flag_1 = 0;
  flag_2 = 0;
  flag_3 = 0;
  flag_4 = 0;
  value = 0;
  lcd.begin(16, 2);
  scale.begin(DT, SCK);
  scale.tare(10);
  pinMode(sw_1, INPUT_PULLUP);
  pinMode(sw_2, INPUT_PULLUP);
  pinMode(sw_3, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  lcdReset(0,0);  
  lcd.print("Let's brew!");
}
void loop() {
  
  if(digitalRead(sw_2)==HIGH){
    reset();
  }
  
  if(digitalRead(sw_1)==HIGH){ //Placing coffee on scale
    
    if(digitalRead(sw_2)==HIGH){
      reset();
    }
    
    lcdReset(0,0);
    lcd.print("Place beans...");
    lcd.setCursor(0,1);
    lcd.print("Beans in:");    
    while(flag_1 == 0){
      
      if(digitalRead(sw_2)==HIGH){
        reset();
      }      
      
      value = measure(value);
      lcd.setCursor(9,1);
      lcd.print(value); //Does not round properly
      lcd.setCursor(15,1);
      lcd.print("g");
      if(digitalRead(sw_3)==HIGH){
        flag_1 = 1;
      }
    }

  }
  if(flag_1 == 1){
    
    if(digitalRead(sw_2)==HIGH){
      reset();
    }
    
    currentBrew.setBeans(value);
    lcdReset(0,0);
    lcd.print("Blooming Stage");
    lcd.setCursor(0,1);
    lcd.print("Start Countdown");
    scale.tare(10);
    
    if(digitalRead(sw_3)==HIGH){ //Blooming Stage
 
      if(digitalRead(sw_2)==HIGH){
        reset();
      }   
     
      lcdReset(0,0);
      lcd.print("Starting in 3");
      lcd.setCursor(12,0);
      delay(1000);
      lcd.print("2");
      lcd.setCursor(12,0);
      delay(1000);
      lcd.print("1");
      delay(1000);
      lcdReset(0,0);
      lcd.print("Target:");
      lcd.setCursor(7,0);
      lcd.print(currentBrew.bloomWeight());
      lcd.setCursor(0,1);
      startingTime = millis();
      while(flag_2 == 0){
        
        if(digitalRead(sw_2)==HIGH){
          reset();
        }
        
        value = measure(value);
        lcd.setCursor(0,1);
        lcd.print(value);
        if(value >= currentBrew.bloomWeight()){
          flag_1 = 0;
          flag_2 = 1;
          digitalWrite(red, HIGH);
        }     
      }
      if(flag_2 == 1 && flag_1 == 0){

        if(digitalRead(sw_2)==HIGH){
          reset();
        }
        
        lcdReset(0,0);
        lcd.print("Swirl until all");
        lcd.setCursor(0,1);
        lcd.print("grounds are wet");
        delay(2000);      
        lcd.clear(); 
        while(flag_3 ==0){ //Loop for a 45 second blooming period    

          if(digitalRead(sw_2)==HIGH){
            reset();
          }
                       
          value = measure(value);
          lcd.setCursor(0,0);
          lcd.print("Blooming...");
          lcd.setCursor(0,1);
          lcd.print("Wait for Green!");
          if(millis()-startingTime >= 45000){
            digitalWrite(red, LOW);
            digitalWrite(green, HIGH);
            lcdReset(0,0);
            lcd.print("Bloom Complete");
            flag_2 = 0;
            flag_3 = 1;
          }
        }        
      }
    }     
  }
}

void lcdReset(int col, int row){
  lcd.clear();
  lcd.setCursor(col,row);
  
}

float measure(float value){
    value = scale.get_value()*scale_val;
    return value;
}

void reset(){
  lcd.clear();
  value = 0;
  scale.tare(10);
  flag_1 = 0;
  flag_2 = 0;
  flag_3 = 0;
  flag_4 = 0;
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  lcd.print("Let's brew!");
  //make sure to remember to delete the currentBrew object
}
