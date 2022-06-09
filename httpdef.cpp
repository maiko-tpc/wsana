#include "analysis.hpp"

void analysis::MakeTHttp(int portnum){
  serv = new THttpServer(Form("http:%d?thrds=2;rw", portnum));
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
}

void analysis::CloseTHttp(){
}
