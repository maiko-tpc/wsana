#include "anassd.hpp"
#define V1190_SSD_GEO 24

anassd::anassd(){
}

anassd::~anassd(){
}

void anassd::V1190Hit2SSDTDC(evtdata *evt){
  unsigned int tmp_geo, tmp_ch;
  unsigned int tmp_multi[N_V1190_CH]={0};
  int hit_size = (int)(evt->v1190_hit_all.size());

  for(int i=0; i<hit_size; i++){
    tmp_geo = evt->v1190_hit_all[i].geo;

    if(tmp_geo == V1190_SSD_GEO){
      tmp_ch = evt->v1190_hit_all[i].ch;

      evt->v1190_ssd.lead[tmp_ch][tmp_multi[tmp_ch]]
	= evt->v1190_hit_all[i].lead_cor;
      evt->v1190_ssd.trail[tmp_ch][tmp_multi[tmp_ch]]
	= evt->v1190_hit_all[i].trail_raw;
      evt->v1190_ssd.tot[tmp_ch][tmp_multi[tmp_ch]]
	= evt->v1190_hit_all[i].tot;
      evt->v1190_ssd.multi[tmp_ch]++;
      evt->v1190_ssd.counter=evt->v1190_hit_all[i].counter;
    }    
  }
  
  //  V1190_SSD_GEO
}


void anassd::analyze(evtdata *evt){
  V1190Hit2SSDTDC(evt);
}
