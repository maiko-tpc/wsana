#define N_MADC 3
#define N_V1190 1

const int N_MADC_CH = N_MADC*32;

typedef struct madc32_data{
  unsigned int wrdcnt[N_MADC];
  unsigned int adc[N_MADC_CH];
  unsigned int counter[N_MADC];
} madc32_data;
