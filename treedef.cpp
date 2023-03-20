#include "analysis.hpp"

void analysis::TreeDef(){
  tree = new TTree("tree", "tree");

  tree->Branch("run",  &evt.run, "run/I");
  tree->Branch("blk",  &evt.blk, "blk/I");
  tree->Branch("eve",  &evt.eve, "eve/I");
  tree->Branch("unixtime",  &evt.unixtime, "unixtime/i");
  tree->Branch("unixtimesub",  &evt.unixtimesub, "unixtimesub/i");  
  tree->Branch("camac_inp", &evt.camac_inp, Form("camac_inp[%d]/I", N_INP));
  tree->Branch("vme_inp", &evt.vme_inp, Form("vme_inp[%d]/I", N_INP));  
  tree->Branch("camac_sca", &evt.camac_sca, Form("camac_sca[%d]/l", CAMAC_SCA_CH));
  tree->Branch("vme_sca", &evt.vme_sca, Form("vme_sca[%d]/l", VME_SCA_CH));  
  tree->Branch("v1190_hit_ref", evt.v1190_hit_ref,
	       Form("v1190_hit_ref[%d]/I", V1190_MAX_GEO));
  
#ifdef ANASSD
  tree->Branch("madc", &evt.madc,
  	       Form("wrdcnt[%d]/I:adc[%d]/I:counter[%d]/I",
		    N_MADC, N_MADC_CH, N_MADC));

  tree->Branch("v1190_ssd", &evt.v1190_ssd,
	       Form("hit[%d]/I:multi[%d]/I:tdc_raw[%d]/I:tdc_cor[%d]/I",
		    SSD_TDC_CH, SSD_TDC_CH, SSD_TDC_CH, SSD_TDC_CH));
  tree->Branch("rf_ssd", &evt.rf_ssd, Form("rf_ssd[%d]/I", MAX_RF_MULTI));
  tree->Branch("ssd_ene", &evt.ssd_ene, "ssd_ene[96]/F");

  tree->Branch("ssd_clst_multi", &evt.ssd_clst_multi,
	       Form("ssd_clst_multi[%d]/I", N_SSD_CLST));  
  tree->Branch("ssd_clst_ene", &evt.ssd_clst_ene,
	       Form("ssd_clst_ene[%d]/F", N_SSD_CLST));  
  tree->Branch("ssd_clst_tdc", &evt.ssd_clst_tdc,
	       Form("ssd_clst_tdc[%d]/I", N_SSD_CLST));  
#endif
  
  tree->Branch("grpla", &evt.grpla,
	       Form("adc[%d]/I:tdc[%d]/I:pos[%d]/F:de[%d]/F:fqdc[%d]/I:ftdc[%d]/I:vqdc[%d]/I:vtdc[%d]/I",
		    N_PLA_CH, N_PLA_CH, N_GRPLA, N_GRPLA,
		    N_PLA_CH, N_PLA_CH, N_PLA_CH, N_PLA_CH));

  tree->Branch("laspla", &evt.laspla,
	       Form("adc[%d]/I:tdc[%d]/I:pos[%d]/F:de[%d]/F:fqdc[%d]/I:ftdc[%d]/I:vqdc[%d]/I:vtdc[%d]/I",
		    N_PLA_CH, N_PLA_CH, N_GRPLA, N_GRPLA,
		    N_PLA_CH, N_PLA_CH, N_PLA_CH, N_PLA_CH));
  
  tree->Branch("vdcnhit", &evt.nhit_plane, Form("vdcnhit[%d]/I", N_VDCPLANE));
  tree->Branch("meanwire", &evt.mean_wire, Form("vdcwire[%d]/F", N_VDCPLANE));

  tree->Branch("nclst", &evt.nclst, Form("nclst[%d]/I", N_VDCPLANE));
  tree->Branch("clst_size", &evt.clst_size, Form("clst_size[%d]/I", N_VDCPLANE));  
  tree->Branch("gr_good_clst", &evt.gr_good_clst, "gr_good_clst/I");
  tree->Branch("good_fit", &evt.good_fit, "good_fit/I");  

  tree->Branch("redchi2", &evt.redchi2, Form("redchi2[%d]/F", N_VDCPLANE));
  tree->Branch("wire_pos", &evt.wire_pos, Form("wire_pos[%d]/F", N_VDCPLANE));
  tree->Branch("residual", &evt.residual, Form("residual[%d]/F", N_VDCPLANE));        

  tree->Branch("grx", &evt.grx, "grx/F");
  tree->Branch("gry", &evt.gry, "gry/F");
  tree->Branch("grthx", &evt.grthx, "grthx/F");
  tree->Branch("grthy", &evt.grthy, "grthy/F");

  tree->Branch("grp_rela", &evt.grp_rela, "grp_rela/D");
  tree->Branch("grp", &evt.grp, "grp/D");
  tree->Branch("gre", &evt.gre, "gre/D");        

  //  tree->Branch("v1190pla_multi", &evt.v1190pla_multi, "v1190pla_multi[16]/I");

  // for E552 on 2023 Feb.
  tree->Branch("band_v1190_multi", &evt.band_v1190_multi, "band_v1190_multi[32]/I");
  tree->Branch("band_v1190_lead", &evt.band_v1190_lead, "band_v1190_lead[32]/I");  
  tree->Branch("mqdc_nhit", &evt.mqdc_nhit, "mqdc_nhit[2]/i");
  
} // end of void analysis::TreeDef(){

void analysis::TreeWrite(){
  tree->Write();
}

