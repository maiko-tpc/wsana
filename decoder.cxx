#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <arpa/inet.h>
#include "moduledata.h"

#define V1190_SSD_GEO 24
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
    if(ch>+112 && ch<=127) return -1;
  }

  if(geo==1 || geo==5){
    if(ch>=  0 && ch<= 15) return ch+80;
    if(ch>= 16 && ch<= 31) return ch+96;
    if(ch>= 32 && ch<= 47) return ch+96;
    if(ch>= 48 && ch<= 63) return ch+112;
    if(ch>= 64 && ch<= 79) return ch+80;
    if(ch>= 80 && ch<= 95) return ch+96;
    if(ch>= 96 && ch<=111) return -1;
    if(ch>+112 && ch<=127) return -1;
  }

  if(geo==2 || geo==6){
    if(ch>=  0 && ch<= 15) return ch+16;
    if(ch>= 16 && ch<= 31) return ch+32;
    if(ch>= 32 && ch<= 47) return ch;
    if(ch>= 48 && ch<= 63) return ch+16;
    if(ch>= 64 && ch<= 79) return ch+16;
    if(ch>= 80 && ch<= 95) return ch+32;
    if(ch>= 96 && ch<=111) return ch-96;
    if(ch>+112 && ch<=127) return -1;
  }

  if(geo==3 || geo==7){
    if(ch>=  0 && ch<= 15) return ch+96;
    if(ch>= 16 && ch<= 31) return ch+112;
    if(ch>= 32 && ch<= 47) return ch+112;
    if(ch>= 48 && ch<= 63) return ch+128;
    if(ch>= 64 && ch<= 79) return ch+96;
    if(ch>= 80 && ch<= 95) return ch+112;
    if(ch>= 96 && ch<=111) return -1;
    if(ch>+112 && ch<=127) return -1;
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

int ana_v1190(v1190_data *v1190, vector<grvdc_data> &grvdc,
              unsigned int *rawdata, unsigned int size){
  unsigned int rp=0;
  unsigned int data;
  int geo=-1;
  //  unsigned int itdc=0;
  unsigned int edge;
  unsigned int ich;
  unsigned int measure;
  int i,j;
  
  int tmp_multi[N_V1190_CH]={0};

  vector<grvdc_data> tmp_vdc_vec;
  tmp_vdc_vec.clear();

  unsigned int tmp_plane, tmp_wire;

  grvdc_data tmp_grvdc;
  grvdc_data tmp_data;
  
  int vdc_ref[8]={0};
  
  while(rp<size/2){
    data=flip_32bit(ntohl(rawdata[rp]));
    rp++;

    if(data>>27 == 0x08){  // global header
      geo=data&0x1f;
      if(geo==V1190_SSD_GEO) v1190->counter=(data>>5)&0x3fffff;
    }

    //    if(geo!=V1190_SSD_GEO) return geo;

    /* analyze only the ssd data */
    if(geo==V1190_SSD_GEO){

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
	      if(tmp_multi[ich]>=V1190_MAX_MULTI) tmp_multi[ich]=V1190_MAX_MULTI-1;
	      if(edge==0){ // leading edge
		v1190->lead[ich][tmp_multi[ich]]=measure;
		tmp_multi[ich]++;
	      }
	      if(edge==1){ // trailing edge
		v1190->trail[ich][tmp_multi[ich]]=measure;
	      }
	    }

	  }
	} 
      } 
    } // end of if(geo==V1190_SSD_GEO)


    /* analyze GRVDC data */
    if(geo<8){
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
	      tmp_grvdc.geo=10;
	      tmp_grvdc.plane=10;
	      tmp_grvdc.wire=1000;
	      tmp_grvdc.lead_raw=-1000;
	      tmp_grvdc.lead_cor=-1000;

	      edge=(data>>26)&0x3f;
	      ich=(data>>19)&0x7f;
	      measure=data&0x0007ffff;

	      tmp_plane=(int)(geo/2);
	      tmp_wire=get_vdc_wire(geo, ich);

	      if(edge==0){ // leading edge
		tmp_grvdc.geo=geo;
		tmp_grvdc.plane=tmp_plane;
		tmp_grvdc.wire=tmp_wire;
		tmp_grvdc.lead_raw=measure;		
		tmp_vdc_vec.push_back(tmp_grvdc);
		if(ich==V1190_REF_CH){
		  vdc_ref[geo]=measure;
		}
	      }
	      if(edge==1){ // trailing edge
	      }
	    }
	    
	  }
	} 
      } 
    } // end of if(geo<8)
    
  } // end of while(rp<size/2)

  // reanalysis
  for(i=0; i<N_V1190_CH; i++){
    for(j=0; j<tmp_multi[i]-1; j++){
      v1190->tot[i][j] = v1190->trail[i][j] - v1190->lead[i][j];
    }
    v1190->multi[i]=tmp_multi[i];
  }

  for(i=0; i<(int)(tmp_vdc_vec.size()); i++){
    tmp_data.geo = tmp_vdc_vec[i].geo;
    tmp_data.plane = tmp_vdc_vec[i].plane;
    tmp_data.wire = tmp_vdc_vec[i].wire;
    tmp_data.lead_raw = tmp_vdc_vec[i].lead_raw;
    tmp_data.lead_cor = tmp_vdc_vec[i].lead_raw - vdc_ref[tmp_data.geo]+VDC_OFFSET;
    grvdc.push_back(tmp_data);
  }
  
  return geo;
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

