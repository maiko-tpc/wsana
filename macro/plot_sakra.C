int plot_sakra(){
  TCanvas *can =  new TCanvas("can", "sakra hit", 800, 800);
  can->Divide(3,2);

  can->cd(1);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit0->Draw("colz pol same");

  can->cd(2);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit1->Draw("colz pol same");

  can->cd(3);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit2->Draw("colz pol same");

  can->cd(4);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit3->Draw("colz pol same");

  can->cd(5);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit4->Draw("colz pol same");

  can->cd(6);
  gPad->DrawFrame(0, -100, 150, 100);
  hsakra_hit5->Draw("colz pol same");

  return 0;
}
