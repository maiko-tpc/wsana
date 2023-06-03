#include "analysis.hpp"

void analysis::HistDef(){
  std::vector<std::string> gr_plane_name =
    {"gr_X1", "gr_U1", "gr_X2", "gr_U2"};

  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i] = new TH1F(Form("h_%s_hit", gr_plane_name[i].c_str()),
			Form("GR VDC %s hit", gr_plane_name[i].c_str()),
                        PLANE_SIZE, 0, PLANE_SIZE);
    
    hdrifttime[i] = new TH1F(Form("h_%s_tdc", gr_plane_name[i].c_str()),
			     Form("GR VDC %s TDC", gr_plane_name[i].c_str()),
    			     MAX_VDC_TDC, 0, MAX_VDC_TDC);

    hdrifttime_coin[i] = new TH1F(Form("h_%s_tdc_coin", gr_plane_name[i].c_str()),
				  Form("GR VDC %s TDC coin trig", gr_plane_name[i].c_str()),
				  MAX_VDC_TDC, -10000, 10000);
    
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
			  512, 0, 2048);
  }

  for(int i=0; i<4; i++){
    hgrvqdc[i] = new TH1F(Form("h_gr_vqdc_ch%d", i),
			  Form("GR plastic VME QDC ch%d", i),
			  1024, 0, 4096);
  }

  for(int i=0; i<4; i++){
    hlasfqdc[i] = new TH1F(Form("h_las_fqdc_ch%d", i),
			  Form("LAS plastic FERA QDC ch%d", i),
			  512, 0, 2048);
  }

  for(int i=0; i<4; i++){
    hlasvqdc[i] = new TH1F(Form("h_las_vqdc_ch%d", i),
			  Form("LAS plastic VME QDC ch%d", i),
			  1024, 0, 4096);
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
     
  hv1190tdc = new TH2F("hv1190tdc","Pla V1190 TDC pattern",
		       16,0,16, 1000, -10000, 10000);
  
  hmadc_raw = new TH2F("hmadc_raw", "MADC32 without TDC gate",
		       N_MADC_CH,0,N_MADC_CH, 1024,0,4096);
  hmadc_tdc = new TH2F("hmadc_tdc", "MADC32 with TDC gate",
		       N_MADC_CH,0,N_MADC_CH, 1024,0,4096);    
  hmadc_ene = new TH2F("hmadc_ene", "MADC32 with TDC gate, calibrated",
		       N_MADC_CH,0,N_MADC_CH, 2048,0,50);    

#ifdef ANASSD
  
  // layer 1
  for(int i=0; i<3; i++){
    hsakra_hit[i] = new TH2F(Form("hsakra_hit%d", i),
			     Form("SAKRA hit pattern %d", i),
			     8, -1.0*sakra_ang*TMath::DegToRad(), sakra_ang*TMath::DegToRad(),
			     //			     8, -TMath::Pi(), TMath::Pi(), 
			     16, sakra_rmin, sakra_rmax);
  }

  // layer 2
  for(int i=3; i<6; i++){
    hsakra_hit[i] = new TH2F(Form("hsakra_hit%d", i),
			     Form("SAKRA hit pattern %d", i),
			     8, -1.0*sakra_ang*TMath::DegToRad(), sakra_ang*TMath::DegToRad(),
			     1, sakra_rmin, sakra_rmax);
  }

#endif

  hunixtimesub = new TH1F("unixtimesub", "time diff from run start", 500, 0, 10000);

  hgrtrackx = new TH2F("hgrtrackx", "trajectory of particles X",
		       500, -500,500, 400, 0, 1200);
  hgrtrackx->GetXaxis()->SetTitle("Z (mm)");
  hgrtrackx->GetYaxis()->SetTitle("X (mm)");  
  
  hgrtracky = new TH2F("hgrtracky", "trajectory of particles Y",
		       500, -500,500, 500, -500, 500);
  hgrtracky->GetXaxis()->SetTitle("Z (mm)");
  hgrtracky->GetYaxis()->SetTitle("Y (mm)");  
  
}

void analysis::HistFill(){
  for(int i=0; i<(int)(evt.grvdc.size()); i++){
    int plane=evt.grvdc[i].plane;
    int wire=evt.grvdc[i].wire;
    //    int drifttime=evt.grvdc[i].lead_cor;
    if(wire>0){
      hwire[plane]->Fill(wire);
      hdrifttime[plane]->Fill(evt.grvdc[i].lead_cor);
      if(evt.vme_inp[8]==1) hdrifttime_coin[plane]->Fill(evt.grvdc[i].lead_cor);      
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
    if(evt.nclst[i]==1) hclsteff[i]->Fill(1);
    if(evt.nclst[i]!=1) hclsteff[i]->Fill(0);            
  }

  if(evt.gr_good_hit==0) hhiteffall->Fill(0);
  if(evt.gr_good_hit==1) hhiteffall->Fill(1);  

  if(evt.gr_good_clst==0) hclsteffall->Fill(0);
  if(evt.gr_good_clst==1) hclsteffall->Fill(1);  

  // MADC histogram
#ifdef ANASSD
  for(int i=0; i<N_MADC_CH; i++){
    hmadc_raw->Fill(i, evt.madc.adc[i]);
    if(evt.v1190_ssd.hit[i]==1){
      hmadc_tdc->Fill(i, evt.madc.adc[i]);
      if(i<96) hmadc_ene->Fill(i, evt.ssd_ene[i]);
    }
  }

  for(int i=0; i<N_SAKRA; i++){
    if(evt.sakra_good_hit[i]==1){
      hsakra_hit[i]->Fill(evt.sakra_theta[i], evt.sakra_r[i]);
      //if(i==2) printf("sakra %f %f\n", evt.sakra_theta[i], evt.sakra_r[i]);
    }
  }
#endif

  hunixtimesub->Fill(evt.unixtimesub);

  if(evt.good_fit==1) FillGRTrack();

} // end of function


void analysis::HistWrite(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i]->Write();
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hdrifttime[i]->Write();
    hdrifttime_coin[i]->Write();    
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

#ifdef ANASSD
  hmadc_raw->Write();
  hmadc_tdc->Write();
  hmadc_ene->Write();    
  for(int i=0; i<N_SAKRA; i++){
    hsakra_hit[i]->Write();
  }
#endif

  hunixtimesub->Write();

  hgrtrackx->Write();
  hgrtracky->Write();  
}

void analysis::FillGRTrack(){

  int nbinx;
  float x,y;
  
  // X track
  nbinx = hgrtrackx->GetXaxis()->GetNbins();
  
  for(int i=0; i<nbinx; i++){
    x = hgrtrackx->GetXaxis()->GetBinCenter(i);
    y = x*tan(evt.grthx * TMath::DegToRad()) + evt.grx;
    hgrtrackx->Fill(x,y);
  }

  // Y Track
  nbinx = hgrtracky->GetXaxis()->GetNbins();
  
  for(int i=0; i<nbinx; i++){
    x = hgrtracky->GetXaxis()->GetBinCenter(i);
    y = x*tan(evt.grthy * TMath::DegToRad()) + evt.gry;
    hgrtracky->Fill(x,y);
  }

}
