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

private:
};

#endif // end of include guard
