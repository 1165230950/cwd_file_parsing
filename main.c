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
	int i;
	module_message module;
	hal_message hal;
	module.max = 0;
	hal.max = 0;
	for(i=1; i<argc; i++)
	{
		int len = strlen(argv[i]);
printf("file_name= %s\n", argv[i]);
		printf("name= %s\n", argv[i]);
		if(!strcmp(argv[i]+len-4, xml))
		{
			printf("input xml file\n");
			message_xml *p = malloc(sizeof(message_xml));
			p->buf = malloc(sizeof(argv[i]));
			strcpy(p->buf, argv[i]);
			result_xml *q = parsing_xml(p);
			if(q == NULL)
			{
				printf("文件解析失败\n");
				exit(0);
			}
			if(q->sign == 2)
			{
				module.max = q->line;
				int i;
				for(i=0; i<q->line; i++)
				{
					module.name[i] = malloc(255);
					if(module.name[i] == NULL)
					{
						printf("(main)module->name[%d] malloc failed\n", i);
						exit(0);
					}
					module.id[i] = malloc(255);
					if(module.id[i] == NULL)
					{
						printf("(main)module->id[%d] malloc failed\n", i);
						exit(0);
					}
					strcpy(module.name[i], q->module_name[i]);
					strcpy(module.id[i], q->module_id[i]);
					printf("name= %s id= %s\n", module.name[i], module.id[i]);
				}
			}else{
				hal.max = q->line;
				int i;
				for(i=0; i<q->line; i++)
				{
					hal.name[i] = malloc(255);
					if(hal.name[i] == NULL)
					{
						printf("(main)module->name[%d] malloc failed\n", i);
						exit(0);
					}
					hal.id[i] = malloc(255);
					if(hal.id[i] == NULL)
					{
						printf("(main)module->id[%d] malloc failed\n", i);
						exit(0);
					}
					strcpy(hal.name[i], q->hal_name[i]);
					strcpy(hal.id[i], q->moduleid[i]);
					printf("name= %s id= %s\n", hal.name[i], hal.id[i]);
				}
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
	}
	if(module.max != 0 && hal.max != 0)
	{
		int j;
		for(i=0; i<hal.max; i++)
		{
			j=0;
			while(strcmp(hal.name[i], module.name[j]) != 0)
			{
				j++;
				if(j >= module.max)
				{
					printf("module文件夹中没有找到%s文件 关联失败\n", hal.name[i]);
					j=-1;
					break;
				}
			}
			if(j != -1)
			{
				if(strcmp(hal.id[i], module.id[j]) == 0)
				{
					printf("%s 文件关联成功\n", hal.name[i]);
				}else{
					printf("%s 文件关联失败\n", hal.name[i]);
				}
			}
		}
	}
	printf("parsing OK\n");
	return 0;
}
