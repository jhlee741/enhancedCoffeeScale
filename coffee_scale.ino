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

float value, valueBefore, valueCurrent, valueTotal, pouringRate, valueBloom, valuePhaseOne;
int flag_1, flag_2, flag_3, flag_4, flag_5, flag_6, minutes, seconds;
unsigned long startingTime, previousTime, currentTime, finalTime;
coffeeParameters currentBrew;

void setup() {
  flag_1 = 0;
  flag_2 = 0;
  flag_3 = 0;
  flag_4 = 0;
  value = 0;
  valueBefore = 0;
  valueCurrent = 0;
  valueTotal = 0;
  valueBloom = 0;
  valuePhaseOne = 0;
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
  lcdReset(0, 0);
  lcd.print("Let's brew!");
}
void loop() {
  if (digitalRead(sw_2) == HIGH) {
    reset();
  }

  if (digitalRead(sw_1) == HIGH) { //Placing coffee on scale

    if (digitalRead(sw_2) == HIGH) {
      reset();
    }

    lcdReset(0, 0);
    lcd.print("Place beans...");
    lcd.setCursor(0, 1);
    lcd.print("Beans in:");
    while (flag_1 == 0) {

      if (digitalRead(sw_2) == HIGH) {
        flag_1 = 1;
        reset();
      }

      value = measure(value);
      lcd.setCursor(9, 1);
      lcd.print(value); //Does not round properly
      lcd.setCursor(15, 1);
      lcd.print("g");
      if (digitalRead(sw_3) == HIGH) {
        flag_1 = 1;
      }
    }

  }
  if (flag_1 == 1) {

    if (digitalRead(sw_2) == HIGH) {
      reset();
    }

    currentBrew.setBeans(value);
    currentBrew.setTotalBrew(currentBrew.getBeans());
    currentBrew.setPhaseOne(currentBrew.getBeans());
    currentBrew.setPhaseOneRate(currentBrew.getPhaseOne());

    lcdReset(0, 0);
    lcd.print("Blooming Stage");
    lcd.setCursor(0, 1);
    lcd.print("Start Countdown");
    scale.tare(10);

    if (digitalRead(sw_3) == HIGH) { //Blooming Stage

      if (digitalRead(sw_2) == HIGH) {
        reset();
      }

      lcdReset(0, 0);
      lcd.print("Starting in 3");
      lcd.setCursor(12, 0);
      delay(1000);
      lcd.print("2");
      lcd.setCursor(12, 0);
      delay(1000);
      lcd.print("1");
      delay(1000);
      lcdReset(0, 0);
      lcd.print("Target:");
      lcd.setCursor(7, 0);
      lcd.print(currentBrew.bloomWeight());
      lcd.setCursor(0, 1);
      startingTime = millis();
      while (flag_2 == 0) {

        if (digitalRead(sw_2) == HIGH) {
          flag_2 = 1;
          reset();
        }

        value = measure(value);
        lcd.setCursor(0, 1);
        lcd.print(value);
        if (value >= currentBrew.bloomWeight()) {
          flag_1 = 0;
          flag_2 = 1;
          digitalWrite(red, HIGH);
        }
      }
      if (flag_2 == 1) {

        if (digitalRead(sw_2) == HIGH) {
          reset();
        }
        valueTotal += value;
        valueBloom = value;
        lcdReset(0, 0);
        lcd.print("Swirl until all");
        lcd.setCursor(0, 1);
        lcd.print("grounds are wet");
        delay(2000);
        lcd.clear();
        while (flag_3 == 0) { //Loop for a 45 second blooming period

          if (digitalRead(sw_2) == HIGH) {
            reset();
          }

          value = measure(value);
          lcd.setCursor(0, 0);
          lcd.print("Blooming...");
          lcd.setCursor(0, 1);
          lcd.print("Wait for Green!");
          if (millis() - startingTime >= 43000) {

            if (digitalRead(sw_2) == HIGH) {
              reset();
            }

            digitalWrite(red, LOW);
            digitalWrite(green, HIGH);
            lcdReset(0, 0);
            lcd.print("Bloom Complete");
            flag_2 = 0;
            flag_3 = 1;
            delay(2000);
            digitalWrite(green, LOW);
          }
        }
        if (flag_3 == 1) { //Stage 1 of brew (Rapid pour)

          if (digitalRead(sw_2) == HIGH) {
            reset();
          }

          lcdReset(0, 0);
          lcd.print("Rapid pour phase");
          previousTime = millis();
          valueBefore = value;
          while (flag_4 == 0) {

            if (digitalRead(sw_2) == HIGH) {
              flag_4 = 1;
              reset();
            }

            value = measure(value);
            lcd.setCursor(0, 1);
            lcd.print(value);
            currentTime = millis();
            valueCurrent = measure(value);
            valueTotal += (valueCurrent - valueBefore);
            pouringRate = currentBrew.currentRate(previousTime, currentTime, valueBefore, valueCurrent);

            if ((valueTotal - valueBloom) > currentBrew.getPhaseOne()) {
              valuePhaseOne = valueTotal - valueBloom;
              flag_3 = 0;
              flag_4 = 1;

            }
            else if (pouringRate > (1.25 * currentBrew.getPhaseOneRate())) { //Too fast!
              if(digitalRead(blue) == LOW){
                digitalWrite(red, LOW);
                digitalWrite(green, LOW);
                digitalWrite(blue, HIGH);                
              }
            }
            else if (pouringRate >= (0.75 * currentBrew.getPhaseOneRate()) && pouringRate <= (1.25 * currentBrew.getPhaseOneRate())) { //Just right
              if(digitalRead(green) == LOW){
                digitalWrite(red, LOW);
                digitalWrite(green, HIGH);
                digitalWrite(blue, LOW);                
              }
            }
            else if (pouringRate < (0.75 * currentBrew.getPhaseOneRate())) { //Too slow
              if(digitalRead(red) == LOW){
                digitalWrite(red, HIGH);
                digitalWrite(green, LOW);
                digitalWrite(blue, LOW);                
              }
            }
            previousTime = currentTime;
            valueBefore = valueCurrent;
          }

          if (flag_4 == 1) { //Second phase

            if (digitalRead(sw_2) == HIGH) {
              reset();
            }

            digitalWrite(blue, LOW);
            digitalWrite(green, LOW);
            digitalWrite(red, LOW);
            lcdReset(0, 0);
            lcd.print("Second phase");
            previousTime = millis();
            valueBefore = value;
            while (flag_5 == 0) {

              if (digitalRead(sw_2) == HIGH) {
                flag_5 = 1;
                reset();
              }

              value = measure(value);
              lcd.setCursor(0, 1);
              lcd.print(value);
              currentTime = millis();
              valueCurrent = measure(value);
              valueTotal += (valueCurrent - valueBefore);
              pouringRate = currentBrew.currentRate(previousTime, currentTime, valueBefore, valueCurrent);

              if (valueTotal > currentBrew.getTotalBrew()) {
                flag_4 = 0;
                flag_5 = 1;
              }
              else if (pouringRate > (1.25 * currentBrew.getPhaseTwoRate())) { //Too fast!
                if(digitalRead(blue) == LOW){
                  digitalWrite(red, LOW);
                  digitalWrite(green, LOW);
                  digitalWrite(blue, HIGH);                
                }
              }
              else if (pouringRate >= (0.75 * currentBrew.getPhaseTwoRate()) && pouringRate <= (1.25 * currentBrew.getPhaseTwoRate())) { //Just right
                if(digitalRead(green) == LOW){
                  digitalWrite(red, LOW);
                  digitalWrite(green, HIGH);
                  digitalWrite(blue, LOW);                
                }
              }
              else if (pouringRate < (0.75 * currentBrew.getPhaseTwoRate())) { //Too slow
                if(digitalRead(red) == LOW){
                  digitalWrite(red, HIGH);
                  digitalWrite(green, LOW);
                  digitalWrite(blue, LOW);                
                }
              }
              previousTime = currentTime;
              valueBefore = valueCurrent;
            }
            if (flag_5 == 1) { //Drawdown phase
              digitalWrite(green, HIGH);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Stop!");
              delay(2000);
              if (digitalRead(sw_2) == HIGH) {
                reset();
              }

              digitalWrite(blue, LOW);
              digitalWrite(green, LOW);
              digitalWrite(red, LOW);
              lcdReset(0, 0);
              lcd.print("Drawdown Phase");
              delay(7500);
              lcd.setCursor(0, 1);
              lcd.print("Stir Gently :)");
              digitalWrite(green, HIGH);
              delay(5000);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Drawdown Phase");
              digitalWrite(green, LOW);

              while (flag_6 == 0) {

                if (digitalRead(sw_2) == HIGH) {
                  flag_6 = 0;
                  reset();
                }

                if (digitalRead(sw_3) == HIGH) {
                  finalTime = millis();
                  minutes = ((finalTime - startingTime)/1000)/60;
                  seconds = ((finalTime - startingTime)/1000)%60;
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Brew Complete!");
                  lcd.setCursor(0, 1);
                  lcd.print("Brew time:");
                  lcd.setCursor(10,1);
                  lcd.print(minutes);
                  lcd.setCursor(11,1);
                  lcd.print(":");
                  lcd.setCursor(12,1);
                  lcd.print(seconds);
                  delay(5000);
                  flag_6 = 1;
                }
              }
            }
          }
        }
      }
    }
  }
}
void lcdReset(int col, int row) {
  lcd.clear();
  lcd.setCursor(col, row);

}

float measure(float value) {
  value = scale.get_value() * scale_val;
  return value;
}

void reset() {
  lcd.clear();
  value = 0;
  valueBefore = 0;
  valueCurrent = 0;
  valueTotal = 0;
  valueBloom = 0;
  valuePhaseOne = 0;
  scale.tare(10);
  flag_1 = 0;
  flag_2 = 0;
  flag_3 = 0;
  flag_4 = 0;
  flag_5 = 0;
  flag_6 = 0;
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  lcd.print("Let's brew!");
  loop();
  //make sure to remember to delete the currentBrew object
}
