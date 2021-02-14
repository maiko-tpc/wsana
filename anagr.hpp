#ifndef ANAGR_HPP_
#define ANAGR_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <signal.h>
#include <string.h>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TMath.h>
#include <TRandom3.h>

#include "mtformat.h"
#include "moduledata.h"
#include "evtdata.h"
#include "decoder.hpp"

class anagr{
public:
  anagr();
  ~anagr();

  void anavdc(evtdata *);

  void V1190Hit2VDCData(evtdata *);
  int GetPlane(int geo);
  int GetWire(int geo, int ch);
  
  void TDC2Len_GR(evtdata *);
  void cal_nclst(evtdata *);

  void SetMaxDriftLength(float length);
  void SetWireSpace(std::vector<float> space);  
  void SetUPlaneAng(float ang);
  void SetChambSpace(float space);  

  void SetGRPars();
  void SetTDC2LenTab_GR();
  
private:
  /* Tracking parameters */
  unsigned int min_clst_size = 2;
  unsigned int max_clst_size = 20;
  unsigned int max_clst_zero = 1;

  /* VDC paramters */
  float max_drift_length;
  std::vector<float> wire_space;
  float u_plane_ang;
  float chamb_space;

  /* TDC to drift length table */
  unsigned int tdc_bin_wid=1;
  float tdc2len_tab[N_VDCPLANE][MAX_VDC_TDC];
  TRandom3 *rnd;
  
};


#endif
