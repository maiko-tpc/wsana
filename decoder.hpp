#ifndef DECODER_HPP_
#define DECODER_HPP_

#include "moduledata.h"
#include "evtdata.h"

using namespace std;

unsigned int flip_32bit(unsigned int inp);

void init_madc32_data(madc32_data *madc);
void init_mqdc32_data(mqdc32_data *mqdc);
void ana_madc32(madc32_data *madc,
		unsigned int *rawdata, unsigned int size);
void ana_mxdc32(evtdata *,
		unsigned int *rawdata, unsigned int size);
void ana_mxdc32(vector<mxdc32_hit> &mxdc32_hit_all, unsigned int *rawdata, unsigned int size, int field_id);
void init_v1190_data(v1190_data *v1190);
int ana_v1190(vector<v1190_hit> &v1190_hit_all,
	      unsigned int *rawdata, unsigned int size, int field_id);

void init_grpla_data(pla_data *grpla);
int ana_grpla_qdc(pla_data *grpla, unsigned int *rawdata, unsigned int size);
int ana_grpla_tdc(pla_data *grpla, unsigned int *rawdata, unsigned int size);

int ana_fera(vector<fera_hit> &fera_hit_all, unsigned int *rawdata, unsigned int size,
	     int field, unsigned int region);

int ana_inp(evtdata *evt, unsigned int *rawdata, unsigned int size,
	    int field);

#endif // end of include guard
