{

  // ch 160 from run9132
  //tree->SetAlias("Egam"," pow( (rndm+madc.adc[160]),2)*-2.61926e-5 + (rndm+madc.adc[160])*1.39789 - 93.4553");

  // ch 161 from run9132
  //tree->SetAlias("Egam"," pow( (rndm+madc.adc[161]),2)*-3.94068e-5 + (rndm+madc.adc[161])*1.6587 -209.3");

  // ch 162 from run9132
  //tree->SetAlias("Egam"," pow( (rndm+madc.adc[162]),2)*-0.00018986 + (rndm+madc.adc[162])*3.03196 -1669.7");
 
  // ch 166 from run9132
  tree->SetAlias("Egam"," pow( (rndm+madc.adc[166]),2)*-1.45254e-5 + (rndm+madc.adc[160])*1.29969 - 93.0248 +60.");

      
  tree->Draw("Egam>>hE1s(4000,0,12000)","","");

  hE1s->Draw();

}
