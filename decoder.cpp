#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include "decoder.hpp"

#define V1190_SSD_GEO 24
#define V1190_MAX_GEO 32

#define V1190_SUBTRACT_REF 1

#define VSN_GR 1

using namespace std;

unsigned int flip_32bit(unsigned int inp){
  unsigned int low = inp&0x0000ffff;
  unsigned int up  = (inp>>16)&0xffff;
  return low*0x00010000 + up;
}

void init_madc32_data(madc32_data *madc){
  int i;
  
  for(i=0; i<N_MADC; i++){
    madc->wrdcnt[i]=0;
    madc->counter[i]=0;
  }

  for(i=0; i<N_MADC_CH; i++){
    madc->adc[i]=0;
  }
}

void init_mqdc32_data(mqdc32_data *mqdc){
  int i;
  
  for(i=0; i<N_MQDC; i++){
    mqdc->wrdcnt[i]=0;
    mqdc->counter[i]=0;
  }

  for(i=0; i<N_MQDC_CH; i++){
    mqdc->adc[i]=0;
  }
}


void ana_madc32(madc32_data *madc, unsigned int *rawdata, unsigned int size){
  int finish_mod=0;
  unsigned int rp=0;
  unsigned int tmpdata;
  int geo;
  int nword;
  int ich;
  unsigned int tmpadc;
  unsigned int tmp_counter;
  
  while(rp<size/2){
    tmpdata=flip_32bit(ntohl(rawdata[rp]));
    rp++;

    if((tmpdata>>24) == 0x40){  // header
      geo=(tmpdata>>16)&0x00ff;
      if(geo>=N_MADC) geo=0;
      nword=(tmpdata)&0x00000fff;
      madc->wrdcnt[geo]=nword;
      for(int i=0; i<nword-1; i++){  // data
	tmpdata=flip_32bit(ntohl(rawdata[rp]));

	if((tmpdata>>26)==0x1){
	  ich=32*geo+((tmpdata>>16)&0x001f);
	  tmpadc=(tmpdata)&0x00001fff;
	  madc->adc[ich]=tmpadc;
	}
	rp++;
      }
      tmpdata=flip_32bit(ntohl(rawdata[rp]));
      rp++;
      if(((tmpdata>>30))==0x3){  // ender 
	tmp_counter=((unsigned int)tmpdata)&0x0fffffff;
	madc->counter[geo]=tmp_counter;
	finish_mod++;
      }
    }
    if(finish_mod==N_MADC) break;
  }

}

void ana_mxdc32(evtdata *evt, unsigned int *rawdata, unsigned int size){
  int finish_mod=0;
  unsigned int rp=0;
  unsigned int tmpdata;
  int geo;
  int nword;
  int ich;
  unsigned int tmpadc;
  unsigned int tmp_counter;
  
  while(rp<size/2){
    tmpdata=flip_32bit(ntohl(rawdata[rp]));
    rp++;

    if((tmpdata>>24) == 0x40){  // header
      geo=(tmpdata>>16)&0x00ff;
//      if(geo>=N_MADC || geo>=N_MQDC){
//	printf("Unknown MXDC32 geo: %d\n", geo);
//	geo=-1;
//      }
      nword=(tmpdata)&0x00000fff;

      if(geo>=MQDC_START_GEO && geo<(MQDC_START_GEO+N_MQDC)){
	evt->mqdc.wrdcnt[geo-MQDC_START_GEO]=nword;
      }
      if(geo>=MADC_START_GEO && geo<(MADC_START_GEO+N_MADC)){
	evt->madc.wrdcnt[geo-MADC_START_GEO]=nword;      
      }
      
      for(int i=0; i<nword-1; i++){  // data
	tmpdata=flip_32bit(ntohl(rawdata[rp]));

	if((tmpdata>>26)==0x1){

	  tmpadc=(tmpdata)&0x00001fff;
	  if(geo>=MQDC_START_GEO && geo<MQDC_START_GEO+N_MQDC){
	    ich=32*(geo-MQDC_START_GEO)+((tmpdata>>16)&0x001f);
	    evt->mqdc.adc[ich]=tmpadc;
	  }
	  if(geo>=MADC_START_GEO && geo<MADC_START_GEO+N_MADC){
	    ich=32*(geo-MADC_START_GEO)+((tmpdata>>16)&0x001f);
	    evt->madc.adc[ich]=tmpadc;
	  }	  

	}
	rp++;
      }
      tmpdata=flip_32bit(ntohl(rawdata[rp]));
      rp++;
      if(((tmpdata>>30))==0x3){  // ender 
	tmp_counter=((unsigned int)tmpdata)&0x0fffffff;
	if(geo>=MQDC_START_GEO && geo<MQDC_START_GEO+N_MQDC){
	  evt->mqdc.counter[geo-MQDC_START_GEO]=tmp_counter;
	}
	if(geo>=MADC_START_GEO && geo<MADC_START_GEO+N_MADC){
	  evt->madc.counter[geo-MADC_START_GEO]=tmp_counter;
	}
	finish_mod++;
      }
    }
    //if(finish_mod==N_MADC) break;
  }

}

void ana_mxdc32(vector<mxdc32_hit> &mxdc32_hit_all, unsigned int *rawdata, unsigned int size, int field_id){
  unsigned int rp=0;
  unsigned int tmpdata;
  int geo;
  int nword;
  int ich;
  unsigned int tmpadc;
  //  unsigned int tmp_counter;
  mxdc32_hit tmp_hit;

  
  while(rp<size/2){
    tmpdata=flip_32bit(ntohl(rawdata[rp]));
    rp++;

    if((tmpdata>>24) == 0x40){  // header
      geo=(tmpdata>>16)&0x00ff;
      nword=(tmpdata)&0x00000fff;

      for(int i=0; i<nword-1; i++){  // data
	tmpdata=flip_32bit(ntohl(rawdata[rp]));

	if((tmpdata>>26)==0x1){

	  ich = (tmpdata>>16)&0x001f;
	  tmpadc=(tmpdata)&0x00001fff;

	  tmp_hit.field = field_id;
	  tmp_hit.geo = geo;
	  tmp_hit.ch = ich;
	  tmp_hit.adc = tmpadc;
	  mxdc32_hit_all.push_back(tmp_hit);
	}
	rp++;
      }
      tmpdata=flip_32bit(ntohl(rawdata[rp]));
      rp++;
      if(((tmpdata>>30))==0x3){  // ender 
	//	tmp_counter=((unsigned int)tmpdata)&0x0fffffff;
      }
    }

  }

}


void init_v1190_data(v1190_data *v1190){
  int i,j;
  for(i=0; i<N_V1190_CH; i++){
    for(j=0; j<V1190_MAX_MULTI; j++){
      v1190->lead[i][j]=0;
      v1190->trail[i][j]=0;
      v1190->tot[i][j]=0;
    }
    v1190->multi[i]=0;
  }
  v1190->counter=0;
}



int ana_v1190(evtdata *evt, vector<v1190_hit> &v1190_hit_all,
              unsigned int *rawdata, unsigned int size, int field_id){

  // reference channel by geo address
  unsigned int v1190_ref_ch[V1190_MAX_GEO]=
    {112, 112, 112, 112, 112, 112, 112, 112,  // GR
       0, 127,   0, 127,   0, 127,   0, 127,  // LAS
       0, 127,   0, 127,   0, 127,   0, 127,  // LAS
     120, 120,  // GEO=24,25: SSD, ch120:GRTRIG, ch127:FCET(same as common trig)
     127, 127, 127, 127,
     15,  // GEO=30:PLA (GR TRIG)
     127}; 

  unsigned int rp=0;
  unsigned int data;
  int geo=V1190_MAX_GEO-1;
  unsigned int edge;
  unsigned int ich;
  unsigned int measure;
  unsigned long time_tag_raw=0;

  int i,j;
  
  //  int counter=0;
  
  int tmp_lead[V1190_MAX_GEO][N_V1190_CH][V1190_MAX_MULTI]={0};   // 128ch
  int tmp_trail[V1190_MAX_GEO][N_V1190_CH][V1190_MAX_MULTI]={0};  // 128ch  
  int lead_cnt[V1190_MAX_GEO][N_V1190_CH]={0};
  int trail_cnt[V1190_MAX_GEO][N_V1190_CH]={0};  

  int ref_lead[V1190_MAX_GEO]={0};
  int ref_flag[V1190_MAX_GEO]={0};  

  v1190_hit tmp_hit;
  
  // multiple module data is coming (not single module)
  while(rp<size/2){
    data=flip_32bit(ntohl(rawdata[rp]));
    rp++;
    
    // for one module 
    if(data>>27 == 0x08){  // global header
      geo=data&0x1f;
      //      printf("field=%d, geo=%d\n", field_id, geo);
      //      counter=(data>>5)&0x3fffff;
        
      while(1){
	data=flip_32bit(ntohl(rawdata[rp]));
	rp++;

	if(data>>27 == 0x10) break; // global trailer
	
	if((data>>27) == 0x1){  // TDC header
	}
	
	if((data>>27) == 0x03){ // TDC trailer
	}

	if((data>>27) == 0x4){ // TDC error
	}

	if(((data>>27)&0x1f) == 0x11){ // extended trigger time tag

	  if(geo==30){ // only for pla
	    time_tag_raw = (data)&0x07ffffff;
	    
	    if(evt->eve==0) evt->time_tag_first = time_tag_raw;

	    if(time_tag_raw < (evt->time_tag_previous)){
	      //	      printf("kuriagari, eve %d, %ld, %d \n",
	      //		     evt->eve, evt->time_tag_cor, 0x08000000);
	      evt->time_tag_cor += 0x08000000;
	      //	      printf("after: %ld\n", evt->time_tag_cor);
	    }

	    evt->time_tag_cor = (evt->time_tag_cor&0xfffffffff8000000)
	      +time_tag_raw;	    
	    
	    if(evt->eve==0) evt->time_tag_cor -= evt->time_tag_first;
	    evt->time_tag_previous = time_tag_raw;
	  }
	}
	
	if((data>>27) == 0x00){ // TDC measure
	  edge=(data>>26)&0x3f;
	  ich=(data>>19)&0x7f;
	  measure=data&0x0007ffff;
	  
	  if(edge==0){ // leading edge
	    if(lead_cnt[geo][ich]<V1190_MAX_MULTI){
	      tmp_lead[geo][ich][lead_cnt[geo][ich]]=measure;
	      //	      if(ich==ref_ch && ref_flag[geo]==0){
	      if(ich==v1190_ref_ch[geo] && ref_flag[geo]==0){		
		ref_lead[geo]=measure;
		ref_flag[geo]=1;
	      }
	      lead_cnt[geo][ich]++;
	    }
	    
	  }
	  if(edge==1){ // trailing edge
	    if(trail_cnt[geo][ich]<V1190_MAX_MULTI){
	      tmp_trail[geo][ich][trail_cnt[geo][ich]]=measure;
	      trail_cnt[geo][ich]++;
	    }
	    
	  } // end of edge==1
	  
	} // end of TDC measdure
	
      } // end of while(1)
    } // end of global header
  } // end of all data loop 
  
  // reanalysis 
  int total_hit=0;

  for(geo=0; geo<V1190_MAX_GEO; geo++){
    if(ref_flag[geo]==1){
      for(i=0; i<N_V1190_CH; i++){
	if(lead_cnt[geo][i]>0){
	  for(j=0; j<lead_cnt[geo][i]; j++){

	    tmp_hit.field=field_id;
	    //	    tmp_hit.counter=counter;
	    tmp_hit.counter=0;	    
	    tmp_hit.trail_raw=0;
	    tmp_hit.tot=0;  
	    tmp_hit.geo=geo;
	    tmp_hit.ch=i;
	    tmp_hit.lead_raw = tmp_lead[geo][i][j];
	    tmp_hit.lead_cor = tmp_lead[geo][i][j];
	    if(V1190_SUBTRACT_REF==1){
	      tmp_hit.lead_cor -= ref_lead[geo];	    
	    }
	    
	    if(lead_cnt[geo][i]==trail_cnt[geo][i]){
	      tmp_hit.trail_raw = tmp_trail[geo][i][j];
	      tmp_hit.tot = tmp_trail[geo][i][j] - tmp_lead[geo][i][j];
	    }
	    else{
	      tmp_hit.trail_raw = -1;
	      tmp_hit.tot = -1;	  
	    }
	    //	    printf("field=%d, geo=%d, ch=%d, lead_raw=%d, trail_raw=%d, lead_cor=%d, tot=%d, counter=%d\n",
	    //		   tmp_hit.field, tmp_hit.geo, tmp_hit.ch, tmp_hit.lead_raw,
	    //		   tmp_hit.trail_raw, tmp_hit.lead_cor, tmp_hit.tot, tmp_hit.counter);
	    
	    v1190_hit_all.push_back(tmp_hit);
	    total_hit++;
	    //	    printf("dec geo=%d\n", tmp_hit.geo);
	  }
	}
      } // end of multi-hit loop
    } // end of ref_flag
  } // end of geo loop

  return total_hit;
}


void init_grpla_data(pla_data *grpla){
  int i;
  for(i=0; i<N_GRPLA_CH; i++){
    grpla->adc[i]=0;
    grpla->tdc[i]=0;
  }
  for(i=0; i<N_GRPLA; i++){
    grpla->pos[i]=-10;
    grpla->de[i]=-10;    
  }
}

int ana_grpla_qdc(pla_data *grpla, unsigned int *rawdata, unsigned int size){
  unsigned int data;

  unsigned short data16;
  unsigned int ndata;
  int vsn;
  int ich,tmp_adc;
  
  data=(ntohl(rawdata[0]));
  data16=(data>>16)&0xffff;
  ndata=(data16>>11)&0xf;
  vsn=data16&0xff;
  
  unsigned int cnt=1;
  int rawdata_index;
  if(vsn==VSN_GR){ // analyze only GR
    data16=data&0xffff;
    ich=(data16>>11)&0x0f;
    tmp_adc=data16&0x7ff;
    if(ich<N_GRPLA_CH)grpla->adc[ich]=tmp_adc;
    
    while(cnt<ndata){
      rawdata_index=(int)(cnt/2+cnt%2);
      data=(ntohl(rawdata[rawdata_index]));
      if((cnt%2)==0) data16=data&0xffff;
      if((cnt%2)==1) data16=(data>>16)&0xffff;
      if(data16!=0){
	ich=(data16>>11)&0x0f;
	tmp_adc=data16&0x7ff;
	if(ich<N_GRPLA_CH)grpla->adc[ich]=tmp_adc;
      }
      
      cnt++;
    }
  }
  
  return vsn;
}

int ana_grpla_tdc(pla_data *grpla, unsigned int *rawdata, unsigned int size){
  unsigned int data;

  unsigned short data16;
  unsigned int ndata;
  int vsn;
  int ich,tmp_tdc;
  
  data=(ntohl(rawdata[0]));
  data16=(data>>16)&0xffff;
  ndata=(data16>>11)&0xf;
  vsn=data16&0xff;

  unsigned int cnt=1;
  int rawdata_index;

  data16=data&0xffff;
  ich=(data16>>11)&0x0f;
  tmp_tdc=data16&0x7ff;
  if(ich<N_GRPLA_CH)grpla->tdc[ich]=tmp_tdc;
    
  while(cnt<ndata){
    rawdata_index=(int)(cnt/2+cnt%2);
    data=(ntohl(rawdata[rawdata_index]));
    if((cnt%2)==0) data16=data&0xffff;
    if((cnt%2)==1) data16=(data>>16)&0xffff;
    if(data16!=0){
      ich=(data16>>11)&0x0f;
      tmp_tdc=data16&0x7ff;
      if(ich<N_GRPLA_CH)grpla->tdc[ich]=tmp_tdc;
    }
    
    cnt++;
  }
  return vsn;
}

int ana_fera(vector<fera_hit> &fera_hit_all, unsigned int *rawdata, unsigned int size,
	     int field, unsigned int region){
  unsigned int data;

  unsigned short data16;
  //  unsigned int ndata;
  int vsn;
  int ich,tmp_adc;
  
  data=(ntohl(rawdata[0]));
  data16=(data>>16)&0xffff;
  //  ndata=(data16>>11)&0x1f;
  vsn=data16&0xff;
  int qdc_tdc = 0;

  //  printf("header data16=%x\n", data16);
  
  if(region==0xd) qdc_tdc = 0;
  if(region==0xe) qdc_tdc = 1;  
  
  fera_hit fera_hit;
  fera_hit.field = field;
  fera_hit.qdc_tdc = qdc_tdc;
  fera_hit.vsn = vsn;
  
  unsigned int cnt=0;
  int rawdata_index;

  //  printf("header size=%d, field=%d, ndata=%d, vsn=%d\n", size, field, ndata, vsn);
  
  //  while(cnt<ndata){
  while(cnt<size-1){    
    rawdata_index=(int)(cnt/2+cnt%2);
    data=(ntohl(rawdata[rawdata_index]));
    if(cnt==0) data16=data&0xffff;
    if(cnt>0 && (cnt%2)==0) data16=data&0xffff;
    if(cnt>0 && (cnt%2)==1) data16=(data>>16)&0xffff;
    //    printf("data16=%x\n", data16);
    if(data16!=0){
      ich=(data16>>11)&0x0f;
      tmp_adc=data16&0x7ff;

      fera_hit.ch = ich;
      fera_hit.val = tmp_adc;
      //      printf("field=%d, vsn=%d\n", field, vsn);
      
      fera_hit_all.push_back(fera_hit);
    }
    
    cnt++;
  }
  
  return cnt;
}

int ana_inp(evtdata *evt, unsigned int *rawdata, unsigned int size,
	    int field){

  unsigned int data;
  unsigned short data16;

  if(evt->first_camac_inp==1 && field==FIELD_GR_NEW){
    //  printf("ana_inp, field=%d\n", field);
    data=(ntohl(rawdata[0]));
    data16=(data>>16)&0xffff;
    
    if(field==FIELD_GR_NEW){  // .gv data, CAMAC
      for(int i=0; i<N_INP; i++){
	evt->camac_inp[i] = (data16>>i)&0x1;
      }
    }
    evt->first_camac_inp=0;
  }
  return 0;
}

int ana_unixtime(evtdata *evt, unsigned int *rawdata, unsigned int size,
		 int field){

  unsigned int data;

  data=(ntohl(rawdata[0]));
  evt->unixtime = data;

  evt->unixtimesub = evt->unixtime - evt->unixtimefirst;
  
  if(evt->eve<1000){
    evt->unixtimesub = 0;
    evt->unixtimefirst = evt->unixtime;    
  }

  //  printf("ana_unixtime, field=%d\n", field);
  //  printf("time=%d\n", data);
  
  return 0;
}

int ana_camac_sca(evtdata *evt, unsigned int *rawdata, unsigned int size,
		  int field){
  unsigned int rp=0;
  unsigned int tmpdata;

  if(evt->camac_sca_flag<2){  // analysis first (spin-up) and second (spin-down)
    int ich=0;
    while(rp<size/2){
      tmpdata=flip_32bit(ntohl(rawdata[rp]));
      rp++;
      
      if(evt->camac_sca_flag==0) evt->camac_sca_up[ich] += tmpdata;
      if(evt->camac_sca_flag==1) evt->camac_sca_down[ich] += tmpdata;
      evt->camac_sca_total[ich] += tmpdata;
      
      ich++;
    }
  }
  evt->camac_sca_flag++;
  return 0;
}

int ana_vme_sca(evtdata *evt, unsigned int *rawdata, unsigned int size,
		  int field){
  unsigned int rp=0;
  unsigned int tmpdata;

  printf("ana_vme_sca\n");

  if(field==FIELD_PLA){
    int ich=0;
    while(rp<size/2){
      tmpdata=flip_32bit(ntohl(rawdata[rp]));
      rp++;

      if(ich<VME_SCA_CH) evt->vme_sca[ich] = tmpdata;
      ich++;
    }  
  }
  return 0;
}

int ana_vme_sca64(evtdata *evt, unsigned int *rawdata, unsigned int size,
		  int field){

  //  unsigned int tmpdata;
  unsigned int tmpdata12, tmpdata0, tmpdata3;
  //  unsigned short data16;
  int rawdata_index12, rawdata_index0, rawdata_index3;
  unsigned long int sca0,sca1,sca2,sca3;
  
  if(field==FIELD_PLA){
    //    if(evt->eve <10)printf("ana_vme_sca64*******************\n");

    // Read the Extended ID (should be 0x0200)
    //    tmpdata = (ntohl(rawdata[0]));
    //    data16=(tmpdata>>16)&0xffff;

    //    if(evt->eve <10) printf("ID=0x%04x\n", data16);
    
    // Read scaler data (divided into 16 bit data)
    int nch = (size-1)/4;
    int ich = 0;
    while(ich<nch){
      rawdata_index12=(int)(ich*2 + 1);
      rawdata_index0 =rawdata_index12 -1;
      rawdata_index3 =rawdata_index12 +1;      

      tmpdata12=(ntohl(rawdata[rawdata_index12]));
      tmpdata0 =(ntohl(rawdata[rawdata_index0]));
      tmpdata3 =(ntohl(rawdata[rawdata_index3]));      
      
      sca0 = tmpdata0  & 0xffff;
      sca1 = (tmpdata12 >>16) & 0xffff;      
      sca2 = tmpdata12  & 0xffff;
      sca3 = (tmpdata3 >>16) & 0xffff;

      if(ich<VME_SCA_CH){
	evt->vme_sca[ich] =
	  ((sca3<<48) & 0xffff000000000000) +
	  ((sca2<<32) & 0x0000ffff00000000) +
	  ((sca1<<16) & 0x00000000ffff0000) +
	  ((sca0)     & 0x000000000000ffff);
      }
      
      //      if(evt->eve <10) printf("ch%d: 0x %04lx %04lx %04lx %04lx\n", ich, sca3, sca2, sca1, sca0);
      //      if(evt->eve <10) printf("ch%d: 0x %016lx\n", ich, evt->vme_sca[ich]);      
      
      ich++;
    }    
    //    if(evt->eve <10)printf("*******************\n");
  }
  return 0;
}
