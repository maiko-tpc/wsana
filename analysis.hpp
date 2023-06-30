#ifndef ANALYSIS_HPP_
#define ANALYSIS_HPP_

//#define ANASSD 1  // comment out to disable ssd analysis
//#define ANALAS 1    // comment out to disable LAS analysis


#define SKIP_BLK_END 1

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
#include <time.h>

#include "TSystem.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TLatex.h>

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

  void SetPID(int pidnum);
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
  void HttpHistReset();
  void HttpHistFit();
  void HttpInfoUpdate();    
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
  void SetLast();
  void SetRefresh();    
  int GetWeb();  
  void SetWeb();
  int GetOnline();
  int GetLast();
  int GetRefresh();      
  void ShowCommandOption();
  void ShowCamacSca();
  void ShowVmeSca();  
  void ClearCamacSca();
  void ClearVmeSca();  
  void SetParFile(char *fname);
  void ShowGREff();
  int SeekLastBlk();
  
  // defined in histdef.cpp
  void HistDef();  
  void HistWrite();
  void HistFill();  
  void FillGRTrack();
  
  // defined in treedef.cpp
  void TreeDef();
  void TreeWrite();

  // Parameter analysis functions
  int AnaParFile();
  void read_config_file(char *config_file_name);
  void GetTargetPart();
  
  void SetKinema();

  void CalcGREff();
  void CalcLASEff();  

  void ShowProg();
  
  void AnaV1190Ref();

  void AnaV1190inpreg();  

  
private:
  int pid;

  std::ifstream bldfile;

  optdata opt;
  evtdata evt;
  pardata par;
  TFile *outfile;
  TTree *tree;

  THttpServer* serv;

  /* Definition of Canvas for http */
  TCanvas* cinfo;
  
  /* Definition of output histograms */
  TH1F *hwire[N_VDCPLANE];
  TH1F *hdrifttime[N_VDCPLANE];
  TH1F *hdrifttime_coin[N_VDCPLANE];  
  TH1F *hdriftlen[N_VDCPLANE];
  TH1F *hhiteff[N_VDCPLANE];
  TH1F *hclsteff[N_VDCPLANE];    
  TH1F* hhiteffall;
  TH1F* hclsteffall;  
  
  // LAS
  TH1F *hwire_las[N_VDCPLANE_LAS];
  TH1F *hdrifttime_las[N_VDCPLANE_LAS];  

  TH1F *hhiteff_las[N_VDCPLANE_LAS];
  TH1F *hclsteff_las[N_VDCPLANE_LAS];    
  TH1F* hhiteffall_las;
  TH1F* hclsteffall_las;  
  
  TH1F *hgrfqdc[N_PLA_CH];
  TH1F *hgrvqdc[N_PLA_CH];

  TH1F *hlasfqdc[N_PLA_CH];
  TH1F *hlasvqdc[N_PLA_CH];

  TH2F *hgrqdccor[N_PLA_CH];
  TH2F *hlasqdccor[N_PLA_CH];  

  TH2F *hmqdcglcor;
  
  TH2F* hv1190tdc;
  
  TH2F* hmadc_raw;
  TH2F* hmadc_tdc;
  TH2F* hmadc_ene;    

  TH1F *hgrrf[N_RF];

  TH1F *hgrlascoin;
  
  /* GR PLA PID */
  TH2F *hgrplaposq[4];
  TH2F *hgrplarfde[2];
  
  TH1F* hunixtimesub;
  
  TCanvas* fitcan;  // for web monitor

  TH1F* hgrx;
  TH1F* hgrx_thxgate;  
  TH1F* hgry;

  TH1F* hgrangx;
  TH1F* hgrangy;  
  
  TH2F* hgrthx;
  TH2F* hgryx;  
  
  TH1F* hgrrelap;

#ifdef ANASSD
  TH2F* hsakra_hit[6];
#endif

  TH2F* hgrtrackx;
  TH2F* hgrtracky;  


  //
  
  char comment[100000];

  anapla *pla;
  anagr *gr;
  anagr *las;  
  anassd *ssd;  
  kinema *kine;
  
  int par_debug_mode;
  
public:

};

#endif
