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
#include <TH1F.h>
#include <TH2F.h>

#include "mtformat.h"
#include "moduledata.h"
#include "evtdata.h"
#include "decoder.hpp"
#include "analysis.hpp"
#include "config.h"

//#define DEBUG 

// signal handler
void abrt_handler(int sig, siginfo_t *info, void *ctx);
volatile sig_atomic_t eflag;  

int main(int iarg, char *argv[]) {
  
  /* define action when Cntl-c is given */
  struct sigaction sa_sigabrt;
  memset(&sa_sigabrt, 0, sizeof(sa_sigabrt));
  sa_sigabrt.sa_sigaction = abrt_handler;
  sa_sigabrt.sa_flags = SA_SIGINFO;

  if (sigaction(SIGINT, &sa_sigabrt, NULL) < 0 ) {
    exit(1);
  }

  /* main analysis frame */
  analysis *ana = new analysis();


  /* analyze the command option */
  for(int i=0; i<iarg; i++){
    // show help
    if(strstr(argv[i], "-h") != NULL){
      ana->SetUseage();
    }
    
    // input file
    if(strstr(argv[i], ".bld") != NULL){
      ana->SetBLDFile(argv[i]);
    }
    if(strstr(argv[i], ".bld") == NULL){
      ana->UnsetUseage();
    }

    // output file
    if(strstr(argv[i], ".root") != NULL){
      ana->SetROOTFile(argv[i]);
    }
    
    // online flag
    if(strstr(argv[i], "-online") != NULL){
      ana->SetOnline();
    }

    // online flag
    if(strstr(argv[i], "-online") != NULL){
      ana->SetOnline();
    }

    // parameter file name
    if(strstr(argv[i], "-par") != NULL){
      if(strstr(argv[i+1], ".par") == NULL){
	printf("Give the correct parameter file name: %s\n",
	       argv[i+1]);
	exit(0);	
      }
      ana->SetParFile(argv[i+1]);
    }
          
  }
  
  /* Print how to use */
  if(ana->GetUseage() || iarg==1) {
    printf("usage: ./ana xxxx.bld xxxx.root [options]\n");
    printf("Example: $> ./ana run7144.bld run7144.root\n");
    printf("Options: \n");
    printf(" -online: Online mode. When the program reaches at the eond of file, it will stand-by and wait for another data.\n");
    exit(0);
  }

  ana->ShowCommandOption();

  ana->AnaParFile();
  ana->SetKinema();
  
  /* Open .bld file */
  int bldres = ana->OpenBLDFile();
  if(bldres==1) exit(0);  // file not exist

  /* Make output ROOT file with tree and histograms */
  ana->MakeROOTFile();
  ana->TreeDef();
  ana->HistDef();
  
  ana->AnaRunHeader();

  /* Analyze blocks */
  unsigned int blkcnt=0;
//  while(!(ana->IsBLDeof()) && !eflag){
//    ana->AnaBlk();
//    blkcnt++;
//  }
  while(!eflag){
    if(!(ana->IsBLDeof())){
      ana->AnaBlk();
      blkcnt++;
    }
    if(ana->GetOnline() && ana->IsBLDeof()){
      printf("Waiting for data... (eve: %d)\n", ana->GetEveNum());
      ana->ClearBLDError();
      sleep(10);
    }
    if( ((ana->GetOnline())!=1) && ana->IsBLDeof()){
      break;
    }
  }
  
  /* Save output ROOT */
  ana->TreeWrite();
  ana->HistWrite();
  ana->CloseROOTFile();

  ana->CloseBLDFile();
  ana->ShowCamacSca();
  
  printf("\nAnalyzed %d events (%d blocks)\n", ana->GetEveNum(), blkcnt);
  
  return 0;
}


void abrt_handler(int sig, siginfo_t *info, void *ctx) {
  printf("\n");
  printf("Interrupt is detected!\n");
  printf("Saving output file...\n");
  printf("\n");
  eflag = 1;
}
