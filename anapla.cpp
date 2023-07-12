#include "anapla.hpp"

#define PLA_V1190_LEAD_MIN = -100000
#define PLA_V1190_LEAD_MAX =  100000

anapla::anapla(){
}

anapla::~anapla(){
}

void anapla::analyze(evtdata *evt){

  // Get FERA data from fera_hit_all
  GetFERAHit(evt);
  
  // Get GR&LAS MQDC data from hit_all_data
  GetMQDCHit(evt);

  // Get GR&LAS V1190 data from hit_all_data
  GetV1190Hit(evt);
  
  // GR ANA

  //  Hit position by TDC difference
  evt->grpla.pos[0] = evt->grpla.vtdc[0] - evt->grpla.vtdc[1];
  evt->grpla.pos[1] = evt->grpla.vtdc[2] - evt->grpla.vtdc[3];

  
  //  Energy deposite by QDC mean
  evt->grpla.de[0] = TMath::Sqrt(evt->grpla.vqdc[0] * evt->grpla.vqdc[1]);
  evt->grpla.de[1] = TMath::Sqrt(evt->grpla.vqdc[2] * evt->grpla.vqdc[3]); 

  //  RF
  for(int i=0; i<N_RF; i++){
    evt->rf[i] = evt->grpla.vtdc[8+i]-evt->grpla.vtdc[15];
  }

  // LAS ANA
  for(int i=0; i<6; i++){
    evt->laspla.de[i] = TMath::Sqrt(evt->laspla.vqdc[i*2] * evt->laspla.vqdc[i*2+1]);
    //    evt->laspla.pos[i] = evt->laspla.vtdc[i*4] - evt->laspla.vtdc[i*4+2];
    evt->laspla.pos[i] = evt->laspla.vtdc[i*4+2] - evt->laspla.vtdc[i*4];    
  }
}

int anapla::GetMQDCHit(evtdata *evt){
  int i, ch;
  int mqdc32_size = (int)(evt->mxdc32_hit_all.size());  
  int cnt=0;
  
  for(i=0; i<mqdc32_size; i++){

    // GR pla
    if(evt->mxdc32_hit_all[i].geo==0 && evt->mxdc32_hit_all[i].field==FIELD_PLA){
      ch = evt->mxdc32_hit_all[i].ch;
      evt->mqdc_nhit[0]++;
      if(ch<N_PLA_CH){
	evt->grpla.vqdc[ch] = evt->mxdc32_hit_all[i].adc;
	cnt++;
      }
    }

    // LAS pla
    if(evt->mxdc32_hit_all[i].geo==1 && evt->mxdc32_hit_all[i].field==FIELD_PLA){
      ch = evt->mxdc32_hit_all[i].ch;
      evt->mqdc_nhit[1]++;
      if(ch<N_PLA_CH){
	evt->laspla.vqdc[ch] = evt->mxdc32_hit_all[i].adc;
	cnt++;
      }
    }
    
  }
  return cnt;
}

int anapla::GetV1190Hit(evtdata *evt){
  int i, ch;
  int v1190_size = (int)(evt->v1190_hit_all.size());  
  int cnt=0;
  //  int lead_raw=0;
  
  for(i=0; i<v1190_size; i++){

    if(evt->v1190_hit_all[i].field == FIELD_PLA){
      ch = evt->v1190_hit_all[i].ch;
      //      lead_raw = evt->v1190_hit_all[i].lead_raw;
      
      // GR       
      if(ch>=0 && ch<32){
	evt->v1190pla_multi[ch]++;
	// take only the first hit
	if(evt->v1190_first_hit[PLA_V1190_GEO][ch] == 1){
	  evt->grpla.vtdc[ch] = evt->v1190_hit_all[i].lead_cor+VDC_OFFSET;
	  evt->v1190_first_hit[PLA_V1190_GEO][ch] = 0;
	}
	cnt++;
      }

      // LAS
      if(ch>=32 && ch<64){
	// take only the first hit
	if(evt->v1190_first_hit[PLA_V1190_GEO][ch] == 1){
	  evt->laspla.vtdc[ch-32] = evt->v1190_hit_all[i].lead_cor+VDC_OFFSET;
	  evt->v1190_first_hit[PLA_V1190_GEO][ch] = 0;
	}
	cnt++;
      }

      
    } // end of  if(evt->v1190_hit_all[i].field == FIELD_PLA)
        
  } // end of   for(i=0; i<v1190_size; i++)
  return cnt;
}

int anapla::GetFERAHit(evtdata *evt){
  int cnt=0;
  int vsn, ch, val;
  int fera_size = (int)(evt->fera_hit_all.size());  

  for(int i=0; i<fera_size; i++){
    vsn = evt->fera_hit_all[i].vsn;
    ch = evt->fera_hit_all[i].ch;
    val = evt->fera_hit_all[i].val;      

    //    printf("vsn=%d\n", vsn);

    // GR QDC
    if(vsn == VSN_GRQDC1){
      if(ch<N_PLA_CH){
	evt->grpla.fqdc[ch] = val;
	cnt++;
      }
    }

    // GR TDC
    if(vsn == VSN_GRTDC1){
      if(ch<N_PLA_CH){
	evt->grpla.ftdc[ch] = val;
	cnt++;
      }
    }

    // LAS QDC
    if(vsn == VSN_LASQDC1){
      if(ch<N_PLA_CH){
	evt->laspla.fqdc[ch] = val;
	cnt++;
      }
    }

    // LAS TDC
    if(vsn == VSN_LASTDC1){
      if(ch<N_PLA_CH){
	evt->laspla.ftdc[ch] = val;
	cnt++;
      }
    }
    
  } // end of   for(int i=0; i<fera_size; i++)

  return cnt;
}

