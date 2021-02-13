#ifndef CHAMB_HPP_
#define CHAMB_HPP_

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

#include "mk_hist.hpp"
#include "mtformat.h"
#include "moduledata.h"
//#include "evtdata.h"
#include "decoder.hpp"
#include "anagr.hpp"

class chamb{
public:
  chamb();
  ~chamb();
};

#endif
