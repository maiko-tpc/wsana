{
/* 
  tree->Draw("madc.adc[168]>>h1slg(8000,0,8000)","","");
  h1slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan168-run9225.C");

  tree->Draw("madc.adc[169]>>h2slg(8000,0,8000)","","");
  h2slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan169-run9225.C");

  tree->Draw("madc.adc[170]>>h3slg(8000,0,8000)","","");
  h3slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan170-run9225.C");

  tree->Draw("madc.adc[171]>>h4slg(8000,0,8000)","","");
  h4slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan171-run9225.C");

  tree->Draw("madc.adc[172]>>h5slg(8000,0,8000)","","");
  h5slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan172-run9225.C");

  tree->Draw("madc.adc[173]>>h6slg(8000,0,8000)","","");
  h6slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan173-run9225.C");

  tree->Draw("madc.adc[174]>>h7slg(8000,0,8000)","","");
  h7slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan174-run9225.C");

  tree->Draw("madc.adc[175]>>h8slg(8000,0,8000)","","");
  h8slg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan175-run9225.C");
*/  
  	
  tree->SetAlias("LaBrE7s","(rndm+madc.adc[174])*4.295-190.37");
  tree->SetAlias("LaBrE6s","(rndm+madc.adc[173])*4.18671-4.77");
  tree->SetAlias("LaBrE5s","(rndm+madc.adc[172])*4.49255-221.32");
  tree->SetAlias("LaBrE4s","(rndm+madc.adc[171])*4.17502-171.64");
  tree->SetAlias("LaBrE3s","(rndm+madc.adc[170])*3.96091-234.77");
  tree->SetAlias("LaBrE2s","(rndm+madc.adc[169])*4.46803-145.396");
  tree->SetAlias("LaBrE1s","(rndm+madc.adc[168])*3.75325-66.396");
  tree->SetAlias("LaBrE8s","(rndm+madc.adc[175])*2.-100.");
 
  tree->Draw("LaBrE1s>>hE1s(4000,0,40000)","","");
  tree->Draw("LaBrE2s>>hE2s(4000,0,40000)","","");
  tree->Draw("LaBrE3s>>hE3s(4000,0,40000)","","");
  tree->Draw("LaBrE4s>>hE4s(4000,0,40000)","","");
  tree->Draw("LaBrE5s>>hE5s(4000,0,40000)","","");
  tree->Draw("LaBrE6s>>hE6s(4000,0,40000)","","");
  tree->Draw("LaBrE7s>>hE7s(4000,0,40000)","","");
  tree->Draw("LaBrE8s>>hE8s(4000,0,40000)","","");

  hE2s->SetLineColor(2);
  hE3s->SetLineColor(3);
  hE4s->SetLineColor(4);
  hE5s->SetLineColor(6);
  hE6s->SetLineColor(7);
  hE7s->SetLineColor(8);
  hE8s->SetLineColor(9);

  hE1s->Draw();
  hE2s->Draw("same");
  hE3s->Draw("same");
  hE4s->Draw("same");
  hE5s->Draw("same");
  hE6s->Draw("same");
  hE7s->Draw("same");
  hE8s->Draw("same");

 TLegend* mylegend=new TLegend(0.7,0.5,0.95,0.7);
  mylegend->AddEntry(hE1s,"E1 slow","l");
  mylegend->AddEntry(hE2s,"E2 slow","l");
  mylegend->AddEntry(hE3s,"E3 slow","l");
  mylegend->AddEntry(hE4s,"E4 slow","l");
  mylegend->AddEntry(hE5s,"E5 slow","l");
  mylegend->AddEntry(hE6s,"E6 slow","l");
  mylegend->AddEntry(hE7s,"E7 slow","l");
  mylegend->AddEntry(hE8s,"E8 slow","l");
  mylegend->SetBorderSize(0);
  mylegend->SetTextSize(0.02);
  mylegend->SetLineWidth(2);
  mylegend->Draw();
  

  
}
