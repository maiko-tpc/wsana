#ifndef EVTDATA_H_
#define EVTDATA_H_

#include <vector>
#include "moduledata.h"

struct evtdata{
  unsigned int run;
  unsigned int blk;  
  unsigned int eve;
  madc32_data madc;
  mqdc32_data mqdc;  
  std::vector<mxdc32_hit> mxdc32_hit_all;

  v1190_data v1190_ssd;
  std::vector<v1190_hit> v1190_hit_all;

  grpla_data grpla;
  std::vector<grvdc_data> grvdc;
  std::vector<grvdc_data> grvdc_x;
  std::vector<grvdc_data> grvdc_u;  
  int nhit_plane[N_VDCPLANE];
  float mean_wire[N_VDCPLANE];  

  int nclst[N_VDCPLANE];
};

#endif
