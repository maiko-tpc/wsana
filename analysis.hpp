#ifndef ANALYSIS_HPP_
#define ANALYSIS_HPP_

//#define ANASSD   // comment out to disable ssd analysis

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
  int GetOnline();  
  void ShowCommandOption();
  void ShowCamacSca();
  void ClearCamacSca();
  
  // defined in histdef.cpp
  void HistDef();  
  void HistWrite();
  void HistFill();  

  // defined in treedef.cpp
  void TreeDef();
  void TreeWrite();

  // Parameter analysis functions
  void AnaPar();
  void read_config_file(char *config_file_name);
  void GetTargetPart();
  
private:
  std::ifstream bldfile;

  optdata opt;
  evtdata evt;
  TFile *outfile;
  TTree *tree;

  THttpServer* serv;
  
  /* Definition of output histograms */
  TH1F *hwire[N_VDCPLANE];
  TH1F *hdrifttime[N_VDCPLANE];
  TH1F *hdriftlen[N_VDCPLANE];  
  
  char comment[100000];

  anapla *pla;
  anagr *gr;
  anassd *ssd;  

  int par_debug_mode;
  
public:

};

#endif
