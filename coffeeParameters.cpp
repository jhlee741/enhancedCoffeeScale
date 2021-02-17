#include "coffeeParameters.h"

coffeeParameters::coffeeParameters(){

}

void coffeeParameters::setBeans(float beans){
  beansIn = beans;
}
int coffeeParameters::bloomWeight(){
  return (beansIn+0.5)*2;
}
