{
  TFile *_file0 = TFile::Open("root/run1168.root");
  

  tree->Draw("grx:eve>>h1(7000,0,700000, 1200,-600,600)",
	     "good_fit","colz");

  vector<float> eve;
  vector<float> eve_err;  
  vector<float> mean;
  vector<float> mean_err;
  vector<float> entry;
  vector<float> sigma;
  vector<float> reso;
  vector<float> reso_err;  
  int cnt=0;  
  
  for(int i=0; i<300; i++){
    cout << i << endl;    
    h1->ProjectionY("temp", i+1, i+2, "");

    eve.push_back(h1->GetXaxis()->GetBinCenter(i+1)/1000.0);    
    eve_err.push_back(0);   
    mean.push_back(temp->GetMean());
    mean_err.push_back(temp->GetStdDev()/sqrt(temp->GetEntries()));    
    reso.push_back(temp->GetStdDev());    
    reso_err.push_back(temp->GetStdDevError());        
    cnt++;    
  }
  
  TGraphErrors *gr1 = new TGraphErrors(cnt, &eve[0], &mean[0],
				      &eve_err[0], &mean_err[0]);

  gr1->SetMarkerStyle(20);
  gr1->SetMarkerColor(2);
  gr1->SetLineColor(2);  
  gr1->GetXaxis()->SetTitle("Mean Event Number");
  gr1->GetYaxis()->SetTitle("Mean X Position (mm)");  

  TGraphErrors *gr2 = new TGraphErrors(cnt, &eve[0], &reso[0],
				      &eve_err[0], &reso_err[0]);

  gr2->SetMarkerStyle(20);
  gr2->SetMarkerColor(4);
  gr2->SetLineColor(4);  
  //  gr2->GetXaxis()->SetTitle("R¤ª");  
  gr2->GetYaxis()->SetTitle("X sigma (mm)");  
  
  TCanvas *can = new TCanvas();
  can->Divide(1,2);
  can->cd(1);
  gr1->Draw("AP");
  can->cd(2);
  gr2->Draw("AP");
  
}
