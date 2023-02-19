#include "anassd.hpp"

#define V1190_SSD_GEO 24
#define RF_SSD_CH 112

anassd::anassd(){
  for(int i=0; i<SI_CH; i++){
    ene_calib_par[i][0]=0;
    ene_calib_par[i][1]=1;    
  }

  // open energy calibration parameter file
  std::ifstream calib_file;
  string calib_fname = "/home/tamidaq/ana/furuno/calib/par/ssdcalib.dat";
  calib_file.open(calib_fname.c_str(), std::ios::in);
  if(!calib_file){
    printf("Cannot open the SSD calibration file: %s\n",
	   calib_fname.c_str());
  }

  // Read the calibration parameters
  int tmp_ch;
  if(calib_file){
    printf("Reading SSD calibration file: %s\n",
	   calib_fname.c_str());
    while(!calib_file.eof()){
      calib_file >> tmp_ch;
      if(tmp_ch>=0 && tmp_ch<SI_CH){
	calib_file >> ene_calib_par[tmp_ch][0];
	calib_file >> ene_calib_par[tmp_ch][1];	
      }
    }
  }

  // Set SAKRA position for hit histograms
  sakra_r_wid = (sakra_rmax-sakra_rmin)/16.0;
  sakra_theta_wid = sakra_ang*2/8.0;  

  for(int i=0; i<16; i++){
    sakra_r_cen[i] = sakra_rmin+(i+0.5)*sakra_r_wid;
  }
  for(int i=0; i<8; i++){
    sakra_theta_cen[i] = (-1.0*sakra_ang+(i+0.5)*sakra_theta_wid)*
      TMath::DegToRad();
  }

}

anassd::~anassd(){
}

void anassd::V1190Hit2SSDTDC(evtdata *evt){
  unsigned int tmp_field;
  unsigned int tmp_geo, tmp_ch;
  int hit_size = (int)(evt->v1190_hit_all.size());

  for(int i=0; i<hit_size; i++){
    tmp_field = evt->v1190_hit_all[i].field;
    tmp_geo = evt->v1190_hit_all[i].geo;
    tmp_ch = evt->v1190_hit_all[i].ch;
    
    if(tmp_geo==V1190_SSD_GEO && tmp_field==FIELD_SSD){
      evt->v1190_ssd.hit[tmp_ch] = 1;
      evt->v1190_ssd.multi[tmp_ch]++;
      evt->v1190_ssd.tdc_raw[tmp_ch]
	=evt->v1190_hit_all[i].lead_raw;
      evt->v1190_ssd.tdc_cor[tmp_ch]
	=evt->v1190_hit_all[i].lead_cor;
    }    

    if(tmp_field==FIELD_PLA && tmp_ch>=96 && tmp_ch<=98){
      evt->v1190_ssd.hit[tmp_ch] = 1;
      evt->v1190_ssd.multi[tmp_ch]++;
      evt->v1190_ssd.tdc_raw[tmp_ch]
	=evt->v1190_hit_all[i].lead_raw;
      evt->v1190_ssd.tdc_cor[tmp_ch]
	=evt->v1190_hit_all[i].lead_cor;
    }
  }
  
  //  V1190_SSD_GEO
}

void anassd::Mxdc32Hit2SSDADC(evtdata *evt){
  unsigned int tmp_geo, tmp_ch, tmp_adc;
  //  unsigned int tmp_field;
  unsigned int ch;

  unsigned int cnt[N_MADC];
  
  int hit_size = (int)(evt->mxdc32_hit_all.size());

  for(int i=0; i<hit_size; i++){
    tmp_geo = evt->mxdc32_hit_all[i].geo;
    //    tmp_field = evt->mxdc32_hit_all[i].field;
    if(tmp_geo>=MADC_START_GEO && tmp_geo<MADC_START_GEO+N_MADC){
    //    if(tmp_field==FIELD_SSD){
      tmp_ch = evt->mxdc32_hit_all[i].ch;
      tmp_adc = evt->mxdc32_hit_all[i].adc;      
      
      ch = 32*(tmp_geo-MADC_START_GEO) + tmp_ch;
      if(ch<N_MADC_CH) evt->madc.adc[ch] = tmp_adc;
      cnt[tmp_geo-MADC_START_GEO]++;
    }
  }

  for(int i=0; i<N_MADC; i++){
    evt->madc.wrdcnt[i] = cnt[i];
    evt->madc.counter[i] = evt->eve;
  }
}

void anassd::ana_rf(evtdata *evt){
  unsigned int tmp_field;
  unsigned int tmp_geo, tmp_ch;
  int hit_size = (int)(evt->v1190_hit_all.size());

  int rf_hit_cnt=0;
  
  for(int i=0; i<hit_size; i++){
    tmp_field = evt->v1190_hit_all[i].field;
    tmp_geo = evt->v1190_hit_all[i].geo;
    tmp_ch = evt->v1190_hit_all[i].ch;
      
    if(tmp_geo==V1190_SSD_GEO && tmp_field==FIELD_SSD){
      if(tmp_ch == RF_SSD_CH && rf_hit_cnt<MAX_RF_MULTI){
	evt->rf_ssd[rf_hit_cnt] = evt->v1190_hit_all[i].lead_cor;
	rf_hit_cnt++;
      }
    }
  }

}

void anassd::ene_calib(evtdata *evt){
  for(int i=0; i<SI_CH; i++){
    evt->ssd_ene[i] = ene_calib_par[i][0] +
      evt->madc.adc[i]*ene_calib_par[i][1];
  }
}

void anassd::ana_clst(evtdata *evt){
  int clst_start_ch[10]={0,16,32, 48,56,64, 72,80,88, 96};

  float tmp_max_ene=0;
  float tmp_tdc=0;
  int tmp_ch=0;
  
  for(int i=0; i<N_SSD_CLST; i++){
    tmp_max_ene=0;
    tmp_ch=0;
    for(int ch=clst_start_ch[i]; ch<clst_start_ch[i+1]; ch++){
      if(evt->v1190_ssd.hit[ch]){
	evt->ssd_clst_multi[i]++;
	evt->ssd_clst_hitch[i]=tmp_ch;
      }
      if(evt->ssd_ene[ch] > tmp_max_ene){
	tmp_max_ene = evt->ssd_ene[ch];
	tmp_tdc = evt->v1190_ssd.tdc_cor[ch];		
      }
      tmp_ch++;
    }   // end of ch loop
    
    evt->ssd_clst_ene[i] = tmp_max_ene;
    evt->ssd_clst_tdc[i] = tmp_tdc;    
  }  // end of N_SSD_CLST loop

  // layer 1
  if(evt->ssd_clst_multi[0]==1 && evt->ssd_clst_multi[3]==1){
    evt->sakra_good_hit[0]=1;
    evt->sakra_r[0]=sakra_r_cen[evt->ssd_clst_hitch[0]];
    evt->sakra_theta[0]=sakra_theta_cen[evt->ssd_clst_hitch[3]];    
  }
  if(evt->ssd_clst_multi[1]==1 && evt->ssd_clst_multi[5]==1){
    evt->sakra_good_hit[1]=1;
    evt->sakra_r[1]=sakra_r_cen[evt->ssd_clst_hitch[1]];
    evt->sakra_theta[1]=sakra_theta_cen[evt->ssd_clst_hitch[5]];    
  }
  if(evt->ssd_clst_multi[2]==1 && evt->ssd_clst_multi[7]==1){
    evt->sakra_good_hit[2]=1;
    evt->sakra_r[2]=sakra_r_cen[evt->ssd_clst_hitch[2]];
    evt->sakra_theta[2]=sakra_theta_cen[evt->ssd_clst_hitch[7]];    
  }

  
  // layer 2
  if(evt->ssd_clst_multi[4]==1){
    evt->sakra_good_hit[3]=1;
    evt->sakra_r[3]=sakra_r_cen[0];
    evt->sakra_theta[3]=sakra_theta_cen[evt->ssd_clst_hitch[4]];    
  }
  if(evt->ssd_clst_multi[6]==1){
    evt->sakra_good_hit[4]=1;
    evt->sakra_r[4]=sakra_r_cen[0];
    evt->sakra_theta[4]=sakra_theta_cen[evt->ssd_clst_hitch[6]];    
  }
  if(evt->ssd_clst_multi[8]==1){
    evt->sakra_good_hit[5]=1;    
    evt->sakra_r[5]=sakra_r_cen[0];
    evt->sakra_theta[5]=sakra_theta_cen[evt->ssd_clst_hitch[8]];    
  }
}

void anassd::analyze(evtdata *evt){
  Mxdc32Hit2SSDADC(evt);
  V1190Hit2SSDTDC(evt);
  ana_rf(evt);
  ene_calib(evt);
  ana_clst(evt);
}

void anassd::init_data(evtdata *evt){
  for(int i=0; i<SSD_TDC_CH; i++){
    evt->v1190_ssd.hit[i]=0;
    evt->v1190_ssd.multi[i]=0;
    evt->v1190_ssd.tdc_raw[i]=0;
    evt->v1190_ssd.tdc_cor[i]=0;        
  }

  for(int i=0; i<MAX_RF_MULTI; i++){
    evt->rf_ssd[i]=0;
  }

  for(int i=0; i<N_SSD_CLST; i++){
    evt->ssd_clst_multi[i]=0;
    evt->ssd_clst_ene[i]=0;
    evt->ssd_clst_tdc[i]=0;    
    evt->ssd_clst_hitch[i]=0;
  }
  
  for(int i=0; i<N_SAKRA; i++){
    evt->sakra_good_hit[i]=0;
    evt->sakra_r[i]=0;
    evt->sakra_theta[i]=0;    
  }
}
