#ifndef _MK_HIST_HPP_
#define _MK_HIST_HPP_

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "moduledata.h"
#include "evtdata.h"
//#include "analysis.hpp"

class mk_hist{

public:
  mk_hist();
  ~mk_hist();

  void MakeFile(std::string);
  TFile* GetFile();
  void HistDef();
  void TreeDef();
  void CopyEvtData(evtdata);
  void TreeFill(evtdata*);
  void TreeWrite(evtdata);
  //  void HistFill(evtdata);
  
private:
  evtdata *evt;
  TFile *outfile;

  /* Definition of tree */
  TTree *tree;
  
  /* Definition of output histograms */
  TH1F *hwire[N_VDCPLANE];
  TH1F *hdrifttime[N_VDCPLANE];

};

#endif
