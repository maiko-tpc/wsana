#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "moduledata.h"

#define V1190_SSD_GEO 24

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

int ana_v1190(v1190_data *v1190, unsigned int *rawdata, unsigned int size){
  unsigned int rp=0;
  unsigned int data;
  int geo=-1;
  unsigned int itdc;
  unsigned int edge;
  unsigned int ich;
  unsigned int measure;
  int i,j;
  
  int tmp_multi[N_V1190_CH]={0};
  
  while(rp<size/2){
    data=flip_32bit(ntohl(rawdata[rp]));
    rp++;

    if(data>>27 == 0x08){  // global header
      geo=data&0x1f;
      if(geo==V1190_SSD_GEO) v1190->counter=(data>>5)&0x3fffff;
    }

    if(geo!=V1190_SSD_GEO) return geo;

    /* analyze only the ssd data */
    if(geo==V1190_SSD_GEO){

      while(1){
	data=flip_32bit(ntohl(rawdata[rp]));
	rp++;
	if(data>>27 == 0x10) break; // global trailer
	
	if((data>>27) == 0x1){  // TDC header
	  itdc = (data>>24)&0x03;
	  
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
	      }
	      if(edge==1){ // trailing edge
		v1190->trail[ich][tmp_multi[ich]]=measure;
	      }
	      tmp_multi[ich]++;
	    }

	  }
	} 
      } 
    } // end of if(geo==V1190_SSD_GEO)
  } // end of while(rp<size/2)

  for(i=0; i<N_V1190_CH; i++){
    for(j=0; j<tmp_multi[i]-1; j++){
      v1190->tot[i][j] = v1190->trail[i][j] - v1190->lead[i][j];
    }
    v1190->multi[i]=tmp_multi[i];
  }

  //  if(v1190->multi[V1190_REF_CH]==0){
  //    printf("No V1190 ref. hit, geo= %d\n", geo);
  //  }
  
  return geo;
}
