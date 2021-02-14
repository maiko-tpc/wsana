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

