{

  tree->Draw("madc.adc[184]>>h1f(8000,0,8000)","","");
  h1s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan184-run9132.C");

  tree->Draw("madc.adc[185]>>h2f(8000,0,8000)","","");
  h2s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan185-run9132.C");

  tree->Draw("madc.adc[186]>>h3f(8000,0,8000)","","");
  h3s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan186-run9132.C");

  tree->Draw("madc.adc[187]>>h4f(8000,0,8000)","","");
  h4s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan187-run9132.C");

  tree->Draw("madc.adc[188]>>h5f(8000,0,8000)","","");
  h5s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan188-run9132.C");

  tree->Draw("madc.adc[189]>>h6f(8000,0,8000)","","");
  h6s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan189-run9132.C");

  tree->Draw("madc.adc[190]>>h7f(8000,0,8000)","","");
  h7s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan190-run9132.C");

  tree->Draw("madc.adc[191]>>h8f(8000,0,8000)","","");
  h8s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan191-run9132.C");

  
/*
  tree->Draw("madc.adc[184]>>h1f(8000,0,8000)","","");
  tree->Draw("madc.adc[185]>>h2f(8000,0,8000)","","");
  tree->Draw("madc.adc[186]>>h3f(8000,0,8000)","","");
  tree->Draw("madc.adc[187]>>h4f(8000,0,8000)","","");
  tree->Draw("madc.adc[188]>>h5f(8000,0,8000)","","");
  tree->Draw("madc.adc[189]>>h6f(8000,0,8000)","","");
  tree->Draw("madc.adc[190]>>h7f(8000,0,8000)","","");
  tree->Draw("madc.adc[191]>>h8f(8000,0,8000)","","");
*/


/*  
  tree->SetAlias("LaBrE1f2","(rndm+madc.adc[176])*1.3-42.4");
  tree->SetAlias("LaBrE2f2","(rndm+madc.adc[177])*1.6-48.3");
  tree->SetAlias("LaBrE3f2","(rndm+madc.adc[178])*2.1-10.1");
  tree->SetAlias("LaBrE4f2","(rndm+madc.adc[179])*1.3-22.0");
  tree->SetAlias("LaBrE5f2","(rndm+madc.adc[180])*1.6-55.3");
  tree->SetAlias("LaBrE6f2","(rndm+madc.adc[181])*1.6-44.9");
  tree->SetAlias("LaBrE7f2","(rndm+madc.adc[182])*1.5-46.0");
  tree->SetAlias("LaBrE8f2","(rndm+madc.adc[183])*1.5-45.5");
 
  tree->Draw("LaBrE1f2>>hE1f2(3000,0,30000)","","");
  tree->Draw("LaBrE2f2>>hE2f2(3000,0,30000)","","");
  tree->Draw("LaBrE3f2>>hE3f2(3000,0,30000)","","");
  tree->Draw("LaBrE4f2>>hE4f2(3000,0,30000)","","");
  tree->Draw("LaBrE5f2>>hE5f2(3000,0,30000)","","");
  tree->Draw("LaBrE6f2>>hE6f2(3000,0,30000)","","");
  tree->Draw("LaBrE7f2>>hE7f2(3000,0,30000)","","");
  tree->Draw("LaBrE8f2>>hE8f2(3000,0,30000)","","");

  hE2f2->SetLineColor(2);
  hE3f2->SetLineColor(3);
  hE4f2->SetLineColor(4);
  hE5f2->SetLineColor(6);
  hE6f2->SetLineColor(7);
  hE7f2->SetLineColor(8);
  hE8f2->SetLineColor(9);

  hE1f2->Draw();
  hE2f2->Draw("same");
  hE3f2->Draw("same");
  hE4f2->Draw("same");
  hE5f2->Draw("same");
  hE6f2->Draw("same");
  hE7f2->Draw("same");
  hE8f2->Draw("same");

*/
}
