unsigned int flip_32bit(unsigned int inp);

void init_madc32_data(madc32_data *madc);
void ana_madc32(madc32_data *madc,
		unsigned int *rawdata, unsigned int size);

void init_v1190_data(v1190_data *v1190);
int ana_v1190(v1190_data *v1190, unsigned int *rawdata, unsigned int size);
