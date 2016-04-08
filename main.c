#include <stdio.h>
#include <stdlib.h>
#include "head.h"
#include "txt.h"

int main(int argc, char *argv[])
{
	/*========================检查输入=========================*/
	check_input;

	/*======================判断文件格式=======================*/
	int len = strlen(argv[1]);
	if(!strcmp(argv[1]+len-4, format[0]))
	{
		message *p = malloc(message);
		result *q = parsing_xml(p);
	}


	return 0;
}
