{

  // Correlation between GR MQDC, FERA QDC, and LAS MQDC
  TCanvas *c_qdc_cor = new TCanvas("c_qdc_cor", "QDC correlation for DAQ debug", 1200,800);
  c_qdc_cor->Divide(2,2);
  
  c_qdc_cor->cd(1);
  tree->Draw("grpla.vqdc[0]:grpla.fqdc[0]>>qdc_cor_h1(200,0,2000, 200,0,4000)","","colz");

  c_qdc_cor->cd(2);
  tree->Draw("grpla.vqdc[2]:grpla.fqdc[2]>>qdc_cor_h2(200,0,2000, 200,0,4000)","","colz");  

  c_qdc_cor->cd(3);
  tree->Draw("grpla.vqdc[0]:laspla.vqdc[10]>>qdc_cor_h3(200,0,4000, 200,0,4000)","","colz");

  c_qdc_cor->cd(4);
  tree->Draw("grpla.vqdc[1]:laspla.vqdc[21]>>qdc_cor_h4(200,0,4000, 200,0,4000)","","colz");  

  // Correlation between vmepla V1190, GR MQDC, and vmegr v1190
  TCanvas *c_tdc_cor = new TCanvas("c_tdc_cor", "TDC correlation for DAQ debug", 600,800);
  c_tdc_cor->Divide(1,2);

  c_tdc_cor->cd(1);
  tree->Draw("grpla.vqdc[1]:grpla.vtdc[0]-grpla.vtdc[1]>>tdc_cor_h1(150,-150,150, 400,0,4000)",
	     "", "colz");

  c_tdc_cor->cd(2);
  tree->Draw("meanwire[0]:vtdc[0]-grpla.vtdc[1]>>tdc_cor_h2(150,-150,150, 100,0,200)","","colz");
}
