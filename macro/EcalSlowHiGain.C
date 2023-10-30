{
/*   
  tree->Draw("madc.adc[160]>>h1shg(8000,0,8000)","","");
  h1shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan160-run9225.C");

  tree->Draw("madc.adc[161]>>h2shg(8000,0,8000)","","");
  h2shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan161-run9225.C");

  tree->Draw("madc.adc[162]>>h3shg(8000,0,8000)","","");
  h3shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan162-run9225.C");

  tree->Draw("madc.adc[163]>>h4shg(8000,0,8000)","","");
  h4shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan163-run9225.C");

  tree->Draw("madc.adc[164]>>h5shg(8000,0,8000)","","");
  h5shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan164-run9225.C");

  tree->Draw("madc.adc[165]>>h6shg(8000,0,8000)","","");
  h6shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan165-run9225.C");

  tree->Draw("madc.adc[166]>>h7shg(8000,0,8000)","","");
  h7shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan166-run9225.C");

  tree->Draw("madc.adc[167]>>h8shg(8000,0,8000)","","");
  h8shg->SaveAs("/home/pandora/ana/furuno/labr/madc-chan167-run9225.C");
*/  
  	
  tree->SetAlias("LaBrE1s","(rndm+madc.adc[160])*1.206+1");
  tree->SetAlias("LaBrE2s","(rndm+madc.adc[161])*1.480-34.6");
  tree->SetAlias("LaBrE3s","(rndm+madc.adc[162])*1.156+686.");
  tree->SetAlias("LaBrE4s","(rndm+madc.adc[163])*1.290+36.27");
  tree->SetAlias("LaBrE5s","(rndm+madc.adc[164])*1.274+58.70");
  tree->SetAlias("LaBrE6s","(rndm+madc.adc[165])*1.236-11.18");
  tree->SetAlias("LaBrE7s","(rndm+madc.adc[166])*1.279+39.78");
  tree->SetAlias("LaBrE8s","(rndm+madc.adc[167])*1.326-24.89");
 
  tree->Draw("LaBrE1s>>hE1s(3000,0,30000)","","");
  tree->Draw("LaBrE2s>>hE2s(3000,0,30000)","","");
  tree->Draw("LaBrE3s>>hE3s(3000,0,30000)","","");
  tree->Draw("LaBrE4s>>hE4s(3000,0,30000)","","");
  tree->Draw("LaBrE5s>>hE5s(3000,0,30000)","","");
  tree->Draw("LaBrE6s>>hE6s(3000,0,30000)","","");
  tree->Draw("LaBrE7s>>hE7s(3000,0,30000)","","");
  tree->Draw("LaBrE8s>>hE8s(3000,0,30000)","","");

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
