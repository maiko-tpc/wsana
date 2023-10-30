{

  tree->Draw("madc.adc[176]>>h1s(8000,0,8000)","","");
  h1s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan176-run9136.C");

  tree->Draw("madc.adc[177]>>h2s(8000,0,8000)","","");
  h2s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan177-run9136.C");

  tree->Draw("madc.adc[178]>>h3s(8000,0,8000)","","");
  h3s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan178-run9136.C");

  tree->Draw("madc.adc[179]>>h4s(8000,0,8000)","","");
  h4s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan179-run9136.C");

  tree->Draw("madc.adc[180]>>h5s(8000,0,8000)","","");
  h5s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan180-run9136.C");

  tree->Draw("madc.adc[181]>>h6s(8000,0,8000)","","");
  h6s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan181-run9136.C");

  tree->Draw("madc.adc[182]>>h7s(8000,0,8000)","","");
  h7s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan182-run9136.C");

  tree->Draw("madc.adc[183]>>h8s(8000,0,8000)","","");
  h8s->SaveAs("/home/pandora/ana/furuno/labr/madc-chan183-run9136.C");

  
/*
  tree->Draw("madc.adc[176]>>h1f(8000,0,8000)","","");
  tree->Draw("madc.adc[177]>>h2f(8000,0,8000)","","");
  tree->Draw("madc.adc[178]>>h3f(8000,0,8000)","","");
  tree->Draw("madc.adc[179]>>h4f(8000,0,8000)","","");
  tree->Draw("madc.adc[180]>>h5f(8000,0,8000)","","");
  tree->Draw("madc.adc[181]>>h6f(8000,0,8000)","","");
  tree->Draw("madc.adc[182]>>h7f(8000,0,8000)","","");
  tree->Draw("madc.adc[183]>>h8f(8000,0,8000)","","");
*/


/*  
  tree->SetAlias("LaBrE1f","(rndm+madc.adc[176])*1.3-42.4");
  tree->SetAlias("LaBrE2f","(rndm+madc.adc[177])*1.6-48.3");
  tree->SetAlias("LaBrE3f","(rndm+madc.adc[178])*2.1-10.1");
  tree->SetAlias("LaBrE4f","(rndm+madc.adc[179])*1.3-22.0");
  tree->SetAlias("LaBrE5f","(rndm+madc.adc[180])*1.6-55.3");
  tree->SetAlias("LaBrE6f","(rndm+madc.adc[181])*1.6-44.9");
  tree->SetAlias("LaBrE7f","(rndm+madc.adc[182])*1.5-46.0");
  tree->SetAlias("LaBrE8f","(rndm+madc.adc[183])*1.5-45.5");
 
  tree->Draw("LaBrE1f>>hE1f(3000,0,30000)","","");
  tree->Draw("LaBrE2f>>hE2f(3000,0,30000)","","");
  tree->Draw("LaBrE3f>>hE3f(3000,0,30000)","","");
  tree->Draw("LaBrE4f>>hE4f(3000,0,30000)","","");
  tree->Draw("LaBrE5f>>hE5f(3000,0,30000)","","");
  tree->Draw("LaBrE6f>>hE6f(3000,0,30000)","","");
  tree->Draw("LaBrE7f>>hE7f(3000,0,30000)","","");
  tree->Draw("LaBrE8f>>hE8f(3000,0,30000)","","");

  hE2f->SetLineColor(2);
  hE3f->SetLineColor(3);
  hE4f->SetLineColor(4);
  hE5f->SetLineColor(6);
  hE6f->SetLineColor(7);
  hE7f->SetLineColor(8);
  hE8f->SetLineColor(9);

  hE1f->Draw();
  hE2f->Draw("same");
  hE3f->Draw("same");
  hE4f->Draw("same");
  hE5f->Draw("same");
  hE6f->Draw("same");
  hE7f->Draw("same");
  hE8f->Draw("same");

*/
}
