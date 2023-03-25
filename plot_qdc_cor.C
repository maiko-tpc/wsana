{
  // QDC correlation
  TCanvas *c1 = new TCanvas("c1", "QDC correlation", 600, 400);
  c1->Divide(2,1);
  c1->cd(1);
  tree->Draw("grpla.vqdc[0]:grpla.fqdc[0]","","colz");
  c1->cd(2);
  tree->Draw("grpla.vqdc[0]/grpla.fqdc[0]:eve","grpla.vqdc[0]>50","colz");
  c1->SaveAs("run3161_qdc.png");
  
  TCanvas *c2 = new TCanvas("c2", "position correlation", 600, 400);
  c2->cd();
  tree->Draw("grpla.vqdc[0]:grpla.vtdc[0]-grpla.vtdc[1]>>h1(200,-500,500, 200,0,1000)","","colz");
  c2->SaveAs("run3161_tdc.png");
  
}
