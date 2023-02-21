#include "analysis.hpp"

//#define DEBUG

#define MAX_REGION 100000

analysis::analysis(){
  evt.eve=0;

  sprintf(opt.rootfname, "out.root");
  opt.online_flag=0;
  opt.web_flag=0;  
  opt.useage_flag=1;  
  sprintf(opt.parfname, "par/default.par");
  
  ClearCamacSca();
  
  pla = new anapla();

  gr = new anagr();
  gr->SetGRPars();
  gr->SetTDC2LenTab_GR();

  ssd = new anassd();

  kine = new kinema();
  
  par_debug_mode = 0;
}

analysis::~analysis(){
  delete gr;
}

int analysis::OpenBLDFile(){
  bldfile.open(GetBLDFile(), ios_base::in|ios::binary);
  if(!bldfile){
    printf("File '%s' does not exist!\n", GetBLDFile());
    printf("Input File: %s\n", GetBLDFile());
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

void analysis::ClearBLDError(){
  bldfile.clear();
}

void analysis::MakeROOTFile(){
  outfile = new TFile(GetROOTFile(), "RECREATE");
}

void analysis::CloseROOTFile(){
  outfile->Close();
}


TFile* analysis::GetFile(){
  return outfile;
}

void analysis::read_config_file(char *config_file_name){
  fprintf(stderr, "Reading configration file '%s'...\n", config_file_name);
  config_set_debug_mode(par_debug_mode);
  config_read_file(config_file_name);
  fprintf(stderr, "Done\n");
}

void analysis::GetTargetPart(){
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
  if(evt.eve==0) printf("Field ID=%d\n", field_id);
  
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
      ana_inp(&evt, tmpdata, region_size, field_id);
      break;
    case 0x3:  // MADC32 or MQDC32
      //	ana_madc32(&evt.madc, tmpdata, region_size);
      //      ana_mxdc32(&evt, tmpdata, region_size);
      ana_mxdc32(evt.mxdc32_hit_all, tmpdata, region_size, field_id);		
      break;
    case 0x5:  // Scaler64?
      // will be implemented later...	  
      break;
    case 0x6:  // CAMAC scaler?
      ana_camac_sca(&evt, tmpdata, region_size, field_id);
      break;
    case 0x8:  // UNIX time
      ana_unixtime(&evt, tmpdata, region_size, field_id);
      break;
    case 0x9:  // V830 scaler
      // will be implemented later...
      break;	  
    case 0xd:  // FERA QDC
      //      ana_grpla_qdc(&evt.grpla, tmpdata, region_size);
      ana_fera(evt.fera_hit_all, tmpdata, region_size, field_id, region_id);
      break;
    case 0xe:  // FERA TDC
      //      ana_grpla_tdc(&evt.grpla, tmpdata, region_size);
      ana_fera(evt.fera_hit_all, tmpdata, region_size, field_id, region_id);
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

  // Analyze v1190 ref hit
  AnaV1190Ref();
  
  // Analyze input register by V1190
  AnaV1190inpreg();
  
  // GR plastic
  pla->analyze(&evt);
     
  // SSD
#ifdef ANASSD
  ssd->analyze(&evt);
#endif
  
  // GR VDC
  gr->anavdc(&evt);

  // kinema
  evt.grp=kine->Calc_p3(&evt);
  evt.grtote = kine->Calc_TotEne(kine->scat_nucl, evt.grp);
  evt.gre = kine->Calc_KineEne(kine->scat_nucl, evt.grtote);  
  
  
  if(evt.eve%10000==0){
    CalcGREff();
    //    ShowProg();
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
  evt.unixtime = (int)t;
  
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

int analysis::GetBlkNum(){
  return evt.blk;
}

int analysis::GetRunNum(){
  return evt.run;
}

void analysis::SetBLDFile(char *fname){
  sprintf(opt.bldfname, "%s", fname);
}

char* analysis::GetBLDFile(){
  return opt.bldfname;
}

void analysis::SetROOTFile(char *fname){
  sprintf(opt.rootfname, "%s", fname);
}

char* analysis::GetROOTFile(){
  return opt.rootfname;
}

void analysis::SetUseage(){
  opt.useage_flag=1;
}

void analysis::UnsetUseage(){
  opt.useage_flag=0;
}

int analysis::GetUseage(){
  return opt.useage_flag;
}

void analysis::SetOnline(){
  opt.online_flag=1;
}

int analysis::GetOnline(){
  return opt.online_flag;
}

void analysis::SetWeb(){
  opt.web_flag=1;
}

int analysis::GetWeb(){
  return opt.web_flag;
}

void analysis::SetParFile(char *fname){
  sprintf(opt.parfname, "%s", fname);
}

void analysis::ShowCommandOption(){
  printf("\n");
  printf("Input  file: %s\n", opt.bldfname);
  printf("Output file: %s\n", opt.rootfname);
  if(opt.online_flag){
    printf("Online mode\n");
  }
  if(opt.web_flag){
    printf("THttp server is enabled\n");
  }
}

void analysis::ShowCamacSca(){
  char sca_name[256];
  printf("*********** CAMAC Scaler ***********\n");
  for(int i=0; i<CAMAC_SCA_CH; i++){
    sprintf(sca_name, "Sca %02d", i);
    printf("%s: %012ld\n", sca_name, evt.camac_sca[i]);
  }
  printf("**********************\n");
}

void analysis::ShowGREff(){
  printf("\n");
  printf("GR Hit Efficiencies\n");
  printf("X1: %.3f, U1: %.3f, X2: %.3f, U2: %.3f\n",
	 evt.gr_hit_eff[0], evt.gr_hit_eff[1], evt.gr_hit_eff[2], evt.gr_hit_eff[3]);
  printf("Total Hit Efficiency: %.3f\n", evt.gr_hit_eff_all);
  printf("\n");

  printf("GR Cluster Efficiencies\n");
  printf("X1: %.3f, U1: %.3f, X2: %.3f, U2: %.3f\n",
	 evt.gr_clst_eff[0], evt.gr_clst_eff[1], evt.gr_clst_eff[2], evt.gr_clst_eff[3]);
  printf("Total Cluster Efficiency: %.3f\n", evt.gr_clst_eff_all);
  printf("\n");
}

void analysis::ClearCamacSca(){
  for(int i=0; i<CAMAC_SCA_CH; i++){
    evt.camac_sca[i]=0;
  }
}

int analysis::AnaParFile(){
  read_config_file(opt.parfname);

  // Read target nuclei
  sprintf(par.target_nucl, "%s",
	  config_get_s_value("target_nucl", 0, "12C"));

  // Read beam nuclei
  sprintf(par.beam_nucl, "%s",
	  config_get_s_value("beam_nucl", 0, "1H"));

  // Read scattered nuclei
  sprintf(par.scat_nucl, "%s",
	  config_get_s_value("scat_nucl", 0, "1H"));
  
  // Read recoil nuclei
  sprintf(par.recoil_nucl, "%s",
	  config_get_s_value("recoil_nucl", 0, "12C"));

  // Read beam nuclei
  par.beam_ene = config_get_d_value("beam_ene", 0, 100.0);

  // Read GR angle set
  par.gr_ang = config_get_d_value("gr_ang", 0, 4.0);

  // Read GR angle set
  par.gr_mag = config_get_d_value("gr_mag", 0, 0.90);

  // Show results
  printf("------ Ana parameters ------\n");
  printf("Target nuclei   : %s\n",     par.target_nucl);
  printf("Beam nuclei     : %s\n",     par.beam_nucl);
  printf("Scattered nuclei: %s\n",     par.scat_nucl);
  printf("Recoil nuclei   : %s\n",     par.recoil_nucl);
  printf("Beam energy     : %f MeV\n", par.beam_ene);
  printf("GR angle        : %f deg\n", par.gr_ang);
  printf("GR mag field    : %f T\n"  , par.gr_mag);        
  printf("---------------------\n");
  
  return 0;
}

void analysis::SetKinema(){

  int nucl_size = (int)(kine->nucl_data.size());
  printf("Nucl database: %d\n", nucl_size);
  
  // search the target nuclei
  for(int i=0; i<nucl_size; i++){
    if(strcmp(par.target_nucl, kine->nucl_data[i].name)==0){
      kine->target_nucl = kine->nucl_data[i];
      break;
    }
    if(i==nucl_size-1){
      printf("Cannot find the target in data: %s\n",
	     par.target_nucl);
    }
  }
  
  // search the beam nuclei
  for(int i=0; i<nucl_size; i++){
    if(strcmp(par.beam_nucl, kine->nucl_data[i].name)==0){
      kine->beam_nucl = kine->nucl_data[i];
      break;
    }
    if(i==nucl_size-1){
      printf("Cannot find the beam in data: %s\n",
	     par.beam_nucl);
    }
  }

  // search the scattered nuclei
  for(int i=0; i<nucl_size; i++){
    if(strcmp(par.scat_nucl, kine->nucl_data[i].name)==0){
      kine->scat_nucl = kine->nucl_data[i];
      break;
    }
    if(i==nucl_size-1){
      printf("Cannot find the scattered nucleus in data: %s\n",
	     par.scat_nucl);
    }
  }

  // search the recoil nuclei
  for(int i=0; i<nucl_size; i++){
    if(strcmp(par.recoil_nucl, kine->nucl_data[i].name)==0){
      kine->recoil_nucl = kine->nucl_data[i];
      break;
    }
    if(i==nucl_size-1){
      printf("Cannot find the recoil nucleus in data: %s\n",
	     par.recoil_nucl);
    }
  }
  
  kine->beam_ene = par.beam_ene;
  kine->gr_ang = par.gr_ang;
  kine->gr_mag = par.gr_mag;    
  
  // Prepare the kinema calc
  kine->SetMass();
  kine->SetBrho(&par);
  
}

void analysis::CalcGREff(){
  for(int i=0; i<N_VDCPLANE; i++){
    evt.gr_hit_eff[i] = hhiteff[i]->GetBinContent(2)/hhiteff[i]->GetEntries();
    evt.gr_clst_eff[i] = hclsteff[i]->GetBinContent(2)/hclsteff[i]->GetEntries();
  }
  evt.gr_hit_eff_all = hhiteffall->GetBinContent(2)/hhiteffall->GetEntries();
  evt.gr_clst_eff_all = hclsteffall->GetBinContent(2)/hclsteffall->GetEntries();  
}

void analysis::ShowProg(){
  mvprintw(1, 2, "1:Start,  2:Stop,  3:Reset,  Ctrl-c:Exit \n");
  refresh();
}

void analysis::AnaV1190Ref(){
  unsigned int tmp_field;
  unsigned int tmp_geo, tmp_ch;
  int hit_size = (int)(evt.v1190_hit_all.size());

  for(int i=0; i<hit_size; i++){
    tmp_field = evt.v1190_hit_all[i].field;
    tmp_geo = evt.v1190_hit_all[i].geo;

    if(tmp_field == FIELD_SSD || tmp_field == FIELD_GV_NEW ||
       tmp_field == FIELD_GV_OLD){
      tmp_ch = evt.v1190_hit_all[i].ch;
      if(tmp_ch==127) evt.v1190_hit_ref[tmp_geo]=1;
    }

    if(tmp_field == FIELD_PLA){
      tmp_ch = evt.v1190_hit_all[i].ch;
      if(tmp_ch==15) evt.v1190_hit_ref[tmp_geo]=1;
    }
  }
}

void analysis::AnaV1190inpreg(){
  unsigned int tmp_field, tmp_ch, inp_ch;
  int hit_size = (int)(evt.v1190_hit_all.size());

  for(int i=0; i<5; i++) evt.vme_inp[i]=1; // level input

  for(int i=0; i<hit_size; i++){
    tmp_field = evt.v1190_hit_all[i].field;
    tmp_ch = evt.v1190_hit_all[i].ch;
    
    if(tmp_field == FIELD_PLA && tmp_ch>15 && tmp_ch<32){
      inp_ch = tmp_ch-16;

      if(inp_ch>=0 && inp_ch<5)  evt.vme_inp[inp_ch] = 0; // level input
      if(inp_ch>=5 && inp_ch<16) evt.vme_inp[inp_ch] = 1; // pulse input
    }
  }
}

void analysis::InitEvt(){  
  init_madc32_data(&evt.madc);
  evt.mxdc32_hit_all.clear();

  //  init_v1190_data(&evt.v1190_ssd);
#ifdef ANASSD
  ssd->init_data(&evt);
#endif
  init_grpla_data(&evt.grpla);
  init_grpla_data(&evt.laspla);  
  evt.grvdc.clear();
  evt.grvdc_x.clear();
  evt.grvdc_u.clear();  

  evt.v1190_hit_all.clear();
  evt.mxdc32_hit_all.clear();
  evt.fera_hit_all.clear();    
  
  for(int i=0; i<N_VDCPLANE; i++){
    evt.nclst[i]=0;
    evt.clst_size[i]=0;    
  }
  
  for(int i=0; i<N_VDCPLANE; i++){
    evt.nhit_plane[i]=0;
    evt.mean_wire[i]=0.0;
  }

  for(int i=0; i<N_GRPLA_CH; i++){
    evt.grpla.adc[i]=0;
    evt.grpla.tdc[i]=0;
    evt.grpla.fqdc[i]=0;
    evt.grpla.ftdc[i]=0;    
    evt.grpla.vqdc[i]=0;
    evt.grpla.vtdc[i]=0;    
  }

  for(int i=0; i<N_GRPLA; i++){
    evt.grpla.pos[i]=-100;
    evt.grpla.de[i]=-100;    
  }

  for(int i=0; i<V1190_MAX_GEO; i++){
    evt.v1190_hit_ref[i]=0;
  }
  
  evt.gr_good_hit=0;
  evt.gr_good_clst=0;

  for(int i=0; i<N_VDCPLANE; i++){
    evt.redchi2[i]=100000;
    evt.wire_pos[i]=1000000;
    evt.residual[i]=1000000;    
  }

  evt.grx = 100000;
  evt.gry = 100000;  
  evt.grthx = 100000;
  evt.grthy = 100000;  

  evt.good_fit=1;

  evt.grp_rela = -5;
  evt.grp = (kine->p3_cen)*(1+(evt.grp_rela)/100.0);
  evt.grtote = kine->Calc_TotEne(kine->scat_nucl, evt.grp);
  evt.gre = kine->Calc_KineEne(kine->scat_nucl, evt.grtote);  
  
  for(int i=0; i<N_INP; i++){
    evt.camac_inp[i]=0;
    evt.vme_inp[i]=0;
  }
  evt.first_camac_inp=1;
  
  evt.camac_sca_flag=0;

  for(int i=0; i<16; i++){
    evt.v1190pla_multi[i]=0;
  }

  // for E552 on 2023 Feb.
  for(int i=0; i<32; i++){
    evt.band_v1190_multi[i]=0;
    evt.band_v1190_lead[i]=-1000000;    
  }

}


