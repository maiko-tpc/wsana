#include "analysis.hpp"

void analysis::MakeTHttp(int portnum){
  serv = new THttpServer(Form("http:%d?thrds=2;rw", portnum));

  // One could specify location of newer version of JSROOT
  //serv->SetJSROOT("https://root.cern.ch/js/latest/");

  //  serv->Register("", outfile);

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hwire[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hdrifttime[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hdriftlen[i]);
  }

  for(int i=0; i<4; i++){
    serv->Register("/GR_pla", hgrfqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/GR_pla", hgrvqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/GR_pla", hgrqdccor[i]);
  }

  for(int i=0; i<4; i++){
    serv->Register("/LAS_pla", hlasfqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/LAS_pla", hlasvqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/LAS_pla", hlasqdccor[i]);
  }

  serv->SetItemField("/GR_pla","_drawopt","col");
 
}

void analysis::CloseTHttp(){
}
