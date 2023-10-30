{

// ALIASES -------------------------------------------	

  //Energy calib GR
  //tree->SetAlias("Ex","-0.02942*grx+19.59672"); //SINGLES at 6.6deg

  //tree->SetAlias("Ex","-0.02933*grx+18.35521"); //COINCIDENCE at 6.6deg
  //DATAChain->SetAlias("Ex","-0.02933*grx+18.35521"); //COINCIDENCE at 6.6deg

  tree->SetAlias("Ex","1.85+(-0.02933*grx+18.35521)+0.25*grthx+0.05*grthx*grthx"); //COINCIDENCE at 0.0deg
  //DATAChain->SetAlias("Ex","1.85+(-0.02933*grx+18.35521)+0.25*grthx+0.05*grthx*grthx"); //COINCIDENCE at 0.0deg

  tree->SetAlias("grthxCOR","grthx+0.12*Ex-0.2"); 
  //DATAChain->SetAlias("grthxCOR","grthx+0.12*Ex-0.2"); 

  tree->SetAlias("ExCOR","Ex+ 0.1*(grthx+0.12*Ex-1.2) + 0.05*(grthx+0.12*Ex-1.2)*(grthx+0.12*Ex-1.2)"); 
  //DATAChain->SetAlias("ExCOR","Ex+ 0.1*(grthx+0.12*Ex-1.2) + 0.05*(grthx+0.12*Ex-1.2)*(grthx+0.12*Ex-1.2)"); 
 
  //SAKRA
  //tree->SetAlias("sakratof","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-v1190_ssd.tdc_raw[122]-v1190_ssd.tdc_raw[120]"); 
  //DATAChain->SetAlias("sakratof","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-v1190_ssd.tdc_raw[122]-v1190_ssd.tdc_raw[120]"); 
  //DATAChain->SetAlias("sakratof","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0]"); 

  tree->SetAlias("sakratof1","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0]"); 
  //DATAChain->SetAlias("sakratof1","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0]"); 
  // gate of sakratof created as ub sakratof1 for 0 degr runs is -1000 to -400 for real, and -1600 to -1000 for random
  
  //SCILLA
  tree->SetAlias("labrtime","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[248]"); 
  //DATAChain->SetAlias("labrtime","v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[248]"); 




// GATES -----------------------------------------------
//GR
 TCut grgates = "grpidcut && good_fit"; //finite angles
 TCut grgates_zerodeg = "grpidcut && good_fit && grpla.de[0]>150 && grpla.de[0]<350 && grpla.de[1]>100 && grpla.de[1]<400"; // zerodegree

 TCut grthCUT = "grthxCOR>-3  && grthxCOR<4 ";
 TCut grYCUT = "gry<25 && gry>-30";


//SCILLA
TCut labr7_timereal = "(v1190_ssd.tdc_raw[230]-v1190_ssd.tdc_raw[248])>-3800 &&(v1190_ssd.tdc_raw[230]-v1190_ssd.tdc_raw[248])<-3600";
TCut labr6_timereal = "(v1190_ssd.tdc_raw[229]-v1190_ssd.tdc_raw[248])>-3800 &&(v1190_ssd.tdc_raw[229]-v1190_ssd.tdc_raw[248])<-3600";
TCut labr5_timereal = "(v1190_ssd.tdc_raw[228]-v1190_ssd.tdc_raw[248])>-3700 &&(v1190_ssd.tdc_raw[228]-v1190_ssd.tdc_raw[248])<-3520";
TCut labr4_timereal = "(v1190_ssd.tdc_raw[227]-v1190_ssd.tdc_raw[248])>-3860 &&(v1190_ssd.tdc_raw[227]-v1190_ssd.tdc_raw[248])<-3690";
TCut labr3_timereal = "(v1190_ssd.tdc_raw[226]-v1190_ssd.tdc_raw[248])>-3800 &&(v1190_ssd.tdc_raw[226]-v1190_ssd.tdc_raw[248])<-3570";
TCut labr2_timereal = "(v1190_ssd.tdc_raw[225]-v1190_ssd.tdc_raw[248])>-3820 &&(v1190_ssd.tdc_raw[225]-v1190_ssd.tdc_raw[248])<-3630";
TCut labr1_timereal = "(v1190_ssd.tdc_raw[224]-v1190_ssd.tdc_raw[248])>-3810 &&(v1190_ssd.tdc_raw[224]-v1190_ssd.tdc_raw[248])<-3600";
TCut labr0_timereal = "(v1190_ssd.tdc_raw[223]-v1190_ssd.tdc_raw[248])>-3820 && (v1190_ssd.tdc_raw[223]-v1190_ssd.tdc_raw[248])<-3620";
TCut labr_timereal = labr7_timereal || labr6_timereal || labr5_timereal || labr4_timereal || labr3_timereal || labr2_timereal || labr1_timereal || labr0_timereal   ;


 TCut labrall_TDC_lg = "Iteration$>223&&Iteration$<232";
 TCut labrall_lg = "Iteration$>167&&Iteration$<176";
 TCut labr1_lg = "Iteration$==168";
 TCut labr2_lg = "Iteration$==169";
 TCut labr3_lg = "Iteration$==170";
 TCut labr4_lg = "Iteration$==171";
 TCut labr5_lg = "Iteration$==172";
 TCut labr6_lg = "Iteration$==173";
 TCut labr7_lg = "Iteration$==174";
 TCut labr8_lg = "Iteration$==175";

 //SAKRA: only valid for v1190_ssd_mod, not v1190_ssd, which has channels aligned with the ADC chan nrs
 TCut sakra_s1f = "Iteration$<16";
 TCut sakra_s2f = "Iteration$>15&&Iteration$<32";
 TCut sakra_s3f = "Iteration$>31&&Iteration$<48";
 TCut sakra_s4f = "Iteration$>47&&Iteration$<64";
 TCut sakra_s5f = "Iteration$>63&&Iteration$<80";
 TCut sakra_s1to5f = "Iteration$<80";

 //rf peak 
 TCut rf0peak1 = "rf[0]>1250&&rf[0]<1450";
 TCut rf0peak2 = "rf[0]>1550&&rf[0]<1750";

//when selecting rf0peak1; for zerodegr
TCut sakratofreal_pk1Z = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-900 &&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<-400";
TCut sakratofrandom_pk1Z = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-1800 &&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<-1400";
//when selecting rf0peak2; for zerodegr
TCut sakratofreal_pk2Z = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-1200 &&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<-800";
TCut sakratofrandom_pkZ = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-1600 &&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<-1100";

//when selecting rf0peak1; for finite angles
TCut sakratofreal_pk1 = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-1000&&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<0";
TCut sakratofrandom_pk1 = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>200&&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<1200";
//when selecting rf0peak2 for finite angles
TCut sakratofreal_pk2 = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>-1200&&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<-200";
TCut sakratofrandom_pk2 = "(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])>0&&(v1190_ssd.tdc_raw[]-v1190_ssd.tdc_raw[120]-rf[0])<1000";


}
