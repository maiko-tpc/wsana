#ifndef KINEMA_HPP_
#define KINEMA_HPP_

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

const double AMU = 931.4943;  // atomic mass unit

struct nucl_data{
  char name[16];
  int A;
  int Z;
  double mass_ex;
};

class kinema{
public:
  kinema();
  ~kinema();
};

#endif
