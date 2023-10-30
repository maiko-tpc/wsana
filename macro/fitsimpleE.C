{

  double initParams[] = {2000, 662, 10, 500, -0.5 };
  //double initParams[] = {1700,573,10};

  TF1 * f1 = new TF1("f1","gaus(0)+pol1(3)",590,690); 
  //TF1 * f1 = new TF1("f1","gaus(0)",540,590); 
  f1->SetParameters(initParams);  // set initial parameters.

  //h7s_run9133->Fit(f1);
  hE1s->Fit(f1);

}

