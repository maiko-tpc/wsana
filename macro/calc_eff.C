{
  TCanvas *c_eff = new TCanvas("c_eff", "GR VDC efficiency", 1200, 800);
  c_eff->Divide(4,2);
  
  c_eff->cd(1);
  tree->Draw("nclst[0]>>eff_h1(5,0,5)", "grpidcut");
  eff_h1->SetTitle("X1 cluster eff");
  double eff1 = eff_h1->GetBinContent(2)/eff_h1->GetEntries();
  TLatex tex_eff1;
  tex_eff1.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff1));
  
  c_eff->cd(2);
  tree->Draw("nclst[1]>>eff_h2(5,0,5)", "grpidcut");
  eff_h2->SetTitle("U1 cluster eff");
  double eff2 = eff_h2->GetBinContent(2)/eff_h2->GetEntries();
  TLatex tex_eff2;
  tex_eff2.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff2));

  c_eff->cd(3);
  tree->Draw("nclst[2]>>eff_h3(5,0,5)", "grpidcut");
  eff_h3->SetTitle("X2 cluster eff");
  double eff3 = eff_h3->GetBinContent(2)/eff_h3->GetEntries();
  TLatex tex_eff3;
  tex_eff3.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff3));

  c_eff->cd(4);
  tree->Draw("nclst[3]>>eff_h4(5,0,5)", "grpidcut");
  eff_h4->SetTitle("U2 cluster eff");
  double eff4 = eff_h4->GetBinContent(2)/eff_h4->GetEntries();
  TLatex tex_eff4;
  tex_eff4.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff4));
  

  c_eff->cd(5);
  tree->Draw("nclst[0]>>eff_h11(5,0,5)",
	     "nclst[1]==1 && nclst[2]==1 && nclst[3]==1");
  eff_h11->SetTitle("X1 cluster relative eff");
  double eff11 = eff_h11->GetBinContent(2)/eff_h11->GetEntries();
  TLatex tex_eff11;
  tex_eff11.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff11));

  
  c_eff->cd(6);
  tree->Draw("nclst[1]>>eff_h12(5,0,5)",
	     "nclst[0]==1 && nclst[2]==1 && nclst[3]==1");
  eff_h12->SetTitle("U1 cluster relative eff");
  double eff12 = eff_h12->GetBinContent(2)/eff_h12->GetEntries();
  TLatex tex_eff12;
  tex_eff12.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff12));
  
  c_eff->cd(7);
  tree->Draw("nclst[2]>>eff_h13(5,0,5)",
	     "nclst[0]==1 && nclst[1]==1 && nclst[3]==1");
  eff_h13->SetTitle("X2 cluster relative eff");
  double eff13 = eff_h13->GetBinContent(2)/eff_h13->GetEntries();
  TLatex tex_eff13;
  tex_eff13.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff13));

  c_eff->cd(8);
  tree->Draw("nclst[3]>>eff_h14(5,0,5)",
	     "nclst[0]==1 && nclst[1]==1 && nclst[2]==1");
  eff_h14->SetTitle("U2 cluster relative eff");
  double eff14 = eff_h14->GetBinContent(2)/eff_h14->GetEntries();
  TLatex tex_eff14;
  tex_eff14.DrawLatexNDC(0.5,0.7, Form("eff = %.3f", eff14));
  
  
  printf("\n------------------\n");
  printf("Cluster Single Efficiency\n");
  printf("X1: %.3f, U1: %.3f, X2: %.3f, U2: %.3f\n\n",
	 eff1, eff2, eff3, eff4);  

  printf("Cluster Relative Efficiency\n");
  printf("X1: %.3f, U1: %.3f, X2: %.3f, U2: %.3f\n",
	 eff11, eff12, eff13, eff14);  
  printf("------------------\n\n");
}
