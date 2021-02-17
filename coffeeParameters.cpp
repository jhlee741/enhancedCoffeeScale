#include "coffeeParameters.h"

coffeeParameters::coffeeParameters(){

}

void coffeeParameters::setBeans(float beans){
  beansIn = beans;
}

int coffeeParameters::bloomWeight(){
  return (beansIn+0.5)*2;
}

float coffeeParameters::getTotalBrew(){
  return totalBrew;
}

float coffeeParameters::getPhaseOne(){
  return phaseOne;
}

float coffeeParameters::getPhaseTwo(){
  return phaseTwo;
}

float coffeeParameters::getPhaseOneRate(){
  return phaseOneRate;
}

float coffeeParameters::getPhaseTwoRate(){
  return phaseTwoRate;
}

float coffeeParameters::currentRate(unsigned long previousTime, unsigned long currentTime, float valueBefore, float valueCurrent){
  float currentRate;
  currentRate = (valueCurrent-valueBefore)/(currentTime-previousTime);
  return currentRate;
}
