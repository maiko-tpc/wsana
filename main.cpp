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
#include <THttpServer.h>
#include "TSystem.h"

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
  int portnum=8080;
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

    // last flag
    if(strstr(argv[i], "-last") != NULL){
      ana->SetLast();
    }
    
    // THttp flag
    if(strstr(argv[i], "-web") != NULL){
      portnum=atoi(argv[i+1]);
      ana->SetWeb();      
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
    printf("\n");
    printf("=========================\n");
    printf("Usage: ./ana xxxx.bld xxxx.root [options]\n");
    printf("\n");
    printf("Example: $> ./ana run7144.bld run7144.root\n");
    printf("\n");
    printf("Options: \n");
    printf(" -online: Online mode. When the program reaches at the eond of file, it will stand-by and wait for another event.\n");
    printf(" -web [port num]: Enable THttp server. http://aino-1:[port num] \n");
    printf(" -last: Analyze from the last block. \n");        
    printf("=========================\n");
    printf("\n");
    exit(0);
  }

  ana->ShowCommandOption();

  ana->AnaParFile();
  ana->SetKinema();
  if(ana->GetWeb()){
  }
  
  /* Open .bld file */
  int bldres = ana->OpenBLDFile();
  if(bldres==1) exit(0);  // file not exist

  /* Make output ROOT file with tree and histograms */
  ana->MakeROOTFile();
  ana->TreeDef();
  ana->HistDef();
  if(ana->GetWeb()) ana->MakeTHttp(portnum);

  ana->AnaRunHeader();

  /* Seek the last block */
  if(ana->GetLast()){
    ana->SeekLastBlk();
  }
  
  /* Analyze blocks */
  unsigned int blkcnt=0;
  while(!eflag){
    if(!(ana->IsBLDeof())){
      ana->AnaBlk();
      blkcnt++;

      // IMPORTANT: one should regularly call ProcessEvents
      // to let http server process requests
      if(gSystem->ProcessEvents()) break;
    }
    if(ana->GetOnline() && ana->IsBLDeof()){
      printf("Waiting for data... (eve: %d)\r", ana->GetEveNum());
      fflush(stdout);
      ana->ClearBLDError();
      //      sleep(10);
      usleep(100000);      
    }
    if( ((ana->GetOnline())!=1) && ana->IsBLDeof()){
      break;
    }
  }
  
  ana->CloseBLDFile();
  ana->ShowCamacSca();
  ana->ShowVmeSca();  
  ana->ShowGREff();
  
  printf("\nAnalyzed %d events (%d blocks)\n", ana->GetEveNum(), blkcnt);
  
  if(ana->GetWeb()){
    while(!eflag){
      gSystem->Sleep(10); // sleep minimal time
      if(gSystem->ProcessEvents()) break;
    }
  }

  /* Save output ROOT */
  ana->HistWrite();
  ana->TreeWrite();  
  ana->CloseROOTFile();
  
  return 0;
}


void abrt_handler(int sig, siginfo_t *info, void *ctx) {
  printf("\n");
  printf("Interrupt is detected!\n");
  printf("Saving output file...\n");
  printf("\n");
  eflag = 1;
}
