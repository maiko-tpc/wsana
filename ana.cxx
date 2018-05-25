#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <TFile.h>
#include <TTree.h>
#include <TCut.h>
#include <fstream>
#include <TLorentzVector.h>

#define MaxComLen  124
#define ADC_RESO 2

TFile *file;
TTree *tree1;
TTree *tree2;
void analysis(char *filename);
void error_exit(TFile *file, int i);

typedef struct BlkHeader{
  unsigned short                headerID;               /* = 0xFFFF */
  unsigned short                headerSize;
  unsigned short                blockID;
  unsigned short                blockSize;             /* disposed. fixed to 0xfff0 */
  unsigned short                blockNumber;
  unsigned short                numEvents;
  unsigned short                blockSize32_l;          /* 32 bit block size , added on 19-OCT-2014 lower 16 bits*/
  unsigned short                blockSize32_u;          /* 32 bit block size , added on 19-OCT-2014 upper 16 bits*/
} BlkHeader, *BlkHeaderPtr, **BlkHeaderHdl;

typedef struct RunComment{
  unsigned short version;      /* = 4.0 = x0400*/
  unsigned short res1;         /* reserved word 1*/
  unsigned int   byte;           /* byte order */
  unsigned int   time;           /* seconds from the unix epoch */
  unsigned short run;           /* run number */
  unsigned short res2;         /* reserved word 2*/
  unsigned char  comment[MaxComLen];
} RunComment, *RunCommentPtr;

typedef struct BlkTrailer{
  unsigned short		trailerID;	  /* = 0xFFEF */
  unsigned short		trailerSize;
} BlkTrailer, *BlkTrailerPtr, **BlkTrailerHdl;

typedef struct EvtHeader{
  unsigned short		headerID;	  /* = 0xFFDF */
  unsigned short		headerSize;
  unsigned short		eventID;
  unsigned short		eventSize;
  unsigned short		eventNumber;
  unsigned short		numFields;
#if false
  unsigned short		fieldFlag;
#endif
} EvtHeader, *EvtHeaderPtr, **EvtHeaderHdl;

typedef struct FldHeader{
  unsigned short		headerID;	 /* = 0xFFCF */
  unsigned short		headerSize;
  unsigned short		fieldID;
  unsigned short		fieldSize;
} FldHeader, *FldHeaderPtr, **FldHeaderHdl;

typedef struct bld1_header{  /* all components are defined by the network byte order */
  int             id;        /* identification word ('BLD1') */
  int             seq_num;   /* sequential number */
  int             bsize;     /* block size in bytes */
  int             hsize;     /* header size in bytes */
  int             prev_pos1; /* absolute prev buffer position in byte (upper)*/
  int             prev_pos2; /*                                       (lower)*/
  int             next_pos1; /* absolute prev buffer position in byte (upper)*/
  int             next_pos2; /*                                       (lower)*/
} bld1_header_t;


int main(int iarg, char *argv[]) {
  if (iarg != 3) {
    printf("usage: ./ana xxxx.ssd xxxx.root\n");
    printf("eg.)\n");
    printf("$ ./ana run7144.ssd run7144.root\n");
    exit(0);
  }
  
  
  file = new TFile(argv[2], "recreate");
  tree1 = new TTree("tree1", "tree1");
  tree2 = new TTree("tree2", "tree2");
  
  analysis(argv[1]);
  
  tree1->Write();
  tree2->Write();
  file->Close();
  return 0;
}


void analysis(char *filename) {
  /* open data file */
  
  ifstream infile(filename,std::ios_base::in);
  if (!infile){
    error_exit(file, 0);
  }
  
  bld1_header bld1h;
  BlkHeader tmp_blkh;
  unsigned int blksize;
  
  /* search the first header */
  unsigned short comment[100000];
  
  
  infile.read((char*)&bld1h, sizeof(bld1h));
  infile.read((char*)&tmp_blkh, sizeof(tmp_blkh));

  printf("Header ID= 0x%04x\n", tmp_blkh.headerID);
  printf("HeaderSize= 0x%04x\n", htons(tmp_blkh.headerSize));
  printf("BlockID= 0x%04x\n", htons(tmp_blkh.blockID));
  printf("blockSize= 0x%04x\n", tmp_blkh.blockSize);
  printf("blockNumber= 0x%04x\n", tmp_blkh.blockNumber);
  printf("numEvents= 0x%04x\n", tmp_blkh.numEvents);
  printf("blockSize32_l= 0x%04x\n", htons(tmp_blkh.blockSize32_l));
  printf("blockSize32_u= 0x%04x\n", htons(tmp_blkh.blockSize32_u));
  printf("------------------------\n");
  
  blksize =  0x00010000*htons(tmp_blkh.blockSize32_u)
    +htons(tmp_blkh.blockSize32_l);
  printf("blksize=%d\n", blksize);
  printf("------------------------\n");

  //  infile.seekg(-16, std::ios_base::cur);
  infile.read((char*)&comment,
  	      blksize*2);
  //  infile.read((char*)&comment,
  //	      ((sizeof(short))*blksize));
  
  infile.read((char*)&bld1h, sizeof(bld1h));
  printf("id= %08x\n", bld1h.id);
  printf("seq_num= %08x\n", htonl(bld1h.seq_num));
  printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
  
  
  infile.close();
  
}

void error_exit(TFile *file, int i) {
  printf("error exit [%d]\n", i); fflush(stdout);
  file->Close();
  exit(0);
}
