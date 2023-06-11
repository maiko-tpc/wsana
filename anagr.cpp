#include "anagr.hpp"

using namespace std;

anagr::anagr(int gr_las){
  if(gr_las==0 || gr_las==1) i_gr_las = gr_las;
  else i_gr_las = 0;

  rnd = new TRandom3();
  rnd->SetSeed(0);
  SetTDC2LenTab_GR();
}

anagr::~anagr(){
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

void anagr::SetChamberXOffset(float offset){
}

void anagr::SetGRPars(){
  SetMaxDriftLength(10.0);

  std::vector<float> space;
  space.push_back(6.0);
  space.push_back(4.0);  
  space.push_back(6.0);
  space.push_back(4.0);  
  SetWireSpace(space);
  
  SetUPlaneAng(48.2*TMath::DegToRad());

  SetChambSpace(250.0);
}

int anagr::GetPlane(int geo, int ch){

  // GR
  if(geo<8) return (int)geo/2;  // 0:X1, 1:U1, 2:X2, 3:U2

  // LAS (as of 2019.12.04)
  if(geo== 8 || geo==9) return 0;           // X1
  if(geo==11 && ch>31 && ch< 63)  return 3; // X2
  if(geo==11 && ch>95 && ch<127)  return 0; // X1
  if(geo==12 || geo==13) return 1;          // U1
  if(geo==14 || geo==15) return 2;          // V1
  if(geo==16 || geo==17) return 3;          // X2
  if(geo==20 || geo==21) return 4;          // U2
  if(geo==22 || geo==23) return 5;          // X2

  // Default
  return 0;
}

int anagr::GetWire(int geo, int ch){
  
  // GR
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

  // LAS (as of 2019.12.04)
  if(geo== 8 && ch!=  0) return ch;                     // X1
  if(geo== 9 && ch!=127) return ch+128;                 // X1
  if(geo==11 && ch> 31 && ch< 63) return 16*15+(ch-32); // X2
  if(geo==11 && ch> 95 && ch<127) return 16*15+(ch-96); // X1
  if(geo==12 && ch!=  0) return ch;                     // U1
  if(geo==13 && ch!=127) return ch+128;                 // U1
  if(geo==14 && ch!=  0) return ch;                     // V1
  if(geo==15 && ch!=127) return ch+128;                 // V1
  if(geo==16 && ch!=  0) return ch;                     // X2
  if(geo==17 && ch!=127) return ch+128;                 // X2
  if(geo==20 && ch!=  0) return ch;                     // U2
  if(geo==21 && ch!=127) return ch+128;                 // U2
  if(geo==22 && ch!=  0) return ch;                     // V2
  if(geo==23 && ch!=127) return ch+128;                 // V2
  
  // Default or reference ch
  return -1;
}

void anagr::V1190Hit2VDCData(evtdata *evt){
  /* Get V1190 hit data to fill into grvdc */

  int tmp_field;
  int tmp_geo, tmp_ch, tmp_plane, tmp_wire;

  // Accept only the first hit in each wire
  unsigned int hit_flag[N_VDCPLANE][PLANE_SIZE]={0};
  
  int hit_size = (int)(evt->v1190_hit_all.size());
  for(int i=0; i<hit_size; i++){
    grvdc_data tmp_vdc_data;
    tmp_field = evt->v1190_hit_all[i].field;
    tmp_geo = evt->v1190_hit_all[i].geo;
    tmp_ch = evt->v1190_hit_all[i].ch;
    
    if((tmp_field==FIELD_GV_NEW || tmp_field==FIELD_GV_OLD)
       && tmp_geo<=7 && tmp_ch!=127){
      tmp_plane = GetPlane(tmp_geo, tmp_ch);
      tmp_wire = GetWire(tmp_geo, tmp_ch);
      tmp_vdc_data.plane = tmp_plane;
      tmp_vdc_data.wire = tmp_wire;
      tmp_vdc_data.geo = evt->v1190_hit_all[i].geo;
      tmp_vdc_data.lead_raw = evt->v1190_hit_all[i].lead_raw;

      tmp_vdc_data.lead_cor = evt->v1190_hit_all[i].lead_cor + VDC_OFFSET; // use v1190 reference sbtracted

      //      tmp_vdc_data.lead_cor = evt->v1190_hit_all[i].lead_cor -
      //      	(int)((evt->grpla.vtdc[0]+evt->grpla.vtdc[1])/2.0) + VDC_OFFSET2; // before e552 run40990

      //tmp_vdc_data.lead_cor = evt->v1190_hit_all[i].lead_cor - evt->grpla.vtdc[15] + VDC_OFFSET2; //from e552 run4090

      tmp_vdc_data.clst_flag = 0;                  
      if(tmp_vdc_data.plane<4 &&
	 tmp_vdc_data.wire>=0 && tmp_vdc_data.wire<1000 &&
	 hit_flag[tmp_plane][tmp_wire]==0){
	evt->grvdc.push_back(tmp_vdc_data);
	hit_flag[tmp_plane][tmp_wire]=1;
      }
    }
    
  }
}

void anagr::GetXUHits(evtdata *evt){
  int nhits = (int)evt->grvdc.size();
  for(int i=0; i<nhits; i++){
    if((evt->grvdc[i].plane)%2==0){  // X plane
      evt->grvdc_x.push_back(evt->grvdc[i]);
    }
    if((evt->grvdc[i].plane)%2==1){  // U plane
      evt->grvdc_u.push_back(evt->grvdc[i]);
    }
  }

}

void anagr::anavdc(evtdata *evt){
  int i;
  
  V1190Hit2VDCData(evt);
  GetXUHits(evt);
  
  int grvdc_size = (int)(evt->grvdc.size());
  for(i=0; i<grvdc_size; i++){
    int plane=evt->grvdc[i].plane;
    int wire=evt->grvdc[i].wire;
    //    int drifttime=evt->grvdc[i].lead_cor;
    if(wire>=0){
      evt->nhit_plane[plane]++;
      evt->mean_wire[plane]+=wire;
    }
  }
  
  for(i=0; i<N_VDCPLANE; i++){
    if(evt->nhit_plane[i]>0){
      evt->mean_wire[i] = evt->mean_wire[i]/evt->nhit_plane[i];
    }
  }
    
  TDC2Len_GR(evt);
  cal_nclst(evt);
  evt->gr_good_hit=IsGoodHit(evt);
  evt->gr_good_clst=IsGoodClst(evt);

  if(evt->gr_good_clst==1){
    for(i=0; i<N_VDCPLANE; i++){
      FitOnePlane(evt, i);
    }
  }
  
  if(evt->gr_good_clst==0) evt->good_fit=0;
  for(int i=0; i<N_VDCPLANE; i++){
    if(evt->redchi2[i]>100) evt->good_fit=0;
  }
  
  // get position at focal plane & angle
  calc_center_pos(evt);
  fit_planes(evt);  

  // calc the momentum
  calc_rela_momentum(evt);
}

void anagr::TDC2Len_GR(evtdata *evt){
  int ar_index;
  int hit_size = (int)evt->grvdc.size();
  int plane;
  float dlen;
  float wid;
  for(int i=0; i<hit_size; i++){
    plane=evt->grvdc[i].plane;
    ar_index = (int)(evt->grvdc[i].lead_cor/tdc_bin_wid);
    if(ar_index<0) ar_index=0;
    if(ar_index>=(MAX_VDC_TDC-2)) ar_index=MAX_VDC_TDC-2;
    dlen=tdc2len_tab[plane][ar_index];

    // smear with random
    wid=tdc2len_tab[plane][ar_index+1]-tdc2len_tab[plane][ar_index];
    dlen+=wid*rnd->Rndm();
    
    evt->grvdc[i].dlen = dlen;
  }
}

void anagr::cal_nclst(evtdata *evt){

  // Make the hit-table arrays
  unsigned int hit_array[N_VDCPLANE][PLANE_SIZE]={0}; // Initilize with zero
  size_t grvdc_size = evt->grvdc.size();
  for(int i=0; i<(int)grvdc_size; i++){
    if(evt->grvdc[i].wire >=0){
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
	  evt->clst_size[iplane]=tmp_cnt;	  
	  for(int j=start_wire; j<=stop_wire; j++){
	    clst_flag[iplane][j]=evt->nclst[iplane];
	  }
	}
	tmp_cnt=0;
      }

    } // end of PLANE_SIZE loop


  } // end of N_VDCPLANE loop

  for(int i=0; i<(int)grvdc_size; i++){
    if(evt->grvdc[i].wire>0 && evt->grvdc[i].wire<PLANE_SIZE){
      evt->grvdc[i].clst_flag = clst_flag[evt->grvdc[i].plane][evt->grvdc[i].wire];
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
    ifs[i].open(infname[i].c_str());

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

int anagr::IsGoodHit(evtdata *evt){
  int result=1;
  for(int i=0; i<N_VDCPLANE; i++){
    if(evt->nhit_plane[i]==0) result=0;
  }
  return result;
}

int anagr::IsGoodClst(evtdata *evt){
  int result=1;
  for(int i=0; i<N_VDCPLANE; i++){
    if(evt->nclst[i]!=1) result=0;
  }
  return result;
}


int anagr::FitOnePlane(evtdata *evt, unsigned int planeid){

  std::vector<float> hit_x;
  std::vector<float> hit_y;  
  
  hit_x.clear();
  hit_y.clear();  

  float tmp_x, tmp_y;
  float tmp_res[4], best_res[4];

  // initialize
  for(int i=0; i<4; i++){
    tmp_res[i] = 1;
    best_res[i] = 1;
  }
  
  // put target plane hits to vectors
  int vec_size = (int)evt->grvdc.size();

  int hit_cnt=0;
  for(int i=0; i<vec_size; i++){
    if(evt->grvdc[i].plane == planeid && evt->grvdc[i].clst_flag==1){
      tmp_x = evt->grvdc[i].wire*wire_space[planeid];
      tmp_y = evt->grvdc[i].dlen;
      hit_x.push_back(tmp_x);
      hit_y.push_back(tmp_y);      
      hit_cnt++;
    }
  }
  
  std::vector<std::vector<float>> hit(hit_cnt, std::vector<float>(2));

    for(int i=0; i<hit_cnt; i++){
      hit[i][0] = hit_x[i];
      hit[i][1] = hit_y[i];      
    }
    
    // sort the vector by the X position
    sort(hit.begin(), hit.end(),
	 [](const vector<float> &alpha,const vector<float> &beta){return alpha[0] < beta[0];});

    int max_comb = pow(2, hit_cnt);
    std::vector<std::vector<float>> tmphit(vec_size, std::vector<float>(2));
    

    float tmp_chi2;
    
    for(int i=0; i<max_comb; i++){

      // ignore if the first hit position will become negative

      if( (i & 0x01) != 0x1){
	tmphit = SetUpDownHit(evt, hit, hit_cnt, i);
	tmp_chi2=fit_line(tmphit, hit_cnt, tmp_res);
	if(i==0) min_chi2[planeid] = tmp_chi2;
	if(tmp_chi2 < min_chi2[planeid]){
	  min_chi2[planeid] = tmp_chi2;
	  evt->redchi2[planeid] = tmp_chi2;
	  best_res[0] = tmp_res[0]; // offset of the line
	  best_res[1] = tmp_res[1]; // slope of the line
	  best_res[2] = tmp_res[2]; // chi2
	  best_res[3] = tmp_res[3]; // residual
	}
      }
    } // end of     for(int i=0; i<max_comb; i++){

    // calculate the center position
    evt->wire_pos[planeid] = -1.0*best_res[0]/best_res[1];
    evt->residual[planeid] = best_res[3];
    
    return 0;
}

std::vector<std::vector<float>> anagr::SetUpDownHit(evtdata *evt, std::vector<std::vector<float>> hitin, int vec_size, int ud_comb){

  
  std::vector<std::vector<float>> tmphit(vec_size, std::vector<float>(2));

  for(int i=0; i<vec_size; i++){
    tmphit[i][0] = hitin[i][0];
    tmphit[i][1] = hitin[i][1];    

    if((ud_comb>>i & 0x1) == 0x1){
      tmphit[i][1] = -1.0*hitin[i][1];
    }
  }
  return tmphit;
}

double anagr::fit_line(std::vector<std::vector<float>> hitin, int vec_size,
		       float *fit_res){
  
  double tmp_fit_par[2]; // [0]:offset, [1]:slope
  double Sx=0;
  double Sy=0;
  double Sxx=0;
  double Sxy=0;      

  for(int i=0; i<vec_size; i++){
    Sx+=hitin[i][0];
    Sy+=hitin[i][1];
    Sxx+=hitin[i][0]*hitin[i][0];
    Sxy+=hitin[i][0]*hitin[i][1];    
  }

  // calculate fit parameters
  tmp_fit_par[0] = (Sxx*Sy-Sx*Sxy)/(vec_size*Sxx-Sx*Sx);
  tmp_fit_par[1] = (vec_size*Sxy-Sx*Sy)/(vec_size*Sxx-Sx*Sx);  
  
  // calculate chi2 and mean residual with the fitted line
  double chi2=0;
  float residual=0;
  for(int i=0; i<vec_size; i++){
    chi2+=pow(hitin[i][1]-
	      (tmp_fit_par[0]+tmp_fit_par[1]*hitin[i][0]), 2);
    residual+=hitin[i][1]-(tmp_fit_par[0]+tmp_fit_par[1]*hitin[i][0]);
  }

  fit_res[0] = tmp_fit_par[0];
  fit_res[1] = tmp_fit_par[1];  
  fit_res[2] = chi2/((float)vec_size);
  fit_res[3] = residual/((float)vec_size);
  
  return chi2/((float)vec_size);
}

int anagr::calc_center_pos(evtdata *evt){
  center_pos[0] = evt->wire_pos[0]; // X1
  center_pos[2] = evt->wire_pos[2]; // X2

  center_pos[1] = -1.0/tan(u_plane_ang)*center_pos[0]
    + evt->wire_pos[1]/sin(u_plane_ang); // U1
  center_pos[3] = -1.0/tan(u_plane_ang)*center_pos[2]
    + evt->wire_pos[3]/sin(u_plane_ang); // U2
  
  return 0;
}

double anagr::fit_planes(evtdata *evt){
  //  evt->grx = (center_pos[0] + center_pos[2])/2.0;
  //  evt->gry = (center_pos[1] + center_pos[3])/2.0;  

  // 2023.03.21  focal plane is at the exit window of GR
  //  which is close to the X1 plane
  evt->grx = center_pos[0] - grx_size/2.0;
  evt->gry = center_pos[1] - gry_size/2.0;  
  
  evt->grthx = atan((center_pos[2]+chamb_space - center_pos[0])/chamb_space)*TMath::RadToDeg()-45.0;  
  evt->grthx2 = atan((center_pos[2]-center_pos[0])/chamb_space)*TMath::RadToDeg();
  evt->grthy = atan((center_pos[3]-center_pos[1])/chamb_space)*TMath::RadToDeg();

  return 0;
}


void anagr::calc_rela_momentum(evtdata *evt){
  evt->grp_rela = evt->grx/(gr_disp*sqrt(2.0))*100.0;
}
