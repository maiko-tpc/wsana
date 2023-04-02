{
  TCanvas *c_gr = new TCanvas("c_gr", "GR track", 800, 800);
  c_gr->Divide(2,2);

  c_gr->cd(1);
  tree->Draw("grthx:grx>>gr_h1(300,0,1200, 150,-15,15)","good_fit","colz");
  gr_h1->SetMinimum(1);
  gPad->SetLogz();

  c_gr->cd(2);
  tree->Draw("grx>>gr_h2(300,0,1200)", "good_fit");

  c_gr->cd(3);
  tree->Draw("grthx:grx>>gr_h3(300,0,1200, 150,-15,15)","good_fit && grpidcut","colz");
  gr_h3->SetMinimum(1);
  gPad->SetLogz();

  c_gr->cd(4);
  tree->Draw("grx>>gr_h4(300,0,1200)", "good_fit && grpidcut");

}
