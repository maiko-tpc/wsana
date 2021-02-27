#ifndef MODULEDATA_H_
#define MODULEDATA_H_

#define N_MADC 2
#define N_MQDC 2
#define N_V1190 1
#define V1190_MAX_MULTI 2
#define N_VDCPLANE 4
#define PLANE_SIZE 256

#define MQDC_START_GEO 10
#define MADC_START_GEO 0
#define PLA_V1190_GEO 30


const int N_MADC_CH = N_MADC*32;
const int N_MQDC_CH = N_MQDC*32;
const int N_V1190_CH = 128;
const int N_GRPLA_CH = 16;
const int N_GRPLA    = 2;
const int MAX_VDC_TDC = 5000;

const int VDC_OFFSET = 4000;

typedef struct madc32_data{
  unsigned int wrdcnt[N_MADC];
  unsigned int adc[N_MADC_CH];
  unsigned int counter[N_MADC];
} madc32_data;

typedef struct mqdc32_data{
  unsigned int wrdcnt[N_MQDC];
  unsigned int adc[N_MQDC_CH];
  unsigned int counter[N_MQDC];
} mqdc32_data;

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
  unsigned int fqdc[N_GRPLA_CH];
  unsigned int ftdc[N_GRPLA_CH];  
  unsigned int vqdc[N_GRPLA_CH];
  unsigned int vtdc[N_GRPLA_CH];  
} grpla_data;

typedef struct grvdc_data{
  unsigned int geo;
  int ray;
  unsigned int plane;
  int wire;
  int lead_raw;
  int lead_cor;
  unsigned int clst_flag;
  float dlen; // drift length
} grvdc_data;


#endif
