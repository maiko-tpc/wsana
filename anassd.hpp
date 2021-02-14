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


class anassd{
public:
  anassd();
  ~anassd();

  void V1190Hit2SSDTDC(evtdata *);
  void analyze(evtdata *);
};

#endif  // end of include guard
