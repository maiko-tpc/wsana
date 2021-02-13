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
#include "mk_hist.hpp"
#include "vdc.h"
#include "analysis.hpp"

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
  
  /* Print how to use */
  if (iarg != 3) {
    printf("usage: ./ana xxxx.bld xxxx.root\n");
    printf("eg.)\n");
    printf("$ ./ana run7144.bld run7144.root\n");
    exit(0);
  }
  

  /* main analysis frame */
  analysis *ana = new analysis();

  /* Open .bld file */
  int bldres = ana->OpenBLDFile(argv[1]);
  if(bldres==1) exit(0);  // file not exist

  /* Make output ROOT file with tree and histograms */
  ana->MakeROOTFile(argv[2]);
  ana->TreeDef();
  ana->HistDef();
  
  ana->AnaRunHeader();

  /* Analyze blocks */
  unsigned int blkcnt=0;
  while(!(ana->IsBLDeof()) && !eflag){
    ana->AnaBlk();
    blkcnt++;
  }

  /* Save output ROOT */
  ana->TreeWrite();
  ana->HistWrite();
  ana->CloseROOTFile();

  ana->CloseBLDFile();

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
