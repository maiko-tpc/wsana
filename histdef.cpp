#include "analysis.hpp"

void analysis::HistDef(){
  cinfo = new TCanvas("cinfo", "Run Information");

  char *gr_plane_name[N_VDCPLANE] = {"gr_X1", "gr_U1", "gr_X2", "gr_U2"};

  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i] = new TH1F(Form("h_%s_hit", gr_plane_name[i]),
			Form("GR VDC %s hit", gr_plane_name[i]),
                        PLANE_SIZE, 0, PLANE_SIZE);
    
    hdrifttime[i] = new TH1F(Form("h_%s_tdc", gr_plane_name[i]),
			     Form("GR VDC %s TDC", gr_plane_name[i]),
    			     MAX_VDC_TDC, 0, MAX_VDC_TDC);

    hdrifttime_coin[i] = new TH1F(Form("h_%s_tdc_coin", gr_plane_name[i]),
				  Form("GR VDC %s TDC coin trig", gr_plane_name[i]),
				  MAX_VDC_TDC, -10000, 10000);
    
    hdriftlen[i]  = new TH1F(Form("h_%s_len", gr_plane_name[i]),
			     Form("GR VDC %s drift length", gr_plane_name[i]),
                             512, 0, 16);

    hhiteff[i]  = new TH1F(Form("h_%s_hit_eff", gr_plane_name[i]),
			   Form("GR VDC %s hit efficiency", gr_plane_name[i]),
			   2, 0, 2);
    
    hclsteff[i]  = new TH1F(Form("h_%s_clst_eff", gr_plane_name[i]),
			    Form("GR VDC %s cluster efficiency", gr_plane_name[i]),
			   2, 0, 2);
  }

  hhiteffall  = new TH1F("h_hit_eff_all", "GR VDC hit efficiency all", 2, 0, 2);
  hclsteffall = new TH1F("h_clst_eff_all", "GR VDC cluster efficiency all", 2, 0, 2);  
    
  char *las_plane_name[N_VDCPLANE_LAS] =
    {"las_X1", "las_U1", "las_V1", "las_X2", "las_U2", "las_V2"};

  for(int i=0; i<N_VDCPLANE_LAS; i++){
    hwire_las[i] = new TH1F(Form("h_%s_hit", las_plane_name[i]),
			Form("LAS VDC %s hit", las_plane_name[i]),
                        PLANE_SIZE, 0, PLANE_SIZE);
    
    hdrifttime_las[i] = new TH1F(Form("h_%s_tdc", las_plane_name[i]),
			     Form("LAS VDC %s TDC", las_plane_name[i]),
    			     MAX_VDC_TDC, 0, MAX_VDC_TDC);

    hhiteff_las[i]  = new TH1F(Form("h_%s_hit_eff_las", las_plane_name[i]),
			       Form("LAS VDC %s hit efficiency", las_plane_name[i]),
			       2, 0, 2);
    
    hclsteff_las[i]  = new TH1F(Form("h_%s_clst_eff_las", las_plane_name[i]),
				Form("LAS VDC %s cluster efficiency", las_plane_name[i]),
				2, 0, 2);
    
  }

  hhiteffall_las  = new TH1F("h_hit_eff_all_las", "LAS VDC hit efficiency all", 2, 0, 2);
  hclsteffall_las = new TH1F("h_clst_eff_all_las", "LAS VDC cluster efficiency all", 2, 0, 2);  

  for(int i=0; i<N_PLA_CH; i++){
    hgrfqdc[i] = new TH1F(Form("h_gr_fqdc_ch%d", i),
			  Form("GR plastic FERA QDC ch%d", i),
			  512, 0, 2048);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hgrvqdc[i] = new TH1F(Form("h_gr_vqdc_ch%d", i),
			  Form("GR plastic VME QDC ch%d", i),
			  1024, 0, 4096);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasfqdc[i] = new TH1F(Form("h_las_fqdc_ch%d", i),
			  Form("LAS plastic FERA QDC ch%d", i),
			  512, 0, 2048);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasvqdc[i] = new TH1F(Form("h_las_vqdc_ch%d", i),
			  Form("LAS plastic VME QDC ch%d", i),
			  1024, 0, 4096);
  }

  
  for(int i=0; i<N_PLA_CH; i++){
    hgrqdccor[i] = new TH2F(Form("h_gr_qdc_cor_ch%d", i),
			    Form("GR plastic VME QDC vs FERA QDC ch%d", i),
			    256, 0, 2048, 256, 0, 4096);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasqdccor[i] = new TH2F(Form("h_las_qdc_cor_ch%d", i),
			     Form("LAS plastic VME QDC vs FERA QDC ch%d", i),
			     256, 0, 2048, 256, 0, 4096);
  }

  hmqdcglcor = new TH2F("hmqdcglcor", "MQDC GR-LAS consistency",
			256,0,4096, 256,0,4096);
  
  hgrposcor = new TH2F("hgrposcor", "GR PLA-VDC X consistency",
		       150,-150,150, 200,0,200);

  hlasposcor = new TH2F("hlasposcor", "LAS PLA-VDC X consistency",
		       300,-600,600, 250,0,250);
  
  hv1190tdc = new TH2F("hv1190tdc","Pla V1190 TDC pattern",
		       16,0,16, 1000, -10000, 10000);
  
  hmadc_raw = new TH2F("hmadc_raw", "MADC32 without TDC gate",
		       N_MADC_CH,0,N_MADC_CH, 1024,0,4096);
  hmadc_tdc = new TH2F("hmadc_tdc", "MADC32 with TDC gate",
		       N_MADC_CH,0,N_MADC_CH, 1024,0,4096);    
  hmadc_ene = new TH2F("hmadc_ene", "MADC32 with TDC gate, calibrated",
		       N_MADC_CH,0,N_MADC_CH, 2048,0,50);    

  for(int i=0; i<N_RF; i++){
    hgrrf[i] = new TH1F(Form("hgrrf%d", i), Form("GR RF CH%d", i),
			2048, 0, 4096);
  }

  hgrlascoin = new TH1F("hGR_LAS_coin", "GR LAS coin timing", 10000,0,20000);
  hgrlascoin_all = new TH1F("hGR_LAS_coin_all", "GR LAS coin timing all hits", 10000,0,20000);  

  /* GR PID */
  char *name_grpla[4] = {"1L", "1R", "2L", "2R"};
  for(int i=0; i<4; i++){
    hgrplaposq[i] = new TH2F(Form("hgrpla_%s_pos", name_grpla[i]),
			     Form("GR PLA QDC %s vs pos", name_grpla[i]),
			     150, -150, 150, 400,0,4000);
  }
  
  hgrplarfde[0] = new TH2F("hgrpla_1st_rfq", "GR PLA 1st dE vs RF",
			   200,0,4000, 200,0,4000);
  hgrplarfde[1] = new TH2F("hgrpla_2nd_rfq", "GR PLA 1st dE vs RF",
			   200,0,4000, 200,0,4000);
  

  /* LAS PID */
  char *name_laspla[12] = {"1L", "1R", "2L", "2R", "3L", "3R",
			   "4L", "4R", "5L", "5R", "6L", "6R"};  
  for(int i=0; i<12; i++){
    hlasplaposq[i] = new TH2F(Form("hlaspla_%s_pos", name_laspla[i]),
			      Form("LAS PLA QDC %s vs pos", name_laspla[i]),
			      600, -600, 600, 400,0,4000);
  }

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
		       500, -500,500, 400, -800, 800);
  hgrtrackx->GetXaxis()->SetTitle("Z (mm)");
  hgrtrackx->GetYaxis()->SetTitle("X (mm)");  
  
  hgrtracky = new TH2F("hgrtracky", "trajectory of particles Y",
		       500, -500,500, 500, -500, 500);
  hgrtracky->GetXaxis()->SetTitle("Z (mm)");
  hgrtracky->GetYaxis()->SetTitle("Y (mm)");  
  
  fitcan = new TCanvas("fitcan", "Fit result");
  fitcan->SetGridx(1);
  fitcan->SetGridy(1);  
  
  hgrx = new TH1F("hgrx", "GR X position", 1200, -600, 600);
  hgrx_thxgate = new TH1F("hgrx_thxgate", "GR X position angle gated",
			  1200, -600, 600);  
  hgry = new TH1F("hgry", "GR Y position", 100,  -50,  50);  

  hgrangx = new TH1F("hgrangx", "GR X angle", 200, -10, 10);
  hgrangy = new TH1F("hgrangy", "GR Y angle", 200, -10, 10);  
  
  hgrthx = new TH2F("hgrthx", "GR theta vs X", 200,-600,600, 200,-10,10);
  hgryx  = new TH2F("hgryx",  "GR Y vs X", 200,-600,600, 100,-50,50);  

  hgrrelap = new TH1F("hgrrelap", "GR relative momentum", 200,-4,4);  
} // end of histdef

void analysis::HistFill(){

  // GR VDC
  for(int i=0; i<(int)(evt.grvdc.size()); i++){  
    int plane=evt.grvdc[i].plane;
    int wire=evt.grvdc[i].wire;
    //    int drifttime=evt.grvdc[i].lead_cor;
    if(wire>=0){
      hwire[plane]->Fill(wire);
      hdrifttime[plane]->Fill(evt.grvdc[i].lead_cor);
      if(evt.vme_inp[8]==1) hdrifttime_coin[plane]->Fill(evt.grvdc[i].lead_cor);
      if(evt.grvdc[i].lead_cor>0 && evt.grvdc[i].lead_cor<10000){
	hdriftlen[plane]->Fill(evt.grvdc[i].dlen);
      }
    }
  }  

  // LAS VDC
#ifdef ANALAS
  for(int i=0; i<(int)(evt.lasvdc.size()); i++){  
    int plane=evt.lasvdc[i].plane;
    int wire=evt.lasvdc[i].wire;
    if(wire>=0){
      hwire_las[plane]->Fill(wire);
      hdrifttime_las[plane]->Fill(evt.lasvdc[i].lead_cor);
    }
  }  
#endif  
  

  for(int i=0; i<N_PLA_CH; i++){
    hgrfqdc[i]->Fill(evt.grpla.fqdc[i]);
    hgrvqdc[i]->Fill(evt.grpla.vqdc[i]);    
    hgrqdccor[i]->Fill(evt.grpla.fqdc[i], evt.grpla.vqdc[i]);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasfqdc[i]->Fill(evt.laspla.fqdc[i]);
    hlasvqdc[i]->Fill(evt.laspla.vqdc[i]);    
    hlasqdccor[i]->Fill(evt.laspla.fqdc[i], evt.laspla.vqdc[i]);
  }

  hmqdcglcor->Fill(evt.grpla.vqdc[0], evt.laspla.vqdc[14]);

  if(evt.nhit_plane[0]>0 && evt.vme_inp[7]==0){
    hgrposcor->Fill(evt.grpla.pos[0], evt.mean_wire[0]);
  }
  if(evt.nhit_plane_las[0]>0 && evt.vme_inp[5]==0){
    hlasposcor->Fill(evt.laspla.pos[2], evt.mean_wire_las[0]);
  }
  
  // small analysis for VDC efficiency
  // GR
  for(int i=0; i<N_VDCPLANE; i++){
    if(evt.nhit_plane[i]==0) hhiteff[i]->Fill(0);
    if(evt.nhit_plane[i] >0) hhiteff[i]->Fill(1);    
    if(evt.nclst[i]==1) hclsteff[i]->Fill(1);
    if(evt.nclst[i]!=1) hclsteff[i]->Fill(0);            
  }

  // LAS
  for(int i=0; i<N_VDCPLANE_LAS; i++){
    if(evt.vme_inp[8]==1 || evt.vme_inp[7]==1){
      if(evt.nhit_plane_las[i]==0) hhiteff_las[i]->Fill(0);
      if(evt.nhit_plane_las[i] >0) hhiteff_las[i]->Fill(1);    
    }
    //    if(evt.nclst_las[i]==1) hclsteff_las[i]->Fill(1);
    //    if(evt.nclst_las[i]!=1) hclsteff_las[i]->Fill(0);            
  }
  
  if(evt.gr_good_hit==0) hhiteffall->Fill(0);
  if(evt.gr_good_hit==1) hhiteffall->Fill(1);  

  if(evt.gr_good_clst==0) hclsteffall->Fill(0);
  if(evt.gr_good_clst==1) hclsteffall->Fill(1);  

  for(int i=0; i<N_RF; i++){
    hgrrf[i]->Fill(evt.rf[i]);
  }

  if(evt.vme_inp[8]==1){
    hgrlascoin->Fill(evt.grpla.vtdc[11]-evt.grpla.vtdc[15]);
  }

  // analyze all LAS stop timing for coin
  if(evt.vme_inp[8]==1){
    int v1190_size = (int)(evt.v1190_hit_all.size());
    int ch;
    for(int i=0; i<v1190_size; i++){
      ch = evt.v1190_hit_all[i].ch;
      if(ch==11) hgrlascoin_all->Fill(evt.v1190_hit_all[i].lead_cor - evt.grpla.vtdc[15]);
    }
  }

  for(int i=0; i<4; i++){
    hgrplaposq[i]->Fill(evt.grpla.pos[i/2], evt.grpla.vqdc[i]);
  }

  for(int i=0; i<2; i++){
    hgrplarfde[i]->Fill(evt.rf[0], evt.grpla.de[i]);
  }
  
  for(int i=0; i<12; i++){
    hlasplaposq[i]->Fill(evt.laspla.pos[i/2], evt.laspla.vqdc[i]);
  }

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

  if(evt.good_fit==1){
    FillGRTrack();

    hgrx->Fill(evt.grx);
    if(evt.grthx>par.gr_ang_gate_min && evt.grthx<par.gr_ang_gate_max){
      hgrx_thxgate->Fill(evt.grx);
    }
    
    hgry->Fill(evt.gry);    
    hgrangx->Fill(evt.grthx);
    hgrangy->Fill(evt.grthy);    
    hgrthx->Fill(evt.grx, evt.grthx);
    hgryx->Fill(evt.grx, evt.gry);    
    
    hgrrelap->Fill(evt.grp_rela);
  }

} // end of function


void analysis::HistWrite(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i]->Write();
  }
  for(int i=0; i<N_VDCPLANE; i++){
    hdrifttime[i]->Write();
  }
#ifdef ANALAS
  for(int i=0; i<N_VDCPLANE; i++){
    hdrifttime_coin[i]->Write();
  }
#endif
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

#ifdef ANALAS
  for(int i=0; i<N_VDCPLANE_LAS; i++){
    hwire_las[i]->Write();
  }
  for(int i=0; i<N_VDCPLANE_LAS; i++){
    hdrifttime_las[i]->Write();
  }
#endif  

  for(int i=0; i<N_RF; i++){
    hgrrf[i]->Write();
  }

  hgrlascoin->Write();
  hgrlascoin_all->Write();  
  
  for(int i=0; i<4; i++){
    hgrplaposq[i]->Write();
  }

  for(int i=0; i<2; i++){
    hgrplarfde[i]->Write();
  }
  
  hgrx->Write();
  hgrx_thxgate->Write();
  hgry->Write();  
  hgrangx->Write();
  hgrangy->Write();  
  hgrthx->Write();
  hgryx->Write();  

  hgrrelap->Write();
  
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

  hmqdcglcor->Write();
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
