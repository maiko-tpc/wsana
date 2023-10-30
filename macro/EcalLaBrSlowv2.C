{

  tree->Draw("madc.adc[168]>>h1s2(8000,0,8000)","","");
  h1s->SaveAs("madc-chan168-run9133.C");

  tree->Draw("madc.adc[169]>>h2s2(8000,0,8000)","","");
  h2s->SaveAs("madc-chan169-run9133.C");

  tree->Draw("madc.adc[170]>>h3s2(8000,0,8000)","","");
  h3s->SaveAs("madc-chan170-run9133.C");

  tree->Draw("madc.adc[171]>>h4s2(8000,0,8000)","","");
  h4s->SaveAs("madc-chan171-run9133.C");

  tree->Draw("madc.adc[172]>>h5s2(8000,0,8000)","","");
  h5s->SaveAs("madc-chan172-run9133.C");

  tree->Draw("madc.adc[173]>>h6s2(8000,0,8000)","","");
  h6s->SaveAs("madc-chan173-run9133.C");

  tree->Draw("madc.adc[174]>>h7s2(8000,0,8000)","","");
  h7s->SaveAs("madc-chan174-run9133.C");

  tree->Draw("madc.adc[175]>>h8s2(8000,0,8000)","","");
  h8s->SaveAs("madc-chan175-run9133.C")
    
  /*
  tree->Draw("madc.adc[168]>>h1s2(8000,0,8000)","","");
  tree->Draw("madc.adc[169]>>h2s2(8000,0,8000)","","");
  tree->Draw("madc.adc[170]>>h3s2(8000,0,8000)","","");
  tree->Draw("madc.adc[171]>>h4s2(8000,0,8000)","","");
  tree->Draw("madc.adc[172]>>h5s2(8000,0,8000)","","");
  tree->Draw("madc.adc[173]>>h6s2(8000,0,8000)","","");
  tree->Draw("madc.adc[174]>>h7s2(8000,0,8000)","","");
  tree->Draw("madc.adc[175]>>h8s2(8000,0,8000)","","");
*/
/*	
  tree->SetAlias("LaBrE1s2","(rndm+madc.adc[168])*2.6-85.0");
  tree->SetAlias("LaBrE2s2","(rndm+madc.adc[169])*3.0-92.5");
  tree->SetAlias("LaBrE3s2","(rndm+madc.adc[170])*3.9-110.7");
  tree->SetAlias("LaBrE4s2","(rndm+madc.adc[171])*2.8-93.0");
  tree->SetAlias("LaBrE5s2","(rndm+madc.adc[172])*3.0-98.8");
  tree->SetAlias("LaBrE6s2","(rndm+madc.adc[173])*3.0-86.6");
  tree->SetAlias("LaBrE7s2","(rndm+madc.adc[174])*2.0-98.2");
  tree->SetAlias("LaBrE8s2","(rndm+madc.adc[175])*1.-1.");   // bad labpro channel
 
  tree->Draw("LaBrE1s2>>hE1s2(3000,0,30000)","","");
  tree->Draw("LaBrE2s2>>hE2s2(3000,0,30000)","","");
  tree->Draw("LaBrE3s2>>hE3s2(3000,0,30000)","","");
  tree->Draw("LaBrE4s2>>hE4s2(3000,0,30000)","","");
  tree->Draw("LaBrE5s2>>hE5s2(3000,0,30000)","","");
  tree->Draw("LaBrE6s2>>hE6s2(3000,0,30000)","","");
  tree->Draw("LaBrE7s2>>hE7s2(3000,0,30000)","","");
  tree->Draw("LaBrE8s2>>hE8s2(3000,0,30000)","","");

  hE2s2->SetLineColor(2);
  hE3s2->SetLineColor(3);
  hE4s2->SetLineColor(4);
  hE5s2->SetLineColor(6);
  hE6s2->SetLineColor(7);
  hE7s2->SetLineColor(8);
  hE8s2->SetLineColor(9);

  hE1s2->Draw();
  hE2s2->Draw("same");
  hE3s2->Draw("same");
  hE4s2->Draw("same");
  hE5s2->Draw("same");
  hE6s2->Draw("same");
  hE7s2->Draw("same");
  hE8s2->Draw("same");


  TLegend* mylegend=new TLegend(0.7,0.5,0.95,0.7);
  mylegend->AddEntry(hE1s2,"E1 slow v2","l");
  mylegend->AddEntry(hE2s2,"E2 slow v2","l");
  mylegend->AddEntry(hE3s2,"E3 slow v2","l");
  mylegend->AddEntry(hE4s2,"E4 slow v2","l");
  mylegend->AddEntry(hE5s2,"E5 slow v2","l");
  mylegend->AddEntry(hE6s2,"E6 slow v2","l");
  mylegend->AddEntry(hE7s2,"E7 slow v2","l");
  mylegend->AddEntry(hE8s2,"E8 slow v2","l");
  mylegend->SetBorderSize(0);
  mylegend->SetTextSize(0.02);
  mylegend->SetLineWidth(2);
  mylegend->Draw();
*/
  
}
