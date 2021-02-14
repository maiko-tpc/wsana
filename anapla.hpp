#ifndef ANAPLA_HPP_
#define ANAPLA_HPP_

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

#include "mk_hist.hpp"
#include "mtformat.h"
#include "moduledata.h"
#include "evtdata.h"
#include "decoder.hpp"

class anapla{
public:
  anapla();
  ~anapla();

  void analyze(evtdata *);  
};
#endif
