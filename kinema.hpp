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

#include "mtformat.h"
#include "moduledata.h"
#include "evtdata.h"

const double AMU = 931.4943;     // atomic mass unit
const double rho_gr = 3.0;       // orbit radius of GR [m]
const double c = 299792458.0;

struct nucl{
  char name[16];
  int A;
  int Z;
  double mass_ex;
};

class kinema{

public:
  kinema();
  ~kinema();

  nucl target_nucl;
  nucl beam_nucl;
  nucl scat_nucl;
  nucl recoil_nucl;    

  std::vector<nucl> nucl_data;

  double beam_ene;
  double gr_ang;
  double gr_mag;    

  double gr_brho;
  
  double m1, m2, m3, m4;  // rest mass
  double p1, p2, p3, p4;  // momentum
  double p3_cen;          // center momentum of scattered particle [MeV/c]
  
  void SetMass();
  void SetBrho(pardata *par);  
  double Calc_p3(evtdata *evt);
  
private:
};

#endif
