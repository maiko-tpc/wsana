#ifndef _MODULEDATA_H_
#define _MODULEDATA_H_

#define N_MADC 2
#define N_V1190 1
#define V1190_MAX_MULTI 16
#define V1190_REF_CH 127
#define N_VDCPLANE 4
#define PLANE_SIZE 256

const int N_MADC_CH = N_MADC*32;
const int N_V1190_CH = 128;
const int N_GRPLA_CH = 16;
const int N_GRPLA    = 2;
const int MAX_VDC_TDC = 5000;

typedef struct madc32_data{
  unsigned int wrdcnt[N_MADC];
  unsigned int adc[N_MADC_CH];
  unsigned int counter[N_MADC];
} madc32_data;

typedef struct v1190_hit{
  int field; // 0:GR+SSD+pla
  unsigned int geo;
  unsigned int ch;
  int lead_raw;
  int trail_raw;  
  int lead_cor;
  int tot;
  unsigned int counter;
} v1190_hit;

typedef struct v1190_data{
  unsigned int lead[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int trail[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int tot[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int multi[N_V1190_CH];  
  unsigned int counter;
} v1190_data;

typedef struct grpla_data{
  unsigned int adc[N_GRPLA_CH];
  unsigned int tdc[N_GRPLA_CH];
  float pos[N_GRPLA];
  float de[N_GRPLA];
} grpla_data;

typedef struct grvdc_data{
  unsigned int geo;
  unsigned int plane;
  int wire;
  int lead_raw;
  int lead_cor;
  unsigned int clst_flag;
} grvdc_data;


#endif
