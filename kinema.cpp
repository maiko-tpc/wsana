#include "kinema.hpp"

using namespace std;

kinema::kinema(){

  // Prepare the nuclear database
  // "name", (int)A, (int)Z, (double)mass_ex  
  nucl_data.push_back((nucl){"n",       1,  0,   8.071});  
  nucl_data.push_back((nucl){"1H",      1,  1,   6.778});
  nucl_data.push_back((nucl){"p",       1,  1,   6.778});
  nucl_data.push_back((nucl){"2H",      2,  1,  12.624});
  nucl_data.push_back((nucl){"d",       2,  1,  12.624});  
  nucl_data.push_back((nucl){"2He",     2,  2,  13.556});
  nucl_data.push_back((nucl){"3He",     3,  2,  13.909});
  nucl_data.push_back((nucl){"3H",      3,  1,  14.439});
  nucl_data.push_back((nucl){"t",       3,  1,  14.439});    
  nucl_data.push_back((nucl){"4He",     4,  2,   2.425});
  nucl_data.push_back((nucl){"a",       4,  2,   2.425});  
  nucl_data.push_back((nucl){"6Li",     6,  3,  14.086});
  nucl_data.push_back((nucl){"7Li",     7,  3,  14.908});
  nucl_data.push_back((nucl){"8He",     8,  2,  31.598});
  nucl_data.push_back((nucl){"9Be",     9,  4,  11.348});
  nucl_data.push_back((nucl){"10B",    10,  5,  12.050});
  nucl_data.push_back((nucl){"10C",    10,  6,  15.6986});
  nucl_data.push_back((nucl){"11B",    11,  5,   8.668});
  nucl_data.push_back((nucl){"12C",    12,  6,   0.000});
  nucl_data.push_back((nucl){"13C",    13,  6,   3.125});
  nucl_data.push_back((nucl){"14C",    14,  6,   3.0199});
  nucl_data.push_back((nucl){"14N",    14,  7,   2.8634});
  nucl_data.push_back((nucl){"16O",    16,  8,  -4.737});
  nucl_data.push_back((nucl){"18O",    18,  8,  -0.7821});
  nucl_data.push_back((nucl){"20Ne",   20, 10,  -7.0419});
  nucl_data.push_back((nucl){"24Mg",   24, 12, -13.93340});
  nucl_data.push_back((nucl){"197Au", 197, 79, -31.158});
  nucl_data.push_back((nucl){"208Pb", 208, 82, -21.764});

  // Set the default nuclei
  for(int i=0; i<(int)nucl_data.size(); i++){
    if(strcmp("12C", nucl_data[i].name)==0){
      target_nucl = nucl_data[i];
      recoil_nucl = nucl_data[i];
    }
  }
  for(int i=0; i<(int)nucl_data.size(); i++){
    if(strcmp("1H", nucl_data[i].name)==0){
      beam_nucl = nucl_data[i];
      scat_nucl = nucl_data[i];      
    }
  }
}

kinema::~kinema(){
}

void kinema::SetMass(){
  m1 = AMU*target_nucl.A + target_nucl.mass_ex;
  m2 = AMU*beam_nucl.A   + beam_nucl.mass_ex;
  m3 = AMU*scat_nucl.A   + scat_nucl.mass_ex;
  m4 = AMU*recoil_nucl.A + recoil_nucl.mass_ex;  
}
