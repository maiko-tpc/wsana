#include "mk_hist.hpp"
#include <string>

mk_hist::mk_hist(){
}

mk_hist::~mk_hist(){
}

void mk_hist::MakeFile(std::string outfname){
  outfile = new TFile(outfname.c_str(), "RECREATE");
}

TFile* mk_hist::GetFile(){
  return outfile;
}

void mk_hist::TreeDef(){
  tree = new TTree("tree", "tree");

  tree->Branch("eve",  &evt->eve, "eve/I");

  //  tree->Branch("madc", &(evt->madc),
  //	       Form("wrdcnt[%d]/I:adc[%d]/I:counter[%d]/I",
  //		    N_MADC, N_MADC_CH, N_MADC));

//  tree->Branch("v1190_ssd", &evt->v1190_ssd,
//	       Form("lead[%d][%d]/I:trail[%d][%d]/I:tot[%d][%d]/I:multi[%d]/I:counter/I",
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH));
//  
//  tree->Branch("grpla", &evt->grpla,
//	       Form("adc[%d]/I:tdc[%d]/I:pos[%d]/F:de[%d]/F",
//		    N_GRPLA_CH, N_GRPLA_CH, N_GRPLA, N_GRPLA));
//
//  tree->Branch("vdcnhit", &evt->nhit_plane, Form("vdcnhit[%d]/I", N_VDCPLANE));
//  tree->Branch("meanwire", &evt->mean_wire, Form("vdcwire[%d]/F", N_VDCPLANE));

}

void mk_hist::HistDef(){
  for(int i=0; i<N_VDCPLANE; i++){
    hwire[i] = new TH1F(Form("h%d", 1000+i), Form("VDC plane %d hit", i),
                        256, 0, 256);
    hdrifttime[i] = new TH1F(Form("h%d", 1010+i), Form("VDC plane %d drift time", i),
                             5000, 0, 5000);
  }
}

void mk_hist::CopyEvtData(evtdata evtin){
  //  evt = evtin;
}

void mk_hist::TreeFill(evtdata *evtin){
  evt = evtin;
  tree->Fill();
}

void mk_hist::TreeWrite(evtdata evtin){
  tree->Write();
}

