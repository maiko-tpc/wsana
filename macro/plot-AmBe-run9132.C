{
/*

.x labr/madc-chan168-run9132.C 
.x labr/madc-chan169-run9132.C 
.x labr/madc-chan170-run9132.C 
.x labr/madc-chan171-run9132.C
.x labr/madc-chan172-run9132.C 
.x labr/madc-chan173-run9132.C 
.x labr/madc-chan174-run9132.C 
.x labr/madc-chan175-run9132.C


*/

  h1s2__9->SetTitle("Slow E signals, low gain");
  
  
  h1s2__9->SetStats(0);
  h2s2__10->SetStats(0);
  h3s2__11->SetStats(0);
  h4s2__12->SetStats(0);
  h5s2__13->SetStats(0);
  h6s2__14->SetStats(0);
  h7s2__15->SetStats(0);
  h8s2__16->SetStats(0);

  h1s2__9->SetLineColor(1);
  h2s2__10->SetLineColor(2);
  h3s2__11->SetLineColor(3);
  h4s2__12->SetLineColor(4);
  h5s2__13->SetLineColor(28);
  h6s2__14->SetLineColor(6);
  h7s2__15->SetLineColor(7);
  h8s2__16->SetLineColor(8);

  h1s2__9->Draw();
  h2s2__10->Draw("same");
  h3s2__11->Draw("same");
  h4s2__12->Draw("same");
  h5s2__13->Draw("same");
  h6s2__14->Draw("same");
  h7s2__15->Draw("same");
  h8s2__16->Draw("same");

  
  h1s2__9->GetXaxis()->SetRange(100,8000);

  TLegend* mylegend=new TLegend(0.6,0.7,0.95,0.9);
  mylegend->AddEntry(h1s2__9,"ch168 labr1 slow hi gain","l");
  mylegend->AddEntry(h2s2__10,"ch169 labr2 slow hi gain","l");
  mylegend->AddEntry(h3s2__11,"ch170 labr3 slow hi gain","l");
  mylegend->AddEntry(h4s2__12,"ch171 labr4 slow hi gain","l");
  mylegend->AddEntry(h5s2__13,"ch172 labr5 slow hi gain","l");
  mylegend->AddEntry(h6s2__14,"ch173 labr6 slow hi gain","l");
  mylegend->AddEntry(h7s2__15,"ch174 labr7 slow hi gain","l");
  mylegend->AddEntry(h8s2__16,"ch175 labr8 slow hi gain","l");
  mylegend->SetBorderSize(0);
  mylegend->SetTextSize(0.03);
  mylegend->SetLineWidth(4);
  mylegend->Draw();

}
