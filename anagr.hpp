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
#include <sstream>
#include <algorithm>
#include <math.h>

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
  void GetXUHits(evtdata *);
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

  int IsGoodClst(evtdata *);
  
  int  FitOnePlane(evtdata *, unsigned int planeid);
  
  std::vector<std::vector<float>> SetUpDownHit(evtdata *, std::vector<std::vector<float>> hitin, int vec_size, int ud_comb);
  
  double fit_line(std::vector<std::vector<float>> hitin, int vec_size,
		  float *fit_res);

  int calc_center_pos(evtdata *evt);
  double fit_planes(evtdata *evt);  

  void calc_rela_momentum(evtdata *evt);
  
private:
  /* Tracking parameters */
  unsigned int min_clst_size = 2;
  unsigned int max_clst_size = 10;
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

  /* Fit parameters */
  float min_chi2[N_VDCPLANE];

  float center_pos[N_VDCPLANE]; // in XY coordinate
  
  /* spectrometer parameter */
  float gr_disp = 15451.0;
  float grx_size = 1250.0;
  
};


#endif
