#include "analysis.hpp"

void analysis::TreeDef(){
  tree = new TTree("tree", "tree");

  tree->Branch("run",  &evt.run, "run/I");
  tree->Branch("blk",  &evt.blk, "blk/I");
  tree->Branch("eve",  &evt.eve, "eve/I");

  tree->Branch("madc", &evt.madc,
  	       Form("wrdcnt[%d]/I:adc[%d]/I:counter[%d]/I",
		    N_MADC, N_MADC_CH, N_MADC));

  tree->Branch("v1190_ssd", &evt.v1190_ssd,
	       Form("lead[%d][%d]/I:trail[%d][%d]/I:tot[%d][%d]/I:multi[%d]/I:counter/I",
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH));
  
  tree->Branch("grpla", &evt.grpla,
	       Form("adc[%d]/I:tdc[%d]/I:pos[%d]/F:de[%d]/F",
		    N_GRPLA_CH, N_GRPLA_CH, N_GRPLA, N_GRPLA));

  tree->Branch("vdcnhit", &evt.nhit_plane, Form("vdcnhit[%d]/I", N_VDCPLANE));
  tree->Branch("meanwire", &evt.mean_wire, Form("vdcwire[%d]/F", N_VDCPLANE));

  tree->Branch("nclst", &evt.nclst, Form("nclst[%d]/I", N_VDCPLANE));
}

void analysis::TreeWrite(){
  tree->Write();
}

