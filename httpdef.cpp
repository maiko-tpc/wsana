#include "analysis.hpp"
#include "anagr.hpp"

bool bResetHist = FALSE;

void analysis::MakeTHttp(int portnum){
  serv = new THttpServer(Form("http:%d?thrds=2;rw", portnum));

  // One could specify location of newer version of JSROOT
  //  serv->SetJSROOT("/home/tamidaq/cern/root/v6.26.04/js/");

  //  serv->SetItemField("/", "_drawopt", "colz");
  //  serv->SetItemField("/","_monitoring","1000");
  
  string fitcmd = Form("gSystem->Exec(\"kill -10 %d\");", pid);
  serv->RegisterCommand("/Commands/Fit", fitcmd.c_str(),
  			"button;rootsys/icons/bld_delete.png");  

  
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
    serv->Register("/GR_PLA", hgrfqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/GR_PLA", hgrvqdc[i]);
  }
  for(int i=0; i<4; i++){
    hgrqdccor[i]->SetOption("colz");
    serv->Register("/GR_PLA", hgrqdccor[i]);
  }

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
  
  for(int i=0; i<4; i++){
    serv->Register("/LAS_PLA", hlasfqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/LAS_PLA", hlasvqdc[i]);
  }
  for(int i=0; i<4; i++){
    serv->Register("/LAS_PLA", hlasqdccor[i]);
  }

  serv->Register("/Fit", fitcan);

  serv->Register("/GR", hgrx);  

}

void analysis::HttpHistReset(){
}

void analysis::HttpHistFit(){  
  printf("fit function\n");  
  TF1 *fitfunc = new TF1("fitfunc", "gaus");
  TH1F *histtemp = (TH1F*)hgrx->Clone();

  TLatex latex1;
  latex1.SetTextAlign(13);

  /* Get Present Time */
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);
  TLatex latex2;
  latex2.SetTextAlign(11);
  latex2.SetTextSize(0.04);
  
  histtemp->Fit("fitfunc", "","", -250, 0);
  
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

  latex1.DrawLatexNDC(0.12, 0.80, Form("Ene reso: %.1f keV", ene_wid*1000.0));
  
  gSystem->ProcessEvents();
  
}

void analysis::CloseTHttp(){
}
