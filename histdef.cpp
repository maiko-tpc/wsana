#include "analysis.hpp"

void analysis::HistDef(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i] = new TH1F(Form("h%d", 1000+i), Form("VDC plane %d hit", i),
                        PLANE_SIZE, 0, PLANE_SIZE);
    hdrifttime[i] = new TH1F(Form("h%d", 1010+i),
			     Form("VDC plane %d drift time", i),
                             MAX_VDC_TDC, 0, MAX_VDC_TDC);
  }
}

void analysis::HistFill(){
  for(int i=0; i<(int)(evt.grvdc.size()); i++){
    int plane=evt.grvdc[i].plane;
    int wire=evt.grvdc[i].wire;
    int drifttime=evt.grvdc[i].lead_cor;
    if(wire>0){
      if(evt.eve==0){
	//	printf("plane=%d, wire=%d, raw=%d, dtime=%d\n",
	//	       plane, wire, evt.grvdc[i].lead_raw, drifttime);
      }
      hwire[plane]->Fill(wire);
      hdrifttime[plane]->Fill(drifttime);
      evt.nhit_plane[plane]++;
      evt.mean_wire[plane]+=wire;
    }
  }  
}

void analysis::HistWrite(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i]->Write();
    hdrifttime[i]->Write();
  }
}
