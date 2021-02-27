#include "anapla.hpp"

anapla::anapla(){
}

anapla::~anapla(){
}

void anapla::analyze(evtdata *evt){
  int i;

  // Hit position by TDC difference
  evt->grpla.pos[0] = evt->grpla.tdc[0] - evt->grpla.tdc[1];
  evt->grpla.pos[1] = evt->grpla.tdc[2] - evt->grpla.tdc[3];
  
  // Energy deposite by QDC mean
  evt->grpla.de[0] = TMath::Sqrt(evt->grpla.adc[0] * evt->grpla.adc[1]);
  evt->grpla.de[1] = TMath::Sqrt(evt->grpla.adc[2] * evt->grpla.adc[3]); 

  // MQDC32 & FERA QDC/TDC
  for(i=0; i<N_GRPLA_CH; i++){
    evt->grpla.fqdc[i] = evt->grpla.adc[i];
    evt->grpla.vqdc[i] = evt->mqdc.adc[i];    
    evt->grpla.ftdc[i] = evt->grpla.tdc[i];
  }

  // V1190 TDC
  int v1190_size = (int)(evt->v1190_hit_all.size());
  for(i=0; i<v1190_size; i++){
    if(evt->v1190_hit_all[i].geo==PLA_V1190_GEO &&
       evt->v1190_hit_all[i].ch<N_GRPLA_CH){
      evt->grpla.vtdc[i] = evt->v1190_hit_all[i].lead_raw+VDC_OFFSET;
    }
  }
}
