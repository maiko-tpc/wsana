#include "analysis.hpp"
#include "anagr.hpp"

bool bResetHist = FALSE;

void analysis::MakeTHttp(int portnum){
  serv = new THttpServer(Form("http:%d?thrds=2;rw", portnum));

  // One could specify location of newer version of JSROOT
  //  serv->SetJSROOT("/home/tamidaq/cern/root/v6.26.04/js/");

  //  serv->SetItemField("/", "_drawopt", "colz");
  //  serv->SetItemField("/","_monitoring","1000");
  
  /* Fit button definition */
  //  string fitcmd = Form("gSystem->Exec(\"kill -10 %d\");", pid);
  string fitcmd = Form("gSystem->Exec(\"kill -%d %d\");", SIGUSR1, pid);  
  serv->RegisterCommand("/Commands/Fit", fitcmd.c_str(),
  			"button;rootsys/icons/Root6Icon.png");  

  /* Clear button definition */
  string clearcmd = Form("gSystem->Exec(\"kill -%d %d\");", SIGUSR2, pid);  
  serv->RegisterCommand("/Commands/Clear", clearcmd.c_str(),
  			"button;rootsys/icons/refresh.png");  
  

  serv->Register("/Info", cinfo);
  
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

  hgrtrackx->SetOption("colz");
  hgrtracky->SetOption("colz");  
  serv->Register("/GR", hgrtrackx);  
  serv->Register("/GR", hgrtracky);  
  
  for(int i=0; i<5; i++){
    serv->Register("/GR_PLA", hgrfqdc[i]);
  }
  for(int i=0; i<8; i++){
    serv->Register("/GR_PLA", hgrvqdc[i]);
  }

  for(int i=0; i<4; i++){
    hgrqdccor[i]->SetOption("colz");
    serv->Register("/Consistency", hgrqdccor[i]);
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasqdccor[i]->SetOption("colz");
    serv->Register("/Consistency", hlasqdccor[i]);
  }

  hmqdcglcor->SetOption("colz");
  serv->Register("/Consistency", hmqdcglcor);
  
  hgrposcor->SetOption("colz");
  serv->Register("/Consistency", hgrposcor);  

  hlasposcor->SetOption("colz");
  serv->Register("/Consistency", hlasposcor);  

  serv->Register("/Consistency", hgrmadccor);  
  
  for(int i=0; i<4; i++){
    hgrplaposq[i]->SetOption("colz");
    serv->Register("/GR_PLA", hgrplaposq[i]);    
  }   

  for(int i=0; i<2; i++){
    hgrplarfde[i]->SetOption("colz");
    serv->Register("/GR_PLA", hgrplarfde[i]);
  }
  
  for(int i=0; i<N_RF; i++){
    serv->Register("/GR_RF", hgrrf[i]);
  }

  serv->Register("/GR_LAS", hgrlascoin);
  serv->Register("/GR_LAS", hgrlascoin_all);    
  
  for(int i=0; i<16; i++){
    serv->Register("/LAS_PLA", hlasfqdc[i]);
  }
  for(int i=0; i<16; i++){
    serv->Register("/LAS_PLA", hlasvqdc[i]);
  }
  for(int i=0; i<6; i++){
    serv->Register("/LAS_PLA", hlasqdccor[i]);
  }

  for(int i=0; i<12; i++){
    hlasplaposq[i]->SetOption("colz");
    serv->Register("/LAS_PLA", hlasplaposq[i]);    
  }   

  serv->Register("/Fit", fitcan);

  serv->Register("/GR", hgrx);
  serv->Register("/GR", hgrx_thxgate);  
  serv->Register("/GR", hgry);    
  serv->Register("/GR", hgrangx);
  serv->Register("/GR", hgrangy);    

  hgrthx->SetOption("colz");
  hgryx->SetOption("colz");  
  serv->Register("/GR", hgrthx);
  serv->Register("/GR", hgryx);

  serv->Register("/GR", hgrrelap);    

#ifdef ANALAS
  for(int i=0; i<N_VDCPLANE_LAS; i++){
    serv->Register("/LAS_VDC", hwire_las[i]);
  }

  for(int i=0; i<N_VDCPLANE_LAS; i++){
    serv->Register("/LAS_VDC", hdrifttime_las[i]);
  }
#endif
}

void analysis::HttpHistReset(){
  printf("clear function\n");  
  
  HttpInfoUpdate();

  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i]->Reset();
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    hdrifttime[i]->Reset();
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    hdriftlen[i]->Reset();
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    hhiteff[i]->Reset();
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    hclsteff[i]->Reset();
  }
  
  hhiteffall->Reset();
  hclsteffall->Reset();
  
  hgrtrackx->Reset();
  hgrtracky->Reset();  
  
  for(int i=0; i<4; i++){
    hgrfqdc[i]->Reset();
  }
  for(int i=0; i<10; i++){
    hgrvqdc[i]->Reset();
  }
  for(int i=0; i<4; i++){
    hgrqdccor[i]->Reset();
  }

  for(int i=0; i<N_PLA_CH; i++){
    hlasqdccor[i]->Reset();
  }

  hmqdcglcor->Reset();
  hgrposcor->Reset();
  hlasposcor->Reset();
  hgrmadccor->Reset();    
  
  for(int i=0; i<4; i++){
    hgrplaposq[i]->Reset();
  }   
  
  for(int i=0; i<2; i++){
    hgrplarfde[i]->Reset();
  }
  
  for(int i=0; i<N_RF; i++){
    hgrrf[i]->Reset();
  }
  
  hgrlascoin->Reset();
  hgrlascoin_all->Reset();  
	    
  for(int i=0; i<4; i++){
    hlasfqdc[i]->Reset();
  }
  for(int i=0; i<4; i++){
    hlasvqdc[i]->Reset();
  }
  for(int i=0; i<4; i++){
    hlasqdccor[i]->Reset();
  }
  for(int i=0; i<12; i++){
    hlasplaposq[i]->Reset();
  }   
  
  hgrx->Reset();
  hgrx_thxgate->Reset();
  hgry->Reset();
  hgrangx->Reset();
  hgrangy->Reset();
  
  hgrthx->Reset();
  hgryx->Reset();  

  hgrrelap->Reset();
  
#ifdef ANALAS
  for(int i=0; i<N_VDCPLANE_LAS; i++){
    hwire_las[i]->Reset();
  }

  for(int i=0; i<N_VDCPLANE_LAS; i++){
    hdrifttime_las[i]->Reset();
  }
#endif
}

void analysis::HttpHistFit(){  
  printf("fit function\n");  

  /* get fit region from the file */
  //  read_config_file(opt.parfname);
  double fit_min = config_get_d_value("gr_fit_min", 0, -600.0);
  double fit_max = config_get_d_value("gr_fit_max", 0,  600.0);  
  printf("fit region: from %.1f to %.1f\n", fit_min, fit_max);
  printf("theta X gate: from %.1f to %.1f\n",
	 par.gr_ang_gate_min, par.gr_ang_gate_max);  
  printf("\n");
  
  TF1 *fitfunc = new TF1("fitfunc", "gaus", fit_min, fit_max);
  fitfunc->SetNpx(1000);

  TH1F *histtemp = (TH1F*)hgrx_thxgate->Clone();  
  
  TLatex latex1;
  latex1.SetTextAlign(13);

  /* Get Present Time */
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);
  TLatex latex2;
  latex2.SetTextAlign(11);
  latex2.SetTextSize(0.04);
  
  histtemp->Fit("fitfunc", "","", fit_min, fit_max);
  
  fitcan->Clear();
  fitcan->cd();
  histtemp->Draw();
  fitfunc->Draw("same");

  latex1.DrawLatexNDC(0.12, 0.85, Form("FWHM wid: %.1f mm", fitfunc->GetParameter(2)*2.35 ));
  latex2.DrawLatexNDC(0.10, 0.91, Form("%04d/%02d/%02d %02d:%02d:%02d   run%04d",
				       tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
				       tm.tm_hour, tm.tm_min, tm.tm_sec,
				       evt.run));
  
  double p_wid = fitfunc->GetParameter(2)*2.35/(15451.0*sqrt(2.0));
  double ene_wid = kine->GetBeamEne()*p_wid*2.0;

  double counts = fitfunc->Integral(-600,600);
  
  latex1.DrawLatexNDC(0.12, 0.80, Form("Ene reso: %.1f keV", ene_wid*1000.0));
  latex1.DrawLatexNDC(0.12, 0.70, Form("Integ: %.1e counts", counts));  
  
  gSystem->ProcessEvents();
  
}

void analysis::HttpInfoUpdate(){
  cinfo->Clear();
  cinfo->cd();
  
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);

  TLatex latex1;
  latex1.SetTextAlign(11);
  
  latex1.DrawLatexNDC(0.15, 0.80, Form("%04d/%02d/%02d %02d:%02d:%02d",
				       tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
				       tm.tm_hour, tm.tm_min, tm.tm_sec));
  latex1.DrawLatexNDC(0.15, 0.75, Form("Run %04d", evt.run));
  latex1.DrawLatexNDC(0.15, 0.70, Form("Analyzed %d events (%d blk)", evt.eve, evt.blk));    

  if(GetRefresh()){
    latex1.DrawLatexNDC(0.15, 0.60, "Auto refresh hist in 3min");    
  }
}

void analysis::CloseTHttp(){
}
