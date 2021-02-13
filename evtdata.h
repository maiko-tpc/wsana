#ifndef _EVTDATA_H_
#define _EVTDATA_H_

#include <vector>
#include "moduledata.h"

struct evtdata{
  unsigned int run;
  unsigned int blk;  
  unsigned int eve;
  madc32_data madc;
  v1190_data v1190_ssd;
  std::vector<v1190_hit> v1190_hit_all;

  grpla_data grpla;
  std::vector<grvdc_data> grvdc;
  int nhit_plane[N_VDCPLANE];
  float mean_wire[N_VDCPLANE];  

  int nclst[N_VDCPLANE];
};

#endif
