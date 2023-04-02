{
  gStyle->SetOptStat(0);

  TCanvas *c_gr_wire = new TCanvas("c_gr_wire","GR VDC wire hit", 800, 800);

  c_gr_wire->Divide(2,2);

  c_gr_wire->cd(1);
  h_gr_X1_hit->Draw();
  h_gr_X1_hit->GetXaxis()->SetTitle("wire ID");
  h_gr_X1_hit->GetXaxis()->SetRangeUser(0,192);
  
  c_gr_wire->cd(2);
  h_gr_U1_hit->Draw();
  h_gr_U1_hit->GetXaxis()->SetTitle("wire ID");
  h_gr_U1_hit->GetXaxis()->SetRangeUser(0,208);
  
  c_gr_wire->cd(3);
  h_gr_X2_hit->Draw();
  h_gr_X2_hit->GetXaxis()->SetTitle("wire ID");
  h_gr_X2_hit->GetXaxis()->SetRangeUser(0,192);
  
  c_gr_wire->cd(4);
  h_gr_U2_hit->Draw();
  h_gr_U2_hit->GetXaxis()->SetTitle("wire ID");
  h_gr_U2_hit->GetXaxis()->SetRangeUser(0,208);
}
