#include "analysis.hpp"

//#define DEBUG

#define MAX_REGION 100000

analysis::analysis(){
  evt.eve=0;

  pla = new anapla();

  gr = new anagr();
  gr->SetGRPars();
  gr->SetTDC2LenTab_GR();

  ssd = new anassd();
}

analysis::~analysis(){
  delete gr;
}

int analysis::OpenBLDFile(char *bldfilename){
  bldfile.open(bldfilename, ios_base::in|ios::binary);
  if(!bldfile){
    printf("File '%s' does not exist!\n", bldfilename);
    printf("Input File: %s\n", bldfilename);
    return 1;
  }
  return 0;
}

void analysis::CloseBLDFile(){
  bldfile.close();
}

bool analysis::IsBLDeof(){
  return bldfile.eof();
}

void analysis::MakeROOTFile(std::string outfname){
  outfile = new TFile(outfname.c_str(), "RECREATE");
}

void analysis::CloseROOTFile(){
  outfile->Close();
}

TFile* analysis::GetFile(){
  return outfile;
}






int analysis::AnaFld(){
  /* Read the Field header */  
  FldHeader fldh;
  unsigned int byte_cnt=0;

  while(!bldfile.eof()){
    bldfile.read((char*)&fldh, sizeof(fldh));  
    byte_cnt+=sizeof(fldh);
    if(htons(fldh.headerID)==0xffcf) break;
  }

  unsigned int field_size = htons(fldh.fieldSize);
  int field_id = htons(fldh.fieldID);
  if(evt.eve==0) printf("%d\n", field_id);
  
#ifdef DEBUG
    printf("------------------------\n");  
    printf("Field header, eve=%d\n", evt.eve);
    printf("headerID= 0x%04x\n", htons(fldh.headerID));
    printf("headerSize= 0x%04x\n", htons(fldh.headerSize));
    printf("fieldID= 0x%04x\n", field_id);
    printf("fieldSize= %d\n", field_size);
#endif
  
  unsigned int fldcnt=0;
  unsigned int region_id, region_size;
  unsigned short tmp_region;
  unsigned int tmpdata[MAX_REGION];
  
  while(fldcnt<field_size && !bldfile.eof()){
    
    /* Read region header */
    bldfile.read((char*)&tmp_region, sizeof(short));
    byte_cnt+=sizeof(short);
    region_id=((ntohs(tmp_region))>>12)&0xf;
    region_size=(ntohs(tmp_region))&0x0fff;
    fldcnt+=1;
    
    /* Read the module data */
    bldfile.read((char*)&tmpdata, region_size*2);
    byte_cnt+=region_size*2;
    fldcnt+=region_size;
    
    switch(region_id){
    case 0x1:  // V1190
      ana_v1190(evt.v1190_hit_all, tmpdata, region_size, field_id);      
      break;
    case 0x2:  // input register
	// will be implemented later...	  
      break;
    case 0x3:  // MADC32
	ana_madc32(&evt.madc, tmpdata, region_size);
	break;
    case 0x6:  // CAMAC scaler?
      // will be implemented later...	  
      break;
    case 0x8:  // UNIX time
      // will be implemented later...	  
      break;
    case 0x9:  // V830 scaler
      // will be implemented later...
      break;	  
    case 0xd:  // FERA QDC
      ana_grpla_qdc(&evt.grpla, tmpdata, region_size);
      break;
    case 0xe:  // FERA TDC
      ana_grpla_tdc(&evt.grpla, tmpdata, region_size);
      break;
    case 0xf:  // CheckSUM?
      // will be implemented later...
      break;	  
      
    default:
      printf("Unknown region_id: %d\n", region_id);
      break;
    } // end of switch(region_id)
  } // end of while(fldcnt<field_size)
  return 0;
}

int  analysis::AnaEvt(){

  unsigned int byte_cnt=0;
  EvtHeader evth;

  /* Event initialization */
  InitEvt();

  /* Read the Event header */  
  while(!bldfile.eof()){
    bldfile.read((char*)&evth, sizeof(evth));
    byte_cnt+=sizeof(evth);
    if(htons(evth.headerID)==0xffdf) break;
  }

  unsigned int field_num = htons(evth.numFields);
  
#ifdef DEBUG
  printf("========================\n");
  printf("Event header\n");
  printf("header ID= 0x%04x\n", htons(evth.headerID));
  printf("event header size= 0x%04x\n", htons(evth.headerSize));
  printf("eventID= 0x%04x\n", htons(evth.eventID));
  printf("eventSize= 0x%04x\n", htons(evth.eventSize));
  printf("eventNumber= 0x%04x\n", htons(evth.eventNumber));
  printf("eventnumFields= %d\n", field_num);
#endif

  /* Analyze each field */
  unsigned int field_cnt = 0;
  while(field_cnt < field_num){
    AnaFld();
    field_cnt++;
  }
  
  /* Event by event analysis */

  // GR plastic
  pla->analyze(&evt);
     
  // SSD
#ifdef ANASSD
  ssd->analyze(&evt);
#endif
  
  // GR VDC
  gr->anavdc(&evt);


  if(evt.eve%10000==0){
    printf("Analyzed %d events\n", evt.eve);
  }

  tree->Fill();
  HistFill();

  evt.eve++;
  return evt.eve;
}


int analysis::AnaBlk(){
    
  if(!bldfile.eof()){
    
    /* Read the BLD1 header */
    bld1_header bld1h;
    bldfile.read((char*)&bld1h, sizeof(bld1h));
#ifdef DEBUG
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    printf("BLD1 header\n");
    printf("eve=%d\n", evt.eve);
    printf("id= %08x\n", bld1h.id);
    printf("seq_num= %08x\n", htonl(bld1h.seq_num));
    printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
    printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
#endif
    
    /* Read the Block header */  
    BlkHeader blkh;
    bldfile.read((char*)&blkh, sizeof(blkh));
    evt.blk = htons(blkh.blockNumber);
#ifdef DEBUG
    printf("++++++++++++++++++++++++++++++++\n");
    printf("Block Header\n");
    printf("Header ID= 0x%04x\n", htons(blkh.headerID));
    printf("HeaderSize= 0x%04x\n", htons(blkh.headerSize));
    printf("BlockID= 0x%04x\n", htons(blkh.blockID));
    printf("blockSize in old format= 0x%04x\n",
	   htons(blkh.blockSize));
    printf("blockNumber= 0x%04x\n", htons(blkh.blockNumber));
    printf("numEvents= 0x%04x\n", htons(blkh.numEvents));
    printf("blockSize32_l= 0x%04x\n", htons(blkh.blockSize32_l));
    printf("blockSize32_u= 0x%04x\n", htons(blkh.blockSize32_u));
    printf("++++++++++++++++++++++++++++++++\n");
#endif
    
    unsigned int numeve = htons(blkh.numEvents);
    unsigned int eve_cnt=0;

    while(eve_cnt<numeve && (!bldfile.eof())){
      AnaEvt();
      eve_cnt++;
    } // end of while(eve_cnt<numeve)

    /* Read the Block Trailer */
    BlkTrailer blkt;
    bldfile.read((char*)&blkt, sizeof(blkt));    
#ifdef DEBUG
    printf("----------------------\n");
    printf("Block Trailer\n");
    printf("ID= %04x\n", blkt.trailerID);
    printf("size= 0x%08x\n", htonl(blkt.trailerSize));
#endif

  } // end of while(!bldfile.eof())
  
  return evt.eve;
}

void analysis::AnaRunHeader(){
  bld1_header bld1h;
  BlkHeader blkh;
  unsigned int blksize;  
  
  /* Read the first BLD1 header */
  bldfile.read((char*)&bld1h, sizeof(bld1h));
#ifdef DEBUG
  printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
  printf("BLD1 header\n");
  printf("id= %08x\n", bld1h.id);
  printf("seq_num= %08x\n", htonl(bld1h.seq_num));
  printf("bsize= 0x%08x\n", htonl(bld1h.bsize));
  printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
#endif
  
  /* Read the first Block header */
  bldfile.read((char*)&blkh, sizeof(blkh));
  
#ifdef DEBUG
  printf("Header ID= 0x%04x\n", blkh.headerID);
  printf("HeaderSize= 0x%04x\n", htons(blkh.headerSize));
  printf("BlockID= 0x%04x\n", htons(blkh.blockID));
  printf("blockSize in old format= 0x%04x\n",
	 htons(blkh.blockSize));
  printf("blockNumber= 0x%04x\n", htons(blkh.blockNumber));
  printf("numEvents= 0x%04x\n", htons(blkh.numEvents));
  printf("blockSize32_l= 0x%04x\n", htons(blkh.blockSize32_l));
  printf("blockSize32_u= 0x%04x\n", htons(blkh.blockSize32_u));
  printf("------------------------\n");
#endif
  
  /* Block size w/o the header in 32 bit unit */
  blksize =  0x00010000*htons(blkh.blockSize32_u)
    +htons(blkh.blockSize32_l);
  
#ifdef DEBUG  
  printf("comment blksize=%d\n", blksize);
  printf("------------------------\n");
#endif
  
  /* Read the run comment */
  bldfile.read((char*)&comment, blksize*2);

  RunCommentPtr com;
  com = (RunCommentPtr)comment;

  char str[MaxComLen+10];
  strncpy_uchida(str, (char*)com->comment, MaxComLen);
  
  unsigned long t;
  t = ntohl(com->time);
  evt.run = ((com->run<<8) + (com->run>>8))&0x0000ffff;

  printf("---------------------------\n");
  printf("Run %04d\n", evt.run);
  printf("%s\n", ctime((time_t*)&t));
  printf("Comment: %s\n", str);
  //  printf("Byte Order = %.8x\n", com->byte);
  //  printf("Time = %.8x\n", com->time);  
  //  printf("0x01020304=%.8x\n", ntohl(0x01020304));
  printf("---------------------------\n");
  printf("\n");
}

void analysis::strncpy_uchida(char *dst, char *src, int max_len){
  int i;
  for(i=0; i<max_len; i++){
    dst[i] = src[i*2+1]; // changed from src[i*2]
    if(!dst[i]) break;
  }
}

int analysis::GetEveNum(){
  return evt.eve;
}

int analysis::GetRunNum(){
  return evt.run;
}

void analysis::InitEvt(){
  init_madc32_data(&evt.madc);
  init_v1190_data(&evt.v1190_ssd);
  init_grpla_data(&evt.grpla);
  evt.grvdc.clear();
  evt.grvdc_x.clear();
  evt.grvdc_u.clear();  
  evt.v1190_hit_all.clear();  
  
  for(int i=0; i<N_VDCPLANE; i++){
    evt.nclst[i]=0;
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    evt.nhit_plane[i]=0;
    evt.mean_wire[i]=0.0;
  }
}
