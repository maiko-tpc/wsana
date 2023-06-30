{

// fitting for elastic peak
  tree->Draw("grx>>h1(100,-100,100)", "grthx>-4 && grthx<-3 && grpidcut");
  tree->Draw("grx>>h2(100,-100,100)", "grthx>-3 && grthx<-2 && grpidcut");
  tree->Draw("grx>>h3(100,-100,100)", "grthx>-2 && grthx<-1 && grpidcut");
  tree->Draw("grx>>h4(100,-100,100)", "grthx>-1 && grthx< 0 && grpidcut");  

  TF1 *f1 = new TF1("fit1", "gaus");
  TF1 *f2 = new TF1("fit2", "gaus");
  TF1 *f3 = new TF1("fit3", "gaus");
  TF1 *f4 = new TF1("fit4", "gaus");  

// tune elastic ranges if gaussian is a little lost
  h1->Fit("fit1", "","", -5, 15);
  h2->Fit("fit2", "","", -5, 15);
  h3->Fit("fit3", "","", -5, 15);
  h4->Fit("fit4", "","", -5, 15);

// integrating to get elastic counts for quick analysis
  double cnt1 = f1->Integral(-600,600);
  double cnt2 = f2->Integral(-600,600);
  double cnt3 = f3->Integral(-600,600);
  double cnt4 = f4->Integral(-600,600);  

//fitting for first excited state (2+) peak
  tree->Draw("grx>>h5(100,-100,100)", "grthx>-4 && grthx<-3 && grpidcut");
  tree->Draw("grx>>h6(100,-100,100)", "grthx>-3 && grthx<-2 && grpidcut");
  tree->Draw("grx>>h7(100,-100,100)", "grthx>-2 && grthx<-1 && grpidcut");
  tree->Draw("grx>>h8(100,-100,100)", "grthx>-1 && grthx<-0 && grpidcut");

  TF1 *f5 = new TF1("fit5", "gaus");
  TF1 *f6 = new TF1("fit6", "gaus");
  TF1 *f7 = new TF1("fit7", "gaus");
  TF1 *f8 = new TF1("fit8", "gaus");  

// tune 2+ ranges if gaussian is a little lost
  h5->Fit("fit5", "","", -60, -40);
  h6->Fit("fit6", "","", -60, -40);
  h7->Fit("fit7", "","", -60, -42);
  h8->Fit("fit8", "","", -55, -40);

//integrating total counts to get a measure of the statistical uncertainty
  double cnt5 = f5->Integral(-600,600);
  double cnt6 = f6->Integral(-600,600);
  double cnt7 = f7->Integral(-600,600);
  double cnt8 = f8->Integral(-600,600);  

  printf("\n----ELASTIC PEAK COUNTS--------\n");
  printf("----(Enter in sheet)-----------\n");
  printf("h1: %f\n", cnt1);
  printf("h2: %f\n", cnt2);
  printf("h3: %f\n", cnt3);
  printf("h4: %f\n", cnt4);  
  printf("----2+ COUNTS[perc_err]---\n"); // look at uncertainties to determine if runtime
  printf("----Do NOT enter in sheet-\n");
  printf("%f\t[%f]\n", cnt5, 1/sqrt(cnt5)*100); //is appropriate!!
  printf("%f\t[%f]\n", cnt6, 1/sqrt(cnt6)*100);
  printf("%f\t[%f]\n", cnt7, 1/sqrt(cnt7)*100);
  printf("%f\t[%f]\n", cnt8, 1/sqrt(cnt8)*100);
  printf("---if percent error >5, run longer!!--\n");  
  printf("--------------------------------------\n");
  
  
  TCanvas *can = new TCanvas("can", "elastic counts fits");
  can->Divide(2,2);

  can->cd(1);
  h1->Draw();

  can->cd(2);
  h2->Draw();

  can->cd(3);
  h3->Draw();

  can->cd(4);
  h4->Draw();

  TCanvas *can2 = new TCanvas("can2", "2+ state error checks");
  can2->SetWindowPosition(715,28);
  can2->Divide(2,2);
  can2->cd(1);
  h5->Draw();

  can2->cd(2);
  h6->Draw();

  can2->cd(3);
  h7->Draw();

  can2->cd(4);
  h8->Draw();
}
