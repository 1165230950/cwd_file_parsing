#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result_xml* parsing_xml(struct message_xml *head)
{
	if(head == NULL) return NULL;
	/*======================将文件信息保存=======================*/
	struct message_xml message;
	message.buf = (char *)malloc(strlen(head->buf)+1);
	strcpy(message.buf, head->buf);
	free(head);
	head = NULL;
	/*======================开始解析文件========================*/
	result_xml *result = malloc(sizeof(result_xml));

	char file_content[256];
	/*====================查看文件有多少行======================*/
	int line_number = 0;
	FILE *fd = fopen("./modult.xml", "r+");
printf("aaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	fgets(file_content, 100, fd);
	return 0;
	while(fgets(file_content, sizeof(file_content), fd))
	{
		line_number++;
	}
	fclose(fd);
	printf("文件有 %d 行\n", line_number);
	result->value = malloc(sizeof(result->value) * line_number);
	
	int line = 0, sign = 0, line_sign = 0;	//声明标记变量
	int end = 0;
	fd = fopen(message.buf, "r");
	while(fgets(file_content, sizeof(file_content), fd))
	{
		if(end == 1)
		{
			return NULL;
		}
		line++;
		printf("line = %d %s", line, file_content);
		if(sign == 0)	//解析第一行信息
		{
			int i=0;
			while(file_content[i] == '\b' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_head, 38))
				{
					i += 38;
					while(file_content[i] == '\b' || file_content[i] == '\t') i++;
					if(file_content[i] != '\n') exit(0);
					else sign = 1;
				}
			}
		}else if(sign == 1)	//解析第二行信息
		{
			int i=0;
			while(file_content[i] == '\b' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_body1, 9))
				{
					i += 9;
					while(file_content[i] == '\b' || file_content[i] == '\t') i++;
					if(file_content[i] != '\n') exit(0);
					else sign = 2;
				}
			}
		}else{
			line_sign = 0;
			while(1)
			{
				char name[15];
				int i = 0, j = 0;
				while(file_content[i] == '\b' || file_content[i] == '\t')	//去除空格
				{
					i++;
				}
				if(file_content[i] == '\n' && line_sign == 5) break;
				while(file_content[i] != '\b' && file_content[i] != '\t' && file_content[i] != '\n')		//提取字符串
				{
					name[j] = file_content[i];
					i++;
					j++;
					line_sign++;		//给字符串上标记
				}

				if(line_sign > 0)	//提取信息
				{
					name[j] = '\0';
					static int len2 = 0;
					int len, len1;
					switch(line_sign)
					{
						case 1:
							if(!strcmp(name, xml_body2))
							{
								end = 1;
								break;
							}
							if(!strcmp(name, column1)) break;
							else{
								return NULL;
							}
						case 2:
							len = strlen(column2);
							len2 = strlen(name+len+1);
							if(name[len+1] != '\"' || name[j-1] != '\"') return NULL;
							if(!strncmp(name, column2, len))
							{
							result->value[0] = malloc(value_length);
								strncpy(result->value[line], name+len+1,len1-1);
								result->value[line][len2] = '\b';
								printf("number = %s", "number");
							}
							break;
						case 3:
							len = strlen(column3);
							len1 = strlen(name+len+1);
							if(name[len+1] != '\"' || name[j-1] != '\"') return NULL;
							if(!strncmp(name, column3, len))
							{
								strncpy(result->value[line]+len2, name+len+1,len1-1);
								len2 += len1;
								result->value[line][len2] = '\b';
								printf("id = %s", "id");
							}
							
							break;
						case 4:
							len = strlen(column4);
							len1 = strlen(name+len+1);
							if(name[len+1] != '\"' || name[j-1] != '\"') return NULL;
							if(!strncmp(name, column4, len))
							{
								strncpy(result->value[line]+len2, name+len+1,len1-1);
								len2 += len1;
								result->value[line][len2] = '\0';
								printf("name = %s", "name");
							}
							
							break;
						case 5:
							if(!strcmp(name, column5)) break;
							else return NULL;
						default:
							printf("parsing error\n");
							return NULL;
					}
				}
			}
		}
	}
	if(line == 0)
	{
		fclose(fd);
		printf("not content\n");
		return NULL;
	}else if(end == 1){
		return result;
	}else{
		fclose(fd);
		printf("read OK\n");
		return NULL;
	}
}
