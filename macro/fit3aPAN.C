int fit3a(){

  TChain *ch = new TChain("tree");
  //ch->Add("root/run6431.root");
  //ch->Add("root/run6443.root");
  //ch->Add("root/run9147.root");  

  //  ch->Add("out.root");
  
  //  TFile *file = new TFile("run6431.root");

  //  TTree *tree =(TTree*)file->Get("tree");
  
  Int_t maxpeaks = 4;
  Double_t *xpeaks;
  Double_t fitwid = 10;

  std::vector<double> mean;
  std::vector<double> sigma;  
  std::vector<double> ene3a;
  ene3a.push_back(3.182);
  ene3a.push_back(5.486);    
  ene3a.push_back(5.805);

  double p0,p1;
  
  TCanvas *c1 = new TCanvas("c1", "title", 800,800);
  
  TF1 *func0 = new TF1("func0", "gaus");
  TF1 *func1 = new TF1("func1", "gaus");
  TF1 *func2 = new TF1("func2", "gaus");  

  TF1 *func3 = new TF1("func3", "pol1");
  
  func0->SetLineColor(2);
  func1->SetLineColor(4);
  func2->SetLineColor(6);    

  func0->SetLineWidth(2);
  func1->SetLineWidth(2);
  func2->SetLineWidth(2);    

  func0->SetLineStyle(1);
  func1->SetLineStyle(1);
  func2->SetLineStyle(1);    
  
  system("rm -f ssdcalib.dat");

  FILE *fp;
  fp = fopen("ssdcalib.dat", "w");
  
  TH1F *htemp = new TH1F("htemp", "title", 1000,0,1000);
  TSpectrum *spe = new TSpectrum(maxpeaks);

  for(int i=0; i<160; i++){
    printf("ch%d\n", i);
    mean.clear();
    sigma.clear();
    
    tree->Draw(Form("madc.adc[%d]>>htemp",i),
	       Form("madc.adc[%d]>100",i));
    htemp->SetLineColor(1);
    spe->Search(htemp, 2,"nobackground new");
    xpeaks = spe->GetPositionX();
    
    printf("peak pos: %f, %f, %f\n", xpeaks[0], xpeaks[1], xpeaks[2]);
    
    htemp->Fit("func0", "Q0", "", xpeaks[0]-fitwid, xpeaks[0]+fitwid);
    htemp->Fit("func1", "Q0", "", xpeaks[1]-fitwid, xpeaks[1]+fitwid);
    htemp->Fit("func2", "Q0", "", xpeaks[2]-fitwid, xpeaks[2]+fitwid);  
    
    c1->Clear();
    c1->Divide(1,2);
    c1->cd(1);
    
    htemp->Draw("");
    func0->Draw("same");
    func1->Draw("same");
    func2->Draw("same");  
    
    mean.push_back(func0->GetParameter(1));
    mean.push_back(func1->GetParameter(1));
    mean.push_back(func2->GetParameter(1));    
    
    sigma.push_back(func0->GetParameter(2));
    sigma.push_back(func1->GetParameter(2));
    sigma.push_back(func2->GetParameter(2));
    
    std::sort(mean.begin(),mean.end());
    
    printf("mean: %f, %f, %f\n", mean[0], mean[1], mean[2]);
    printf("sigma: %f, %f, %f\n", sigma[0], sigma[1], sigma[2]);  
    
    c1->cd(2);
    TGraph *gr = new TGraph(3, &mean[0], &ene3a[0]);
    gr->SetTitle(Form("Calibration CH%d", i));
    gr->SetMarkerStyle(20);
    gr->GetXaxis()->SetLimits(0.,800); 
    gr->GetHistogram()->SetMinimum(0);
    gr->GetHistogram()->SetMaximum(7);   
    gr->GetXaxis()->SetTitle("ch");
    gr->GetYaxis()->SetTitle("energy (MeV)");  
    gr->Draw("AP");
    gr->Fit("func3","Q","");
    
    p0=func3->GetParameter(0);
    p1=func3->GetParameter(1);  
    printf("%d %f %f\n", i, p0, p1);
    
    fprintf(fp, "%d %f %f\n", i, p0, p1);

    //    c1->SaveAs(Form("~/ana/furuno/macro/pic/ch%d.png", i));
    sleep(1);
  }
  fclose(fp);

  return 0;
}

