#pragma once

class coffeeParameters{
  private:
    float beansIn;
    float totalBrew = beansIn*16.6667;
    float phaseOne = totalBrew*0.6;
    float phaseTwo = totalBrew*0.4;
    float phaseOneRate = phaseOne/30000;
    float phaseTwoRate = phaseTwo/30000;
    
  public:
    coffeeParameters();
    void setBeans(float beans);
    int bloomWeight();
    float getTotalBrew();
    float getPhaseOne();
    float getPhaseTwo();
    float getPhaseOneRate();
    float getPhaseTwoRate();
    float currentRate(unsigned long previousTime, unsigned long currentTime, float valueBefore, float valueCurrent);
  
};
