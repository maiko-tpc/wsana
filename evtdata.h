#ifndef EVTDATA_H_
#define EVTDATA_H_

#include <vector>
#include "moduledata.h"

struct optdata{
  char bldfname[256];
  char rootfname[256];  
  int online_flag;
  int useage_flag;
};

struct evtdata{
  unsigned int run;
  unsigned int blk;  
  unsigned int eve;
  madc32_data madc;
  mqdc32_data mqdc;  
  std::vector<mxdc32_hit> mxdc32_hit_all;

  v1190_data v1190_ssd;
  std::vector<v1190_hit> v1190_hit_all;

  std::vector<fera_hit> fera_hit_all;
  
  pla_data grpla;
  pla_data laspla;  

  std::vector<grvdc_data> grvdc;
  std::vector<grvdc_data> grvdc_x;
  std::vector<grvdc_data> grvdc_u;  
  int nhit_plane[N_VDCPLANE];
  float mean_wire[N_VDCPLANE];  

  int nclst[N_VDCPLANE];
  int clst_size[N_VDCPLANE];

  int gr_good_clst;

  float redchi2[N_VDCPLANE];
  float wire_pos[N_VDCPLANE];
  float residual[N_VDCPLANE];  

  float grx, gry;
  float grthx, grthy;  

  int good_fit;

  float grp_rela;

  unsigned int camac_inp[N_INP];
  unsigned int vme_inp[N_INP];  
};

#endif
