#include <stdio.h>
#ifndef read_information_h
#define read_information_h

typedef struct xml_module{
	int sign;
	char dir[256];
	char *file_name[255];
	char id[255];
	char *module_id[255];
	char name[256];
}module;

typedef struct xml_hal{
	int sign;
	char dir[256];
	char *file_name[255];
	char id[255];
	char *moduleid[255];
	char name[256];
}hal;

int read_module_file_information(struct xml_module* module);
int read_hal_file_information(struct xml_hal *hal);



#endif
