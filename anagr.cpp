#include "anagr.hpp"

const int VDC_OFFSET = 4000;

anagr::anagr(){
  rnd = new TRandom3();
  rnd->SetSeed(0);
}

anagr::~anagr(){
}

void anagr::anapla(evtdata *evt){
  evt->grpla.pos[0] = evt->grpla.tdc[0] - evt->grpla.tdc[1];
  evt->grpla.pos[1] = evt->grpla.tdc[2] - evt->grpla.tdc[3];
  
  evt->grpla.de[0]=sqrt(evt->grpla.adc[0] * evt->grpla.adc[1]);
  evt->grpla.de[1]=sqrt(evt->grpla.adc[2] * evt->grpla.adc[3]); 
}

/* VDC parameters setting */
void anagr::SetMaxDriftLength(float length){
  max_drift_length = length;
}

void anagr::SetWireSpace(std::vector<float> space){
  size_t vec_size = space.size();
  for(int i=0; i<(int)vec_size; i++){
    wire_space.push_back(space[i]);
  }
}

void anagr::SetUPlaneAng(float ang){
  u_plane_ang = ang;
}

void anagr::SetChambSpace(float space){
  chamb_space = space;
}

void anagr::SetGRPars(){
  SetMaxDriftLength(10.0);

  std::vector<float> space;
  space.push_back(6.0);
  space.push_back(4.0);  
  space.push_back(6.0);
  space.push_back(4.0);  
  SetWireSpace(space);
  
  SetUPlaneAng(-48.2*TMath::DegToRad());

  SetChambSpace(250.0);
}

int anagr::GetPlane(int geo){
  return (int)geo/2;
}

int anagr::GetWire(int geo, int ch){
  if(geo==0 || geo==4){
    if(ch>=  0 && ch<= 15) return ch;
    if(ch>= 16 && ch<= 31) return ch+16;
    if(ch>= 32 && ch<= 47) return ch-16;
    if(ch>= 48 && ch<= 63) return ch;
    if(ch>= 64 && ch<= 79) return ch;
    if(ch>= 80 && ch<= 95) return ch+16;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==1 || geo==5){
    if(ch>=  0 && ch<= 15) return ch+80;
    if(ch>= 16 && ch<= 31) return ch+96;
    if(ch>= 32 && ch<= 47) return ch+96;
    if(ch>= 48 && ch<= 63) return ch+112;
    if(ch>= 64 && ch<= 79) return ch+80;
    if(ch>= 80 && ch<= 95) return ch+96;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==2 || geo==6){
    if(ch>=  0 && ch<= 15) return ch+16;
    if(ch>= 16 && ch<= 31) return ch+32;
    if(ch>= 32 && ch<= 47) return ch;
    if(ch>= 48 && ch<= 63) return ch+16;
    if(ch>= 64 && ch<= 79) return ch+16;
    if(ch>= 80 && ch<= 95) return ch+32;
    if(ch>= 96 && ch<=111) return ch-96;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==3 || geo==7){
    if(ch>=  0 && ch<= 15) return ch+96;
    if(ch>= 16 && ch<= 31) return ch+112;
    if(ch>= 32 && ch<= 47) return ch+112;
    if(ch>= 48 && ch<= 63) return ch+128;
    if(ch>= 64 && ch<= 79) return ch+96;
    if(ch>= 80 && ch<= 95) return ch+112;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  return -1;
}

void anagr::V1190Hit2VDCData(evtdata *evt){
  /* Get V1190 hit data to fill into grvdc */

  int tmp_field;
  int tmp_geo, tmp_ch;
  
  int hit_size = (int)(evt->v1190_hit_all.size());
  for(int i=0; i<hit_size; i++){
    grvdc_data tmp_vdc_data;
    tmp_field = evt->v1190_hit_all[i].field;
    tmp_geo = evt->v1190_hit_all[i].geo;
    tmp_ch = evt->v1190_hit_all[i].ch;
    if(evt->eve==5) printf("geo=%d\n", tmp_geo);
    
    if(tmp_field==0 && tmp_geo<=7 && tmp_ch!=127){
      tmp_vdc_data.plane = GetPlane(tmp_geo);
      tmp_vdc_data.wire = GetWire(tmp_geo, tmp_ch);
      tmp_vdc_data.geo = evt->v1190_hit_all[i].geo;
      tmp_vdc_data.lead_raw = evt->v1190_hit_all[i].lead_raw;
      tmp_vdc_data.lead_cor = evt->v1190_hit_all[i].lead_cor + VDC_OFFSET;
      tmp_vdc_data.clst_flag = 0;                  
      if(tmp_vdc_data.plane<4 &&
	 tmp_vdc_data.wire>0 && tmp_vdc_data.wire<1000){
	evt->grvdc.push_back(tmp_vdc_data);
      }
    }
    
  }
}

void anagr::anavdc(evtdata *evt){
  int i;

  for(i=0; i<N_VDCPLANE; i++){
    evt->nhit_plane[i]=0;
    evt->mean_wire[i]=0.0;
  }
  
  V1190Hit2VDCData(evt);
  
  int grvdc_size = (int)(evt->grvdc.size());
  for(i=0; i<grvdc_size; i++){
    int plane=evt->grvdc[i].plane;
    int wire=evt->grvdc[i].wire;
    //    int drifttime=evt->grvdc[i].lead_cor;
    if(wire>0){
      evt->nhit_plane[plane]++;
      evt->mean_wire[plane]+=wire;
    }
  }
  
  for(i=0; i<N_VDCPLANE; i++){
    if(evt->nhit_plane[i]>0){
      evt->mean_wire[i] = evt->mean_wire[i]/evt->nhit_plane[i];
    }
  }
  
  cal_nclst(evt);
}

void anagr::TDC2Len_GR(evtdata *evt){
  int ar_index;
  int hit_size = (int)evt->grvdc.size();
  for(int i=0; i<hit_size; i++){

  }
}

void anagr::cal_nclst(evtdata *evt){

  // Make the hit-table arrays
  unsigned int hit_array[N_VDCPLANE][PLANE_SIZE]={0}; // Initilize with zero
  size_t grvdc_size = evt->grvdc.size();
  for(int i=0; i<(int)grvdc_size; i++){
    if(evt->grvdc[i].wire >0){
      if(evt->eve==5)printf("plane=%d, wire=%d\n", evt->grvdc[i].plane,evt->grvdc[i].wire);
      hit_array[evt->grvdc[i].plane][evt->grvdc[i].wire]++;
    }
  }
  
  // Analyze the hit-table to get hit-cluster
  unsigned int tmp_cnt;
  unsigned int zero_cnt;
  unsigned int clst_flag[N_VDCPLANE][PLANE_SIZE]={0};
  
  for(int iplane=0; iplane<N_VDCPLANE; iplane++){
    int start_wire=0;
    int stop_wire=0;
    tmp_cnt=0;
    zero_cnt=0;    
    for(int iwire=0; iwire<PLANE_SIZE; iwire++){
      if(hit_array[iplane][iwire] >0){
	if(tmp_cnt==0) start_wire=iwire;
	if(iwire==N_VDCPLANE-1) stop_wire=iwire;
	tmp_cnt++;
	zero_cnt=0;
      }
      if(hit_array[iplane][iwire]==0){
	if(zero_cnt==0) stop_wire=iwire-1;
	zero_cnt++;
      }
      if(zero_cnt>max_clst_zero || iwire==PLANE_SIZE-1){
	if(tmp_cnt>=min_clst_size && tmp_cnt<=max_clst_size){
	  evt->nclst[iplane]++;
	  for(int j=start_wire; j<=stop_wire; j++){
	    clst_flag[iplane][j]=evt->nclst[iplane];
	  }
	}
	tmp_cnt=0;
      }

    } // end of PLANE_SIZE loop

    if(evt->eve==5 && iplane==1){
      for(int n=0; n<PLANE_SIZE; n++){
	printf("wire=%d: %d, %d\n",
	       n, hit_array[iplane][n], clst_flag[iplane][n]);
      }
    }

  } // end of N_VDCPLANE loop

  for(int i=0; i<(int)grvdc_size; i++){
    if(evt->grvdc[i].wire>0 && evt->grvdc[i].wire<PLANE_SIZE){
      evt->grvdc[i].clst_flag = clst_flag[evt->grvdc[i].plane][evt->grvdc[i].wire];
      if(evt->eve==5) printf("plane=%d, wire=%d, clst=%d\n",
			     evt->grvdc[i].plane, evt->grvdc[i].wire,
			     evt->grvdc[i].clst_flag);
    }
  }
}


void anagr::SetTDC2LenTab_GR(){
  std::string infname[N_VDCPLANE];
  infname[0]="calib/vdccalib_x1.dat";
  infname[1]="calib/vdccalib_u1.dat";
  infname[2]="calib/vdccalib_x2.dat";
  infname[3]="calib/vdccalib_u2.dat";  

  int tdc[MAX_VDC_TDC];
  
  ifstream ifs[N_VDCPLANE];
  int i,j;
  std::string str;

  int tmp_tdc;
  float tmp_len;
  
  for(i=0; i<N_VDCPLANE; i++){
    ifs[i].open(infname[i]);

    if(!ifs[i]){
      printf("Error in opening VDC calib table: %s\n", infname[i].c_str());
      for(j=0; j<MAX_VDC_TDC; j++){
	tdc2len_tab[i][j] = max_drift_length/2.0;
      }
    }
    
    else{
      j=0;
      while(getline(ifs[i], str)){
	std::stringstream stream;
	stream << str;
	stream >> tmp_tdc >> tmp_len;
	tdc[j]=tmp_tdc;
	tdc2len_tab[i][j]=tmp_len;
	j++;
	if(j>=MAX_VDC_TDC){
	  ifs[i].close();
	  break;
	}
      }
    }
  }
  
  tdc_bin_wid = tdc[1]-tdc[0];
  if(tdc_bin_wid<=0) tdc_bin_wid = 1;
}
