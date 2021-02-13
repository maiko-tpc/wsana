#ifndef ANALYSIS_HPP_
#define ANALYSIS_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <signal.h>
#include <string.h>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "mk_hist.hpp"
#include "mtformat.h"
#include "moduledata.h"
//#include "evtdata.h"
#include "decoder.hpp"
#include "anagr.hpp"
#include "anassd.hpp"


class analysis{
public:
  analysis();
  ~analysis();

  void error_exit(TFile *file, int i);  
  int OpenBLDFile(char *bldfilename);
  void CloseBLDFile();  
  bool IsBLDeof();
  void AnaRunHeader();
  void strncpy_uchida(char *dst, char *src, int max_len);
  int  AnaBlk();
  int  AnaEvt();
  int  AnaFld();
  void InitEvt();
  void MakeROOTFile(std::string);
  void CloseROOTFile();  
  TFile* GetFile();
  int GetRunNum();
  int GetEveNum();

  // defined in histdef.cpp
  void HistDef();  
  void HistWrite();
  void HistFill();  

  // defined in treedef.cpp
  void TreeDef();
  void TreeWrite();
  
private:
  std::ifstream bldfile;
  //  int flag;
  evtdata evt;
  TFile *outfile;
  TTree *tree;

  /* Definition of output histograms */
  TH1F *hwire[N_VDCPLANE];
  TH1F *hdrifttime[N_VDCPLANE];
  
  char comment[100000];

  anagr *gr;
  anassd *ssd;  
};

#endif
