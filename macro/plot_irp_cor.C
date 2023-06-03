{
  gStyle->SetOptStat(0);

  tree->Draw("vme_inp[8]:camac_inp[8]>>h1()","","colz");
  h1->SetTitle("run 3161");
  c1->SaveAs("run3161_irp.png");
}
