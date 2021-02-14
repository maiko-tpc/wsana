#include "anapla.hpp"

anapla::anapla(){
}

anapla::~anapla(){
}

void anapla::analyze(evtdata *evt){
  evt->grpla.pos[0] = evt->grpla.tdc[0] - evt->grpla.tdc[1];
  evt->grpla.pos[1] = evt->grpla.tdc[2] - evt->grpla.tdc[3];
  
  evt->grpla.de[0]=sqrt(evt->grpla.adc[0] * evt->grpla.adc[1]);
  evt->grpla.de[1]=sqrt(evt->grpla.adc[2] * evt->grpla.adc[3]); 
}
