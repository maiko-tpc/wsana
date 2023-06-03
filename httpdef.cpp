#include "analysis.hpp"

bool bResetHist = FALSE;

void analysis::MakeTHttp(int portnum){
  serv = new THttpServer(Form("http:%d?thrds=2;rw", portnum));

  // One could specify location of newer version of JSROOT
  //  serv->SetJSROOT("/home/tamidaq/cern/root/v6.26.04/js/");

  serv->SetItemField("/", "_drawopt", "colz");
  serv->SetItemField("/","_monitoring","1000");
  
  serv->RegisterCommand("/Clear","gSystem->Exec(\"ls\");", "button;rootsys/icons/bld_delete.png");
  
  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hwire[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hdrifttime[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hdriftlen[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hhiteff[i]);
  }

  for(int i=0; i<N_VDCPLANE; i++){
    serv->Register("/GR", hclsteff[i]);
  }

  serv->Register("/GR", hhiteffall);
  serv->Register("/GR", hclsteffall);  

  serv->Register("/GR", hgrtrackx);
  serv->Register("/GR", hgrtracky);  
  
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

}

void analysis::HttpHistReset(){
  if(bResetHist){
    printf("reset hist\n");
  }
}

void analysis::HttpHistFit(){
  //  if(bFitHist){
  //    bFitHist = kFALSE;
  //  }
}

void analysis::CloseTHttp(){
}
