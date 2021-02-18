#pragma once

class coffeeParameters{
  private:
    float beansIn;
    float totalBrew;
    float phaseOne;
    float phaseOneRate;
    float phaseTwoRate;
    
  public:
    coffeeParameters();
    void setBeans(float beans);
    float getBeans();
    int bloomWeight();
    void setTotalBrew(float beans);
    void setPhaseOne(float beans);
    float getTotalBrew();
    float getPhaseOne();
    void setPhaseOneRate(float phaseOne);
    void setPhaseTwoRate(float phaseTwo);
    float getPhaseOneRate();
    float getPhaseTwoRate();
    float currentRate(unsigned long previousTime, unsigned long currentTime, float valueBefore, float valueCurrent);
  
};
