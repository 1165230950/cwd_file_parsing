#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"
#include "xml.h"
#include "html.h"

int main(int argc, char *argv[])
{
	/*========================检查输入=========================*/
	check_input;

	/*======================判断文件格式=======================*/
	int len = strlen(argv[1]);
	if(!strcmp(argv[1]+len-4, xml))
	{
		printf("input xml file\n");
		message_xml *p = malloc(sizeof(message_xml));
		p->buf = malloc(sizeof(argv[1]));
		strcpy(p->buf, argv[1]);
		result_xml *q = parsing_xml(p);
		if(q == NULL)
		{
			printf("文件解析失败\n");
			exit(0);
		}
		int i;
		for(i=0; i<(q->line); i++)
		{
			printf("line =%d %s\n", i, q->value[i]);
		}
	}else if(!strcmp(argv[1]+len-5, html)){
		printf("input html file\n");
		message_html *p = malloc(sizeof(message_html));
		p->buf = malloc(sizeof(argv[1]));
		strcpy(p->buf, argv[1]);
		result_html *q = parsing_html(p);
	}else{
		printf("Unable to parse this file type\n");
		exit(0);
	}

	printf("parsing OK\n");
	return 0;
}
