#ifndef ANALYSIS_HPP_
#define ANALYSIS_HPP_

#define ANASSD   // comment out to disable ssd analysis

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <signal.h>
#include <string.h>
#include <vector>
#include <ncurses.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <THttpServer.h>
#include <TInterpreter.h>

//#include "mk_hist.hpp"
#include "mtformat.h"
#include "moduledata.h"
//#include "evtdata.h"
#include "decoder.hpp"
#include "anapla.hpp"
#include "anagr.hpp"
#include "anassd.hpp"
#include "kinema.hpp"

#include "config.h"

class analysis{
public:
  analysis();
  ~analysis();

  void error_exit(TFile *file, int i);  
  int OpenBLDFile();
  void CloseBLDFile();  
  bool IsBLDeof();
  void ClearBLDError();
  void AnaRunHeader();
  void strncpy_uchida(char *dst, char *src, int max_len);
  int  AnaBlk();
  int  AnaEvt();
  int  AnaFld();
  void InitEvt();
  void MakeROOTFile();
  void CloseROOTFile();  
  void MakeTHttp(int portnum);
  void CloseTHttp();  
  TFile* GetFile();
  int GetRunNum();
  int GetEveNum();
  int GetBlkNum();  
  void SetBLDFile(char *fname);
  char* GetBLDFile();  
  void SetROOTFile(char *fname);
  char* GetROOTFile();  
  void SetUseage();
  void UnsetUseage();
  int GetUseage();    
  void SetOnline();
  int GetWeb();  
  void SetWeb();
  int GetOnline();  
  void ShowCommandOption();
  void ShowCamacSca();
  void ClearCamacSca();
  void SetParFile(char *fname);
  void ShowGREff();
  
  // defined in histdef.cpp
  void HistDef();  
  void HistWrite();
  void HistFill();  

  // defined in treedef.cpp
  void TreeDef();
  void TreeWrite();

  // Parameter analysis functions
  int AnaParFile();
  void read_config_file(char *config_file_name);
  void GetTargetPart();
  
  void SetKinema();

  void CalcGREff();

  void ShowProg();
  
  void AnaV1190Ref();

  void AnaV1190inpreg();  

private:
  std::ifstream bldfile;

  optdata opt;
  evtdata evt;
  pardata par;
  TFile *outfile;
  TTree *tree;

  THttpServer* serv;
  
  /* Definition of output histograms */
  TH1F *hwire[N_VDCPLANE];
  TH1F *hdrifttime[N_VDCPLANE];
  TH1F *hdriftlen[N_VDCPLANE];
  TH1F *hhiteff[N_VDCPLANE];
  TH1F *hclsteff[N_VDCPLANE];    
  TH1F* hhiteffall;
  TH1F* hclsteffall;  
  
  TH1F *hgrfqdc[4];
  TH1F *hgrvqdc[4];

  TH1F *hlasfqdc[4];
  TH1F *hlasvqdc[4];

  TH2F *hgrqdccor[4];
  TH2F *hlasqdccor[4];  

  TH2F* hv1190tdc;
  
  TH2F* hmadc_raw;
  TH2F* hmadc_tdc;
  TH2F* hmadc_ene;    

#ifdef ANASSD
  TH2F* hsakra_hit[6];
#endif
  //
  
  char comment[100000];

  anapla *pla;
  anagr *gr;
  anassd *ssd;  
  kinema *kine;
  
  int par_debug_mode;
  
public:

};

#endif
