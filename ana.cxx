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

#include "mtformat.h"
#include "moduledata.h"
#include "decoder.h"


#define MAX_REGION 100000
#define ADC_RESO 2

//#define DEBUG 

using namespace std;
void analysis(char *filename);
void error_exit(TFile *file, int i);
void abrt_handler(int sig, siginfo_t *info, void *ctx);
volatile sig_atomic_t eflag = 0;

TFile *file;
TTree *tree;

unsigned int eve;
madc32_data madc;
v1190_data v1190_ssd;
grpla_data grpla;
vector<grvdc_data> grvdc;

int main(int iarg, char *argv[]) {

  /* define action when Cntl-c is given */
  struct sigaction sa_sigabrt;
  memset(&sa_sigabrt, 0, sizeof(sa_sigabrt));
  sa_sigabrt.sa_sigaction = abrt_handler;
  sa_sigabrt.sa_flags = SA_SIGINFO;

  if (sigaction(SIGINT, &sa_sigabrt, NULL) < 0 ) {
    exit(1);
  }
  
  if (iarg != 3) {
    printf("usage: ./ana xxxx.ssd xxxx.root\n");
    printf("eg.)\n");
    printf("$ ./ana run7144.ssd run7144.root\n");
    exit(0);
  }
  
  file = new TFile(argv[2], "recreate");
  tree = new TTree("tree", "tree");
  
  tree->Branch("eve", &eve, "eve/I");
  
  tree->Branch("madc", &madc,
	       Form("wrdcnt[%d]/I:adc[%d]/I:counter[%d]/I",
		    N_MADC, N_MADC_CH, N_MADC));
  
  tree->Branch("v1190_ssd", &v1190_ssd,
	       Form("lead[%d][%d]/I:trail[%d][%d]/I:tot[%d][%d]/I:multi[%d]/I:counter/I",
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH, V1190_MAX_MULTI,
		    N_V1190_CH));
  
  tree->Branch("grpla", &grpla,
	       Form("adc[%d]/I:tdc[%d]/I",
		    N_GRPLA_CH, N_GRPLA_CH));
  analysis(argv[1]);
  
  tree->Write();
  file->Close();
  return 0;
}


void analysis(char *filename) {

  /* open data file */  
  ifstream infile(filename,ios_base::in|ios::binary);
  if (!infile){
    printf("File '%s' does not exist!\n", filename);
    error_exit(file, 0);
  }
  
  bld1_header bld1h;
  //  RunComment runcom;
  BlkHeader tmp_blkh;
  EvtHeader evth;
  FldHeader fldh;
  
  unsigned int blksize;
  
  unsigned short comment[100000];  
  
  eve=0;
  printf("analyzed %d events\n", eve);
  fflush(stdout);
  
  /* Read the first BLD1 header */
  infile.read((char*)&bld1h, sizeof(bld1h));
#ifdef DEBUG
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("BLD1 header\n");
    printf("id= %08x\n", bld1h.id);
    printf("seq_num= %08x\n", htonl(bld1h.seq_num));
    printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
#endif

  
  /* Read the first Block header */
  infile.read((char*)&tmp_blkh, sizeof(tmp_blkh));
  
#ifdef DEBUG
  printf("Header ID= 0x%04x\n", tmp_blkh.headerID);
  printf("HeaderSize= 0x%04x\n", htons(tmp_blkh.headerSize));
  printf("BlockID= 0x%04x\n", htons(tmp_blkh.blockID));
  printf("blockSize in old format= 0x%04x\n",
	 htons(tmp_blkh.blockSize));
  printf("blockNumber= 0x%04x\n", htons(tmp_blkh.blockNumber));
  printf("numEvents= 0x%04x\n", htons(tmp_blkh.numEvents));
  printf("blockSize32_l= 0x%04x\n", htons(tmp_blkh.blockSize32_l));
  printf("blockSize32_u= 0x%04x\n", htons(tmp_blkh.blockSize32_u));
  printf("------------------------\n");
#endif
  
  /* Block size w/o the header in 32 bit unit */
  blksize =  0x00010000*htons(tmp_blkh.blockSize32_u)
    +htons(tmp_blkh.blockSize32_l);
  
#ifdef DEBUG  
  printf("comment blksize=%d\n", blksize);
  printf("------------------------\n");
#endif
  
  /* Read the run comment */
  infile.read((char*)&comment, blksize*2);
//  infile.read((char*)&runcom, sizeof(runcom));
  
  while(!infile.eof() && !eflag){
    
    /* Read the BLD1 header */
      infile.read((char*)&bld1h, sizeof(bld1h));
#ifdef DEBUG
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("BLD1 header\n");
    printf("eve=%d\n", eve);
    printf("id= %08x\n", bld1h.id);
    printf("seq_num= %08x\n", htonl(bld1h.seq_num));
    printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
#endif
    
    /* Read the Block header */  
      infile.read((char*)&tmp_blkh, sizeof(tmp_blkh));
#ifdef DEBUG
    printf("++++++++++++++++++++++++++++++++\n");
    printf("Block Header\n");
    printf("Header ID= 0x%04x\n", htons(tmp_blkh.headerID));
    printf("HeaderSize= 0x%04x\n", htons(tmp_blkh.headerSize));
    printf("BlockID= 0x%04x\n", htons(tmp_blkh.blockID));
    printf("blockSize in old format= 0x%04x\n",
	   htons(tmp_blkh.blockSize));
    printf("blockNumber= 0x%04x\n", htons(tmp_blkh.blockNumber));
    printf("numEvents= 0x%04x\n", htons(tmp_blkh.numEvents));
    printf("blockSize32_l= 0x%04x\n", htons(tmp_blkh.blockSize32_l));
    printf("blockSize32_u= 0x%04x\n", htons(tmp_blkh.blockSize32_u));
    printf("++++++++++++++++++++++++++++++++\n");
#endif
    
    unsigned int blksize_byte=4*(0x00010000*htons(tmp_blkh.blockSize32_u)+
				 htons(tmp_blkh.blockSize32_l));
    unsigned int byte_cnt=0;
    
    while(byte_cnt<blksize_byte && (!infile.eof()) && !eflag){
      
      /* Read the Event header */  
      while(!infile.eof() && !eflag){
	infile.read((char*)&evth, sizeof(evth));
	byte_cnt+=sizeof(evth);
	if(htons(evth.headerID)==0xffdf) break;
      }

#ifndef DEBUG      
      if(eve%10000==0){
	printf("\ranalyzed %d events", eve);
	//	printf("analyzed %d events\n", eve);
      }
#endif
      
#ifdef DEBUG
      printf("========================\n");
      printf("Event header\n");
      printf("header ID= 0x%04x\n", htons(evth.headerID));
      printf("event header size= 0x%04x\n", htons(evth.headerSize));
      printf("eventID= 0x%04x\n", htons(evth.eventID));
      printf("eventSize= 0x%04x\n", htons(evth.eventSize));
      printf("eventNumber= 0x%04x\n", htons(evth.eventNumber));
      printf("eventnumFields= 0x%04x\n", htons(evth.numFields));
#endif
      
      /* Event initialization */
      //      unsigned int tmpdata[MAX_REGION]={0};
      unsigned int tmpdata[MAX_REGION];
      init_madc32_data(&madc);
      init_v1190_data(&v1190_ssd);
      init_grpla_data(&grpla);
      
      /* Read the Field header */  
      while(!infile.eof() && !eflag){
	infile.read((char*)&fldh, sizeof(fldh));  
	byte_cnt+=sizeof(fldh);
	if(htons(fldh.headerID)==0xffcf) break;
      }
      unsigned int field_size = htons(fldh.fieldSize);
      
#ifdef DEBUG
      printf("------------------------\n");  
      printf("Field header, eve=%d\n", eve);
      printf("headerID= 0x%04x\n", htons(fldh.headerID));
      printf("headerSize= 0x%04x\n", htons(fldh.headerSize));
      printf("fieldID= 0x%04x\n", htons(fldh.fieldID));
      printf("fieldSize= 0x%04x\n", field_size);
#endif
      
      unsigned int fldcnt=0;
      unsigned int region_id, region_size;
      unsigned short tmp_region;
      
      while(fldcnt<field_size && !infile.eof()){
	
	/* Read region header */
	infile.read((char*)&tmp_region, sizeof(short));
	byte_cnt+=sizeof(short);
	region_id=((ntohs(tmp_region))>>12)&0xf;
	region_size=(ntohs(tmp_region))&0x0fff;
	fldcnt+=1;
	
	/* Read the module data */
	infile.read((char*)&tmpdata, region_size*2);
	byte_cnt+=region_size*2;
	fldcnt+=region_size;
	
	switch(region_id){
	case 0x1:  // V1190
	  ana_v1190(&v1190_ssd, tmpdata, region_size);
	  break;
	case 0x3:  // MADC32
	  ana_madc32(&madc, tmpdata, region_size);
	  break;
	  
	case 0xd:  // FERA QDC
	  ana_grpla_qdc(&grpla, tmpdata, region_size);
	  break;

	case 0xe:  // FERA TDC
	  ana_grpla_tdc(&grpla, tmpdata, region_size);
	  break;

	default:
	  break;
	} // end of switch(region_id)
      } // end of while(fldcnt<field_size)
      eve++;
      tree->Fill();
    } // end of while(byte_cnt<blksize_byte)
  } // end of while(!infile.eof())
  
  
  infile.close();
  printf("\n");
  printf("analyzed %d events\n", eve);
  
}  // end of analysis

void error_exit(TFile *file, int i) {
  printf("error exit [%d]\n", i); fflush(stdout);
  file->Close();
  exit(0);
}

void abrt_handler(int sig, siginfo_t *info, void *ctx) {
  printf("\n");
  printf("Interrupt is detected!\n");
  printf("Saving output file.\n");
  printf("\n");
  eflag = 1;
}
