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

    hhiteff[i]  = new TH1F(Form("h_%s_hit_eff", gr_plane_name[i].c_str()),
			   Form("GR VDC %s hit efficiency", gr_plane_name[i].c_str()),
			   2, 0, 2);
    
    hclsteff[i]  = new TH1F(Form("h_%s_clst_eff", gr_plane_name[i].c_str()),
			    Form("GR VDC %s cluster efficiency", gr_plane_name[i].c_str()),
			   2, 0, 2);
  }

  hhiteffall  = new TH1F("h_hit_eff_all", "GR VDC hit efficiency all", 2, 0, 2);
  hclsteffall = new TH1F("h_clst_eff_all", "GR VDC cluster efficiency all", 2, 0, 2);  
    
  for(int i=0; i<4; i++){
    hgrfqdc[i] = new TH1F(Form("h_gr_fqdc_ch%d", i),
			  Form("GR plastic FERA QDC ch%d", i),
			  512, 0, 4096);
  }

  for(int i=0; i<4; i++){
    hgrvqdc[i] = new TH1F(Form("h_gr_vqdc_ch%d", i),
			  Form("GR plastic VME QDC ch%d", i),
			  512, 0, 4096);
  }

  for(int i=0; i<4; i++){
    hlasfqdc[i] = new TH1F(Form("h_las_fqdc_ch%d", i),
			  Form("LAS plastic FERA QDC ch%d", i),
			  512, 0, 4096);
  }

  for(int i=0; i<4; i++){
    hlasvqdc[i] = new TH1F(Form("h_las_vqdc_ch%d", i),
			  Form("LAS plastic VME QDC ch%d", i),
			  512, 0, 4096);
  }

  
  for(int i=0; i<4; i++){
    hgrqdccor[i] = new TH2F(Form("h_gr_qdc_cor_ch%d", i),
			    Form("GR plastic VME QDC vs FERA QDC ch%d", i),
			    256, 0, 4096, 256, 0, 4096);
  }

  for(int i=0; i<4; i++){
    hlasqdccor[i] = new TH2F(Form("h_las_qdc_cor_ch%d", i),
			     Form("LAS plastic VME QDC vs FERA QDC ch%d", i),
			     256, 0, 4096, 256, 0, 4096);
  }

  for(int i=0; i<N_VDCPLANE; i++){
  }
     

}

void analysis::HistFill(){
  for(int i=0; i<(int)(evt.grvdc.size()); i++){
    int plane=evt.grvdc[i].plane;
    int wire=evt.grvdc[i].wire;
    //    int drifttime=evt.grvdc[i].lead_cor;
    if(wire>0){
      hwire[plane]->Fill(wire);
      hdrifttime[plane]->Fill(evt.grvdc[i].lead_cor);
      hdriftlen[plane]->Fill(evt.grvdc[i].dlen);      
      evt.nhit_plane[plane]++;
      evt.mean_wire[plane]+=wire;
    }
  }  


  for(int i=0; i<4; i++){
    hgrfqdc[i]->Fill(evt.grpla.fqdc[i]);
    hgrvqdc[i]->Fill(evt.grpla.vqdc[i]);    
    hgrqdccor[i]->Fill(evt.grpla.fqdc[i], evt.grpla.vqdc[i]);
  }

  for(int i=0; i<4; i++){
    hlasfqdc[i]->Fill(evt.laspla.fqdc[i]);
    hlasvqdc[i]->Fill(evt.laspla.vqdc[i]);    
    hlasqdccor[i]->Fill(evt.laspla.fqdc[i], evt.laspla.vqdc[i]);
  }

  // small analysis for VDC efficiency
  for(int i=0; i<N_VDCPLANE; i++){
    if(evt.nhit_plane[i]==0) hhiteff[i]->Fill(0);
    if(evt.nhit_plane[i] >0) hhiteff[i]->Fill(1);    
  }

  if(evt.gr_good_hit==0) hhiteffall->Fill(0);
  if(evt.gr_good_hit==1) hhiteffall->Fill(1);  

  if(evt.gr_good_clst==0) hclsteffall->Fill(0);
  if(evt.gr_good_clst==1) hclsteffall->Fill(1);  
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
  for(int i=0; i<N_VDCPLANE; i++){
    hdriftlen[i]->Write();    
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hhiteff[i]->Write();    
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hclsteff[i]->Write();    
  }
  hhiteffall->Write();
  hclsteffall->Write();      
}
