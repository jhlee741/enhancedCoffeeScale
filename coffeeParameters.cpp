#include "coffeeParameters.h"

coffeeParameters::coffeeParameters(){

}

void coffeeParameters::setBeans(float beans){
  beansIn = beans;
}

float coffeeParameters::getBeans(){
  return beansIn;
}

int coffeeParameters::bloomWeight(){
  return (beansIn+0.5)*2;
}

void coffeeParameters::setTotalBrew(float beans){
  totalBrew = beans*16.6667;
}

void coffeeParameters::setPhaseOne(float beans){
  phaseOne  = totalBrew*0.6;
}

float coffeeParameters::getTotalBrew(){
  return totalBrew;
}

float coffeeParameters::getPhaseOne(){
  return phaseOne;
}

void coffeeParameters::setPhaseOneRate(float phaseOne){
  phaseOneRate = phaseOne/30000;
}

void coffeeParameters::setPhaseTwoRate(float phaseTwo){
  phaseTwoRate = phaseTwo/30000;
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
