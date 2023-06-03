{
  TCanvas *c_gr_pid = new TCanvas("c_gr_pid","GR PID with plastic", 800, 800);

  c_gr_pid->Divide(2,2);

  c_gr_pid->cd(1);
  tree->Draw("grpla.vqdc[0]:grpla.vtdc[0]-grpla.vtdc[1]>>grpid_h1(150,-150,150, 400,0,4000)",
	     "", "colz");
  grpid_h1->SetTitle("GR QDC 1L v.s. PLA1 pos");
  grpid_h1->SetMinimum(1);
  gPad->SetLogz();
  
  c_gr_pid->cd(2);
  tree->Draw("grpla.vqdc[1]:grpla.vtdc[0]-grpla.vtdc[1]>>grpid_h2(150,-150,150, 400,0,4000)",
	     "", "colz");
  grpid_h2->SetTitle("GR QDC 1R v.s. PLA1 pos");
  grpid_h2->SetMinimum(1);
  gPad->SetLogz();

  c_gr_pid->cd(3);
  tree->Draw("grpla.vqdc[2]:grpla.vtdc[2]-grpla.vtdc[3]>>grpid_h3(150,-150,150, 400,0,4000)",
	     "", "colz");
  grpid_h3->SetTitle("GR QDC 2L v.s. PLA2 pos");
  grpid_h3->SetMinimum(1);
  gPad->SetLogz();
  
  c_gr_pid->cd(4);
  tree->Draw("grpla.vqdc[3]:grpla.vtdc[2]-grpla.vtdc[3]>>grpid_h4(150,-150,150, 400,0,4000)",
	     "", "colz");
  grpid_h4->SetTitle("GR QDC 2R v.s. PLA2 pos");
  grpid_h4->SetMinimum(1);
  gPad->SetLogz();

  c_gr_pid->SaveAs("pic/tmp.png");
  
  // With PID cut

  TCanvas *c_gr_pid2 = new TCanvas("c_gr_pid2","GR PID witgrpid_h plastic with CUT", 800, 800);

  c_gr_pid2->Divide(2,2);

  c_gr_pid2->cd(1);
  tree->Draw("grpla.vqdc[0]:grpla.vtdc[0]-grpla.vtdc[1]>>grpid_h11(150,-150,150, 400,0,4000)",
	     "grpidcut", "colz");
  grpid_h11->SetTitle("GR QDC 1L v.s. PLA1 pos with CUT");
  grpid_h11->SetMinimum(1);
  gPad->SetLogz();
  
  c_gr_pid2->cd(2);
  tree->Draw("grpla.vqdc[1]:grpla.vtdc[0]-grpla.vtdc[1]>>grpid_h12(150,-150,150, 400,0,4000)",
	     "grpidcut", "colz");
  grpid_h12->SetTitle("GR QDC 1R v.s. PLA1 pos with CUT");
  grpid_h12->SetMinimum(1);
  gPad->SetLogz();

  c_gr_pid2->cd(3);
  tree->Draw("grpla.vqdc[2]:grpla.vtdc[2]-grpla.vtdc[3]>>grpid_h13(150,-150,150, 400,0,4000)",
	     "grpidcut", "colz");
  grpid_h13->SetTitle("GR QDC 2L v.s. PLA2 pos with CUT");
  grpid_h13->SetMinimum(1);
  gPad->SetLogz();
  
  c_gr_pid2->cd(4);
  tree->Draw("grpla.vqdc[3]:grpla.vtdc[2]-grpla.vtdc[3]>>grpid_h14(150,-150,150, 400,0,4000)",
	     "grpidcut", "colz");
  grpid_h14->SetTitle("GR QDC 2R v.s. PLA2 pos with CUT");
  grpid_h14->SetMinimum(1);
  gPad->SetLogz();

}
