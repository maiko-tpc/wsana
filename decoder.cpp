#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include "moduledata.h"

#define V1190_SSD_GEO 24
#define V1190_MAX_GEO 32
#define VSN_GR 1

int VDC_OFFSET = 4000;

using namespace std;

unsigned int flip_32bit(unsigned int inp){
  unsigned int low = inp&0x0000ffff;
  unsigned int up  = (inp>>16)&0xffff;
  return low*0x00010000 + up;
}

int get_vdc_wire(int geo, int ch){
  if(geo==0 || geo==4){
    if(ch>=  0 && ch<= 15) return ch;
    if(ch>= 16 && ch<= 31) return ch+16;
    if(ch>= 32 && ch<= 47) return ch-16;
    if(ch>= 48 && ch<= 63) return ch;
    if(ch>= 64 && ch<= 79) return ch;
    if(ch>= 80 && ch<= 95) return ch+16;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==1 || geo==5){
    if(ch>=  0 && ch<= 15) return ch+80;
    if(ch>= 16 && ch<= 31) return ch+96;
    if(ch>= 32 && ch<= 47) return ch+96;
    if(ch>= 48 && ch<= 63) return ch+112;
    if(ch>= 64 && ch<= 79) return ch+80;
    if(ch>= 80 && ch<= 95) return ch+96;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==2 || geo==6){
    if(ch>=  0 && ch<= 15) return ch+16;
    if(ch>= 16 && ch<= 31) return ch+32;
    if(ch>= 32 && ch<= 47) return ch;
    if(ch>= 48 && ch<= 63) return ch+16;
    if(ch>= 64 && ch<= 79) return ch+16;
    if(ch>= 80 && ch<= 95) return ch+32;
    if(ch>= 96 && ch<=111) return ch-96;
    if(ch> 112 && ch<=127) return -1;
  }

  if(geo==3 || geo==7){
    if(ch>=  0 && ch<= 15) return ch+96;
    if(ch>= 16 && ch<= 31) return ch+112;
    if(ch>= 32 && ch<= 47) return ch+112;
    if(ch>= 48 && ch<= 63) return ch+128;
    if(ch>= 64 && ch<= 79) return ch+96;
    if(ch>= 80 && ch<= 95) return ch+112;
    if(ch>= 96 && ch<=111) return -1;
    if(ch> 112 && ch<=127) return -1;
  }

  return -1;
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



int ana_v1190(vector<v1190_hit> &v1190_hit_all,
              unsigned int *rawdata, unsigned int size, int field_id){
  unsigned int rp=0;
  unsigned int data;
  int geo=V1190_MAX_GEO-1;
  //  unsigned int itdc=0;
  unsigned int edge;
  unsigned int ich;
  unsigned int measure;
  int i,j;
  
  int counter=0;

  v1190_hit tmp_hit;
  
  int tmp_lead[V1190_MAX_GEO][N_V1190_CH][V1190_MAX_MULTI]={0};  // 128ch
  int tmp_trail[V1190_MAX_GEO][N_V1190_CH][V1190_MAX_MULTI]={0};  // 128ch  
  int lead_cnt[V1190_MAX_GEO][N_V1190_CH]={0};
  int trail_cnt[V1190_MAX_GEO][N_V1190_CH]={0};  

  int ref_lead[V1190_MAX_GEO]={0};
  int ref_flag[V1190_MAX_GEO]={0};  

  while(rp<size/2){
    data=flip_32bit(ntohl(rawdata[rp]));
    rp++;
    
    if(data>>27 == 0x08){  // global header
      geo=data&0x1f;
      counter=(data>>5)&0x3fffff;
    }
        
    while(1){
      data=flip_32bit(ntohl(rawdata[rp]));
      rp++;
      if(data>>27 == 0x10) break; // global trailer
      
      if((data>>27) == 0x1){  // TDC header
	//	  itdc = (data>>24)&0x03;
	
	while(1){
	  data=flip_32bit(ntohl(rawdata[rp]));
	  rp++;
	  if((data>>27) == 0x03){
	    break; // TDC trailer
	  }
	  
	  if((data>>27) == 0x4){ // TDC error
	    break;
	  }
	  
	  if((data>>27) == 0x00){ // TDC measure
	    edge=(data>>26)&0x3f;
	    ich=(data>>19)&0x7f;
	    measure=data&0x0007ffff;
	    
	    if(edge==0){ // leading edge
	      if(lead_cnt[geo][ich]<V1190_MAX_MULTI){
		tmp_lead[geo][ich][lead_cnt[geo][ich]]=measure;
	      }
	      lead_cnt[geo][ich]++;
	      if(ich==127 && ref_flag[geo]==0){
		ref_lead[geo]=measure;
		ref_flag[geo]=1;
	      }
	    }
	    if(edge==1){ // trailing edge
	      if(trail_cnt[geo][ich]<V1190_MAX_MULTI){
		tmp_lead[geo][ich][trail_cnt[geo][ich]]=measure;
	      }
	      trail_cnt[geo][ich]++;
	    }
	    
	  }
	  
	}
      } 
    }  
  }
  
  // reanalysis
  int total_hit=0;

  tmp_hit.field=field_id;
  tmp_hit.counter=counter;
  tmp_hit.trail_raw=0;
  tmp_hit.tot=0;  
  
  for(geo=0; geo<V1190_MAX_GEO; geo++){
    if(ref_flag[geo]==1){
      for(i=0; i<N_V1190_CH; i++){
	if(lead_cnt[geo][i]>0){
	  for(j=0; j<lead_cnt[geo][i]; j++){
	    tmp_hit.geo=geo;
	    tmp_hit.ch=i;
	    tmp_hit.lead_raw = tmp_lead[geo][i][j];
	    tmp_hit.lead_cor = tmp_lead[geo][i][j]-ref_lead[geo];		
	    
	    if(lead_cnt[geo][i]==trail_cnt[geo][i]){
	      tmp_hit.trail_raw = tmp_trail[geo][i][j];
	      tmp_hit.tot = tmp_trail[geo][i][j] - tmp_lead[geo][i][j];
	    }
	    else{
	      tmp_hit.trail_raw = -1;
	      tmp_hit.tot = -1;	  
	    }
	    v1190_hit_all.push_back(tmp_hit);
	    total_hit++;
	    //	    printf("dec geo=%d\n", tmp_hit.geo);
	  }
	}
      } // end of multi-hit loop
    } // end of v1190 ch loop
  } // end of geo loop
  return total_hit;
}


void init_grpla_data(grpla_data *grpla){
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

int ana_grpla_qdc(grpla_data *grpla, unsigned int *rawdata, unsigned int size){
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

int ana_grpla_tdc(grpla_data *grpla, unsigned int *rawdata, unsigned int size){
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

