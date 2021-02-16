#include <HX711.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define DT A3
#define SCK A2
#define sw_1 9
#define sw_2 8
#define scale_val 0.212389816)/79.96728456

HX711 scale;

float value;
int flag;

void setup() {
  flag = 0;
  value = 0;
  lcd.begin(16, 2);
  scale.begin(DT, SCK);
  scale.tare(10);
  pinMode(sw_1, INPUT_PULLUP);
  lcd.print("Ready");
}
void loop() {
  lcd.print("Let's brew!");
  if(digitalRead(sw_1)==HIGH){
    value = measure(value);
  }
  else if(digitalRead(sw_2)==HIGH){
    reset();
  }
}

float measure(float value){
    lcd.clear();
    value = (scale.get_value()*scale_val;
    return value;
}
void reset(){
  lcd.clear();
  value = 0;
  scale.tare(10);
  flag = 0;
  lcd.print("Ready");
}
