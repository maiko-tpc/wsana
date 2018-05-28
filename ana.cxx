#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <fstream>

#include "mtformat.h"
#include "moduledata.h"
#include "decoder.h"


#define MAX_REGION 10000
#define ADC_RESO 2

//#define DEBUG 1

using namespace std;
void analysis(char *filename);
void error_exit(TFile *file, int i);

TFile *file;
TTree *tree;

unsigned int eve;
madc32_data madc;
v1190_data v1190_ssd;

int main(int iarg, char *argv[]) {
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
  
//  tree->Branch("v1190_ssd", &v1190_ssd,
//	       Form("lead[%d][%d]/I:trail[%d][%d]/I:tot[%d][%d]/I:multi[%d]/I:counter/I",
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH, V1190_MAX_MULTI,
//		    N_V1190_CH));
  
  analysis(argv[1]);
  
  tree->Write();
  file->Close();
  return 0;
}


void analysis(char *filename) {
  /* open data file */
  
  ifstream infile(filename,ios_base::in|ios::binary);
  if (!infile){
    error_exit(file, 0);
  }
  
  bld1_header bld1h;
  //  RunComment runcom;
  BlkHeader tmp_blkh;
  EvtHeader evth;
  FldHeader fldh;
  RegionHeader regionh;
  
  unsigned int blksize;
  
  unsigned short comment[100000];  

  eve=0;
  printf("analyzed %d events\n", eve);
  fflush(stdout);
  
  /* Read the first BLD1 header */
  infile.read((char*)&bld1h, sizeof(bld1h));

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
  //  infile.read((char*)&runcom, blksize*2);
  
  while(!infile.eof()){

    /* Read the BLD1 header */
    infile.read((char*)&bld1h, sizeof(bld1h));

#ifdef DEBUG
    printf("id= %08x\n", bld1h.id);
    printf("seq_num= %08x\n", htonl(bld1h.seq_num));
    printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
    printf("------------------------\n");
#endif
    
    /* Read the Block header */  
    infile.read((char*)&tmp_blkh, sizeof(tmp_blkh));
#ifdef DEBUG
    printf("Header ID= 0x%04x\n", htons(tmp_blkh.headerID));
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
    
    unsigned int blksize_byte=4*(0x00010000*htons(tmp_blkh.blockSize32_u)+
				 htons(tmp_blkh.blockSize32_l));
    unsigned int byte_cnt=0;
    
    while(byte_cnt<blksize_byte){

      /* Read the Event header */  
      infile.read((char*)&evth, sizeof(evth));
      byte_cnt+=sizeof(evth);
      eve++;
#ifndef DEBUG      
      if(eve%10000==0){
	//	printf("\ranalyzed %d events", eve);
	printf("analyzed %d events\n", eve);
      }
#endif
      
#ifdef DEBUG
      printf("size=%d\n", byte_cnt);
      printf("header ID= 0x%04x\n", htons(evth.headerID));
      printf("event header size= 0x%04x\n", htons(evth.headerSize));
      printf("eventID= 0x%04x\n", htons(evth.eventID));
      printf("eventSize= 0x%04x\n", htons(evth.eventSize));
      printf("eventNumber= 0x%04x\n", htons(evth.eventNumber));
      printf("eventnumFields= 0x%04x\n", htons(evth.numFields));
      printf("------------------------\n");
#endif
      
      /* Event initialization */
//      for(i=0; i<MAX_REGION; i++){
//	tmpdata[i]=0;
//      }
      unsigned int tmpdata[MAX_REGION]={0};
      init_madc32_data(&madc);
      init_v1190_data(&v1190_ssd);
      
      /* Read the Field header */  
      infile.read((char*)&fldh, sizeof(fldh));  
      byte_cnt+=sizeof(fldh);
      unsigned int field_size = htons(fldh.fieldSize);
      
#ifdef DEBUG
      printf("headerID= 0x%04x\n", htons(fldh.headerID));
      printf("headerSize= 0x%04x\n", htons(fldh.headerSize));
      printf("fieldID= 0x%04x\n", htons(fldh.fieldID));
      printf("fieldSize= 0x%04x\n", field_size);
      printf("------------------------\n");  
#endif
      
      unsigned int fldcnt=0;

      //  unsigned int madc32data[1000];
      unsigned int region_id, region_size;
      while(fldcnt<field_size){

	/* Read region header */
	infile.read((char*)&regionh, sizeof(regionh));
	byte_cnt+=sizeof(regionh);
	region_id=htons(regionh.id)>>12;
	region_size=regionh.size;
	fldcnt+=1;
	
	/* Read the module data */
	infile.read((char*)&tmpdata, region_size*2);
	byte_cnt+=region_size*2;
	fldcnt+=region_size;
	

	if(eve>77228 && eve<77234)printf("eve=%d, region id=0x%x, size=%d\n", eve, region_id, region_size);
	if(eve==2)printf("eve=%d, region id=0x%x, size=%d\n", eve, region_id, region_size);
	
	
	switch(region_id){
	case 1:  // V1190
	  ana_v1190(&v1190_ssd, tmpdata, region_size);
	  //	  printf("v1190, eve=%d\n", eve);
	  break;
	case 3:  // MADC32
	  ana_madc32(&madc, tmpdata, region_size);
	  //	  printf("madc, eve=%d\n", eve);
	  break;

	default:
	  break;
	} // end of switch(region_id)
      } // end of while(fldcnt<field_size)
      
      tree->Fill();
    } // end of while(byte_cnt<blksize_byte)
    
  } // end of while(!infile.eof())
  
  infile.close();
  printf("analyzed %d events\n", eve);
  
}  // end of analysis

void error_exit(TFile *file, int i) {
  printf("error exit [%d]\n", i); fflush(stdout);
  file->Close();
  exit(0);
}

