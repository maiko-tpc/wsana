#include "analysis.hpp"

void analysis::TreeDef(){
  tree = new TTree("tree", "tree");

  tree->Branch("run",  &evt.run, "run/I");
  tree->Branch("blk",  &evt.blk, "blk/I");
  tree->Branch("eve",  &evt.eve, "eve/I");

#ifdef ANASSD
  tree->Branch("madc", &evt.madc,
  	       Form("wrdcnt[%d]/I:adc[%d]/I:counter[%d]/I",
		    N_MADC, N_MADC_CH, N_MADC));

  tree->Branch("v1190_ssd", &evt.v1190_ssd,
	       Form("lead[%d][%d]/I:trail[%d][%d]/I:tot[%d][%d]/I:multi[%d]/I:counter/I",
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH));
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

  tree->Branch("grx", &evt.grx, "grx/F");
  tree->Branch("gry", &evt.gry, "gry/F");
  tree->Branch("grthx", &evt.grthx, "grthx/F");
  tree->Branch("grthy", &evt.grthy, "grthy/F");  
}

void analysis::TreeWrite(){
  tree->Write();
}

