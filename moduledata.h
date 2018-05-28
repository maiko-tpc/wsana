#define N_MADC 2
#define N_V1190 1
#define V1190_MAX_MULTI 4
#define V1190_REF_CH 127

const int N_MADC_CH = N_MADC*32;
const int N_V1190_CH = 128;

typedef struct madc32_data{
  unsigned int wrdcnt[N_MADC];
  unsigned int adc[N_MADC_CH];
  unsigned int counter[N_MADC];
} madc32_data;

typedef struct v1190_data{
  unsigned int lead[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int trail[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int tot[N_V1190_CH][V1190_MAX_MULTI];
  unsigned int multi[N_V1190_CH];  
  unsigned int counter;
} v1190_data;
