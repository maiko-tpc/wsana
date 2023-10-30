#ifndef ANASSD_HPP_
#define ANASSD_HPP_

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

#include "mtformat.h"
#include "moduledata.h"
#include "evtdata.h"
#include "decoder.hpp"

#define SI_CH 192

class anassd{
private:
  double ene_calib_par[SI_CH][2];

  int tdc_offset[256];  

  double sakra_r_wid;
  double sakra_theta_wid;  
  double sakra_r_cen[16];
  double sakra_theta_cen[8];      
  
public:
  anassd();
  ~anassd();

  void init_data(evtdata *);
  void V1190Hit2SSDTDC(evtdata *);
  void Mxdc32Hit2SSDADC(evtdata *);
  void ana_rf(evtdata *);
  void ene_calib(evtdata *);
  void ana_clst(evtdata *);
  void analyze(evtdata *);
};

#endif  // end of include guard
