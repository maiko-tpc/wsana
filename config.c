/*
  config.c ---- treat configuration file
  Copyright (C) 2016 Research Center for Nuclear Physics, Osaka University
  Author: A. Tamii, Rsearch Center for Nuclear Physics
  Version 1.00 11-APR-2016 by A. Tamii
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <regex.h>

#include "config.h"

char *title = "config.c ver 1.00 11-APR-2016";
#if 0
char *title = "config.c ver 1.00 11-APR-2016";
#endif


static regex_t preg_num1;
static regex_t preg_num2;
static regex_t preg_num3;

int config_debug_mode = 0;

typedef struct Parameter {
  char *name;
  char *index;
  char *value;
} Parameter_t, *Parameter_p;

#define Max_N_Parameters 65536
static Parameter_p *parameters;
static int n_parameters;

static int init_flag=0;

static void get_matched_string(char *dst, char *str, regmatch_t pmatch[], int index){
	int pos = pmatch[index].rm_so;
	int len = pmatch[index].rm_eo-pmatch[index].rm_so;
	strncpy(dst, &str[pos], len);
	dst[len] = 0x00;
}

char* convert_index(char *index){
	char *new_index;
	char str[256];
	char *endptr;
	long l_num;
	double d_num;
	regmatch_t pmatch[2];

	if(index==NULL) return NULL;
	if(!regexec(&preg_num1, index, 2, pmatch, 0)){
		get_matched_string(str, index, pmatch, 1);
		l_num = strtol(str, &endptr, 10);
		if(!errno && *endptr==0){
			new_index = malloc(256);
			sprintf(new_index, "%ld", l_num);
			return new_index;
		}else{
			return strdup(index);
		}
	}
	if(!regexec(&preg_num2, index, 2, pmatch, 0)){
		get_matched_string(str, index, pmatch, 1);
		l_num = strtol(&str[2], &endptr, 16);
		if(!errno && *endptr==0){
			new_index = malloc(256);
			sprintf(new_index, "%ld", l_num);
			return new_index;
		}else{
			return strdup(index);
		}
	}
	if(!regexec(&preg_num3, index, 2, pmatch, 0)){
		get_matched_string(str, index, pmatch, 1);
		d_num = strtod(str, &endptr);
		if(!errno && *endptr==0){
			new_index = malloc(256);
			sprintf(new_index, "%lf", d_num);
			return new_index;
		}else{
			return strdup(index);
		}
	}
	return strdup(index);
}

void dump_parameters(){
	int i;
	for(i=0; i<n_parameters; i++){
		if(parameters[i]->index==NULL){
			fprintf(stderr, "%s=%s\n", parameters[i]->name, parameters[i]->value);
		}else{
			fprintf(stderr, "%s[%s]=%s\n", parameters[i]->name, parameters[i]->index, parameters[i]->value);
		}
	}
}

void free_parameter(Parameter_p parameter){
	free(parameter->name);
	if(parameter->index) 	free(parameter->index);
	free(parameter->value);
	if(parameter) free(parameter);
}

static int search_parameter_index(char *name, char *index){
	// a hash table may be used in future
	int i;
	for(i=0; i<n_parameters; i++){
		if(!strcasecmp(parameters[i]->name, name)){
			if(index==NULL && parameters[i]->index==NULL) return i;
			if(index==NULL) continue;
			if(parameters[i]->index==NULL) continue;
			if(!strcasecmp(parameters[i]->index, index)) return i;
		}
	}
	return -1;
}
static Parameter_p search_parameter(char *name, char *index){
	int i;
	i = search_parameter_index(name, index);
	return i<0 ? (Parameter_p)NULL : parameters[i];
}

static void register_parameter_if_new(Parameter_p parameter){
	int i;
	if(config_debug_mode>=1){
		if(parameter->index==(char*)NULL){
			fprintf(stderr, "%s=%s\n", parameter->name, parameter->value);
		}else{
			fprintf(stderr, "%s[%s]=%s\n", parameter->name, parameter->index, parameter->value);
		}
	}
		
	i = search_parameter_index(parameter->name, parameter->index);
	if(i<0){
		parameters[n_parameters++] = parameter;
	}else{
		free_parameter(parameters[i]);
		parameters[i] = parameter;
		if(parameter->index){
			fprintf(stderr, "The parameter '%s[%s]' has been superceded.\n", parameter->name, parameter->index);
		}else{
			fprintf(stderr, "The parameter '%s' has been superceded.\n", parameter->name);
		}
	}
}

static void register_parameter(char *name, char *index, char *value){
	Parameter_p parameter = (Parameter_p)malloc(sizeof(Parameter_t));
	parameter->name = strdup(name);
	parameter->index = convert_index(index);
	parameter->value = strdup(value);
	register_parameter_if_new(parameter);
}

static void config_init(){
	if(init_flag) return;
	parameters = (Parameter_p*)malloc(sizeof(Parameter_p)*Max_N_Parameters);
	n_parameters = 0;

	regcomp(&preg_num1, "^[ \t]*([+-]?[0-9]+)[ \t]*$", REG_EXTENDED);
	regcomp(&preg_num2, "^[ \t]*(0x[0-9a-fA-F]+)[ \t]*$", REG_EXTENDED);
	regcomp(&preg_num3, "^[ \t]*([+-]?[0-9]+\\.[0-9]+)[ \t]*$", REG_EXTENDED);
	init_flag = 1;
}


void config_set_debug_mode(int debug_mode){
	config_debug_mode = debug_mode;
}

#define MAX_STR_SIZE 1024
int config_read_file(char *filename){
	char str[MAX_STR_SIZE];
	char *line;
	char name[256];
	char value[256];
	char index[256];
	regmatch_t pmatch[10];
	int i_line;
	
	regex_t pregc1;
	regex_t pregc2;
	regex_t pregc3;
	regex_t pregdef1;
	regex_t pregdef2;
	regex_t pregdef3;
	regex_t pregdefi1;
	regex_t pregdefi2;
	regex_t pregdefi3;

	config_init();

	regcomp(&pregc1, "^[ \t\r\n]*$", REG_EXTENDED);
	regcomp(&pregc2, "^[ \t]*[#;]", REG_EXTENDED);
	regcomp(&pregc3, "^[ \t]*//", REG_EXTENDED);
	regcomp(&pregdef1, "^[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t\r\n]*$", REG_EXTENDED);
	regcomp(&pregdef2, "^[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*[#;]", REG_EXTENDED);
	regcomp(&pregdef3, "^[ \t]*([A-Za-z0-9_]+)[ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*//", REG_EXTENDED);
	regcomp(&pregdefi1, "^[ \t]*([A-Za-z0-9_]+)[ \t]*\\[[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*\\][ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t\r\n]*$", REG_EXTENDED);
	regcomp(&pregdefi2, "^[ \t]*([A-Za-z0-9_]+)[ \t]*\\[[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*\\][ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*[#;]", REG_EXTENDED);
	regcomp(&pregdefi3, "^[ \t]*([A-Za-z0-9_]+)[ \t]*\\[[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*\\][ \t]*=[ \t]*([+-]?[A-Za-z0-9_\\.]+)[ \t]*//", REG_EXTENDED);

						 
	FILE *fd = fopen(filename, "r");
	if(fd==(FILE*)NULL){
		fprintf(stderr, "config_read: could not open file '%s'.\n", filename);
		return -1;
	}
	for(i_line=1;;i_line++){
		line = fgets(str, MAX_STR_SIZE, fd);
		if(line==(char*)NULL) break;
		
		if(!regexec(&pregc1, line, 0, pmatch, 0)
			 || !regexec(&pregc2, line, 0, pmatch, 0)
			 || !regexec(&pregc3, line, 0, pmatch, 0)){
			// comment line
		}else if(!regexec(&pregdef1, line, 3, pmatch, 0) 
						 || !regexec(&pregdef2, line, 3, pmatch, 0)
						 || !regexec(&pregdef3, line, 3, pmatch, 0)){
			get_matched_string(name, line, pmatch, 1);
			get_matched_string(value, line, pmatch, 2);
			register_parameter(name, NULL, value);
		}else if(!regexec(&pregdefi1, line, 4, pmatch, 0) 
						 || !regexec(&pregdefi2, line, 4, pmatch, 0)
						 || !regexec(&pregdefi3, line, 4, pmatch, 0)){
		  get_matched_string(name, line, pmatch, 1);
		  get_matched_string(index, line, pmatch, 2);
			get_matched_string(value, line, pmatch, 3);
			register_parameter(name, index, value);
		}else{
			fprintf(stderr, "parse error in %d of file '%s':\n", i_line, filename);
			fprintf(stderr, "%s", line);
		}
	}
	fclose(fd);
	
	if(config_debug_mode){
		dump_parameters();
	}

	return 0;
}

static long get_l_value(char *s_value, long default_value){
	char str[256];
	char *endptr;
	long l_num;
	double d_num;
	regmatch_t pmatch[2];

	if(s_value==NULL) return default_value;
	if(!regexec(&preg_num1, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		l_num = strtol(str, &endptr, 10);       // signed integernumber conversion to long
		if(!errno && *endptr==0) return l_num;
		fprintf(stderr, "Error in number conversion of '%s'. Using the default value %ld.\n", s_value, default_value);
		return default_value;
	}
	if(!regexec(&preg_num2, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		l_num = strtoul(&str[2], &endptr, 16);  // hexadecimal number conversion to unsigned long
		if(!errno && *endptr==0) return l_num;
		fprintf(stderr, "Error in number conversion of '%s'. Using the default value %ld.\n", s_value, default_value);
		return default_value;
	}
	if(!regexec(&preg_num3, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		d_num = strtod(str, &endptr);           // real number conversion to double
		if(!errno && *endptr==0) return (long)d_num;
		fprintf(stderr, "Error in number conversion of '%s'. Using the default value %ld.\n", s_value, default_value);
		return default_value;
		}
		fprintf(stderr, "Error in number conversion of '%s'. Using the default value %ld.\n", s_value, default_value);
	return default_value;
}

static double get_d_value(char *s_value, double default_value){
	char str[256];
	char *endptr;
	long l_num;
	double d_num;
	regmatch_t pmatch[2];

	if(s_value==NULL) return default_value;
	if(!regexec(&preg_num1, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		l_num = strtol(str, &endptr, 10);
		//		printf("errno=%d\n", errno);
		//		printf("endptr=%d\n", *endptr);		
		//		if(!errno && *endptr==0) return (double)l_num;
		if(*endptr==0) return (double)l_num;		
		fprintf(stderr,
						"Error in number conversion of '%s'. Using the default value %lf.\n",
						s_value, default_value);
		return default_value;
	}
	if(!regexec(&preg_num2, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		l_num = strtol(&str[2], &endptr, 16);
		if(!errno && *endptr==0) return (double)l_num;
		fprintf(stderr,
						"Error in number conversion of '%s'. Using the default value %lf.\n",
						s_value, default_value);
		return default_value;
	}
	if(!regexec(&preg_num3, s_value, 2, pmatch, 0)){
		get_matched_string(str, s_value, pmatch, 1);
		d_num = strtod(str, &endptr);
		if(!errno && *endptr==0) return d_num;
		return d_num;		
		fprintf(stderr,
						"Error in number conversion of '%s'. Using the default value %lf.\n",
						s_value, default_value);
		return default_value;
		}
		fprintf(stderr,
						"Error in number conversion of '%s'. Using the default value %lf.\n",
						s_value, default_value);
	return default_value;
}

long config_get_l_value(char *name, int i_index, long default_value){
	Parameter_p parameter;
	char index[256];

	config_init();

	sprintf(index, "%d", i_index);
	parameter = search_parameter(name, index);
	if(parameter==NULL){
		parameter = search_parameter(name, NULL);
		if(parameter==NULL) return default_value;
		return get_l_value(parameter->value, default_value);
	}
	return get_l_value(parameter->value, default_value);
}

double config_get_d_value(char *name, int i_index, double default_value){
	Parameter_p parameter;
	char index[256];

	config_init();

	sprintf(index, "%d", i_index);
	parameter = search_parameter(name, index);
	if(parameter==NULL){
		parameter = search_parameter(name, NULL);
		if(parameter==NULL) return default_value;
		return get_d_value(parameter->value, default_value);
	}
	return get_d_value(parameter->value, default_value);
}

char *config_get_s_value(char *name, int i_index, char *default_value){
	Parameter_p parameter;
	char index[256];

	config_init();

	sprintf(index, "%d", i_index);
	parameter = search_parameter(name, index);
	if(parameter==NULL){
		parameter = search_parameter(name, NULL);
		if(parameter==NULL){
			return default_value;
		}
	}
	return parameter->value;
}

int config_get_b_value(char *name, int i_index, int default_value){
	//	Parameter_p parameter;
	char *sval;

	sval = config_get_s_value(name, i_index, "true");
	if(!strcasecmp(sval,"1")) return 1;
	if(!strcasecmp(sval,"true")) return 1;
	if(!strcasecmp(sval,"t")) return 1;
	if(!strcasecmp(sval,"yes")) return 1;
	if(!strcasecmp(sval,"ok")) return 1;

	if(!strcasecmp(sval,"0")) return 0;
	if(!strcasecmp(sval,"false")) return 0;
	if(!strcasecmp(sval,"f")) return 0;
	if(!strcasecmp(sval,"no")) return 0;

	fprintf(stderr, "Unknown argument for %s[%d]: %s. The default value will be used: %s.\n", name, i_index, sval, default_value ? "true" : "false");
	return default_value;
}

/*
Local Variables:
mode: C
tab-width: 2
End:
*/

