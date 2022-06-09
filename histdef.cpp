#include "analysis.hpp"

void analysis::HistDef(){
  std::vector<std::string> gr_plane_name =
    {"gr_X1", "gr_V1", "gr_X2", "gr_U2"};

  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i] = new TH1F(Form("h_%s_hit", gr_plane_name[i].c_str()),
			Form("GR VDC %s hit", gr_plane_name[i].c_str()),
                        PLANE_SIZE, 0, PLANE_SIZE);

    hdrifttime[i] = new TH1F(Form("h_%s_tdc", gr_plane_name[i].c_str()),
			     Form("GR VDC %s TDC", gr_plane_name[i].c_str()),
                             MAX_VDC_TDC, 0, MAX_VDC_TDC);

    hdriftlen[i]  = new TH1F(Form("h_%s_len", gr_plane_name[i].c_str()),
			     Form("GR VDC %s drift length", gr_plane_name[i].c_str()),
                             512, 0, 16);
  }
}

void analysis::HistFill(){
  for(int i=0; i<(int)(evt.grvdc.size()); i++){
    int plane=evt.grvdc[i].plane;
    int wire=evt.grvdc[i].wire;
    //    int drifttime=evt.grvdc[i].lead_cor;
    if(wire>0){
      if(evt.eve==0){
	//	printf("plane=%d, wire=%d, raw=%d, dtime=%d\n",
	//	       plane, wire, evt.grvdc[i].lead_raw, drifttime);
      }
      hwire[plane]->Fill(wire);
      hdrifttime[plane]->Fill(evt.grvdc[i].lead_cor);
      hdriftlen[plane]->Fill(evt.grvdc[i].dlen);      
      evt.nhit_plane[plane]++;
      evt.mean_wire[plane]+=wire;
    }
  }  
}

void analysis::HistWrite(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i]->Write();
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hdrifttime[i]->Write();
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hdriftlen[i]->Write();    
  }
}
