#ifndef EVTDATA_H_
#define EVTDATA_H_

#include <vector>
#include "moduledata.h"

struct optdata{
  char bldfname[256];
  char rootfname[256];  
  int online_flag;
  int last_flag;
  int refresh_flag;    
  int web_flag;  
  int useage_flag;
  char parfname[256];
};

struct pardata{
  char target_nucl[256];
  char beam_nucl[256];
  char scat_nucl[256];
  char recoil_nucl[256];
  double beam_ene;
  double gr_ang;
  double gr_mag;  
  double gr_ang_gate_min;
  double gr_ang_gate_max;  
};

struct evtdata{
  unsigned int run;
  unsigned int blk;  
  unsigned int eve;
  madc32_data madc;
  mqdc32_data mqdc;  
  std::vector<mxdc32_hit> mxdc32_hit_all;

  v1190ssd_data v1190_ssd;
  std::vector<v1190_hit> v1190_hit_all;

  int v1190_hit_ref[V1190_MAX_GEO];
  
  std::vector<fera_hit> fera_hit_all;
  
  pla_data grpla;
  pla_data laspla;  

  std::vector<vdc_data> grvdc;
  std::vector<vdc_data> grvdc_x;
  std::vector<vdc_data> grvdc_u;  

  std::vector<vdc_data> lasvdc;
  std::vector<vdc_data> lasvdc_x;
  std::vector<vdc_data> lasvdc_u;
  std::vector<vdc_data> lasvdc_v;    

  int nhit_plane[N_VDCPLANE];
  float mean_wire[N_VDCPLANE];  

  int nhit_plane_las[N_VDCPLANE_LAS];
  float mean_wire_las[N_VDCPLANE_LAS];  
  
  int nclst[N_VDCPLANE];
  int clst_size[N_VDCPLANE];

  int gr_good_hit;
  int gr_good_clst;

  float gr_hit_eff[N_VDCPLANE];
  float gr_clst_eff[N_VDCPLANE];  
  float gr_hit_eff_all;
  float gr_clst_eff_all;  

  float las_hit_eff[N_VDCPLANE_LAS];
  float las_clst_eff[N_VDCPLANE_LAS];  
  float las_hit_eff_all;
  float las_clst_eff_all;  
  
  float gr_tot_mean[N_VDCPLANE];
  int gr_tot_max[N_VDCPLANE];
  float las_tot_mean[N_VDCPLANE_LAS];
  int las_tot_max[N_VDCPLANE_LAS];
  
  float redchi2[N_VDCPLANE];
  float wire_pos[N_VDCPLANE];
  float residual[N_VDCPLANE];  

  float grx, gry;
  float grthx, grthy;

  int good_fit;

  double grp_rela;
  double grp;
  double grtote, gre;
  
  unsigned int camac_inp[N_INP];
  unsigned int vme_inp[N_INP];
  unsigned int first_camac_inp;

  unsigned int unixtime=0;
  unsigned int unixtimesub=0;
  unsigned int unixtimefirst=0;
  unsigned int tmpunixtime=0;      

  unsigned int camac_sca_flag;
  unsigned long int camac_sca[CAMAC_SCA_CH];

  unsigned long int vme_sca[VME_SCA_CH];

  unsigned int v1190_first_hit[V1190_MAX_GEO][N_V1190_CH];
  unsigned int v1190pla_multi[16];

  int rf_ssd[MAX_RF_MULTI];
  float ssd_ene[96];

  int ssd_clst_multi[N_SSD_CLST];
  float ssd_clst_ene[N_SSD_CLST];
  int ssd_clst_tdc[N_SSD_CLST];
  int ssd_clst_hitch[N_SSD_CLST];    

  int sakra_good_hit[N_SAKRA];
  float sakra_r[N_SAKRA];
  float sakra_theta[N_SAKRA];    

  unsigned int mqdc_nhit[2]; //[0]:GR, [1]:LAS

  int rf[N_RF];
  
  
};

#endif
