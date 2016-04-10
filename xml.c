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
	result->value = malloc(100);
	result->line = 0;

	char file_content[256];
	/*====================查看文件有多少行======================*/
	int line_number = 0;
	FILE *fd = fopen(message.buf, "r");
	while(fgets(file_content, sizeof(file_content), fd))
	{
		line_number++;
	}
	fclose(fd);


	int line = 0, sign = 0, line_sign = 0;	//声明标记变量
	int end = 0;
	fd = fopen(message.buf, "r");
	while(fgets(file_content, sizeof(file_content), fd))
	{
		if(end == 1)
		{
//			printf("in end\n");
			int i =0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i+1] == '\n') continue;
//			if(strlen(file_content) == 0) break;
			printf("检测到 </modules> 后面还有字符\n");
			return NULL;
		}
		line++;
//		printf("line=%d %s", line, file_content);
		if(sign == 0)	//解析第一行信息
		{
			int i=0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_head, 38))
				{
					i += 38;
					while(file_content[i] == ' ' || file_content[i] == '\t') i++;
					if(file_content[i+1] != '\n') exit(0);
					else sign = 1;
				}
			}
		}else if(sign == 1)	//解析第二行信息
		{
			int i=0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_body1, 9))
				{
					i += 9;
					while(file_content[i] == ' ' || file_content[i] == '\t') i++;
					if(file_content[i+1] != '\n') exit(0);
					else sign = 2;
				}
			}
		}else{				//解析body内容
			line_sign = 0;
			int i = 0;
			char name_line[15];
			while(1)
			{
				char name[15];
				int j = 0;
				while(file_content[i] == ' ' || file_content[i] == '\t')	//去除空格
				{
					i++;
				}
				if(file_content[i+1] == '\n' && line_sign == 0) break;
				if(file_content[i] == '\n' && line_sign == 5) break;
				while(file_content[i] != ' ' && file_content[i] != '\t' && file_content[i] != '\n')		//提取字符串
				{
					name[j] = file_content[i];
					i++;
					j++;
				}
				line_sign++;		//给字符串上标记

				if(line_sign > 0)	//提取信息
				{
//		printf("解析第 %d 行 第 %d 列\n", line, line_sign);
					name[j] = '\0';
					static int len2 = 0;
					int len, len1;
					switch(line_sign)
					{
						case 1:
							if(!strncmp(name, xml_body2, strlen(xml_body2)))
							{
								printf("检测到 </modules>\n");
								end = 1;
								break;
							}else if(!strcmp(name, column1)) break;
							else{
								printf("第 %d 行 第一列出错\n", line);
								return NULL;
							}
						case 2:
							len = strlen(column2);
							len2 = strlen(name+len+1);
							if(name[len] != '\"' || name[j-1] != '\"') 
							{
								printf("第 %d 行 第二列出错\n", line);
								return NULL;
							}
							if(!strncmp(name, column2, len))
							{
//								printf("拷贝第 %d 行数据\n", line);
								result->value[result->line] = malloc(value_length);
								if(result->value[result->line] == NULL)
								{
									printf("第 %d 行空间分配失败\n", line);
									exit(0);
								}
								strncpy(result->value[result->line], name+len+1, len2-1);

								result->value[result->line][len2-1] = ' ';
//								printf("case2 =%s\n", result->value[result->line]);
								break;
							}else{
								printf("第 %d 行 第四列出错\n", line);
								return NULL;
							}
						case 3:
							len = strlen(column3);
							len1 = strlen(name+len+1);
							if(name[len] != '\"' || name[j-1] != '\"')
							{
								printf("第 %d 行 第三列出错\n", line);
								return NULL;
							}
							if(!strncmp(name, column3, len))
							{
								strncpy(result->value[result->line]+len2, name+len+1,len1-1);
								len2 += len1;
								result->value[result->line][len2-1] = ' ';
//								printf("case3 =%s\n", result->value[result->line]);
								break;
							}else{
								printf("第 %d 行 第四列出错\n", line);
								return NULL;
							}
						case 4:
							len = strlen(column4);
							len1 = strlen(name+len+1);
							if(name[len] != '\"' || name[j-1] != '\"')
							{
								printf("第 %d 行 第四列出错\n", line);
								return NULL;
							}
							if(!strncmp(name, column4, len))
							{
								strncpy(result->value[result->line]+len2, name+len+1,len1-1);
								len2 += len1;
								result->value[result->line][len2] = '\0';
								strcpy(name_line, result->value[result->line]+len2-len1);
//								printf("line=%d case4 =%s\n",result->line, result->value[result->line]);
								break;
							}else{
								printf("第 %d 行 第四列出错\n", line);
								return NULL;
							}
						case 5:
							if(!strncmp(name, column5, strlen(column5)))
							{
								result->line ++;
//								printf("lien =%d\n", result->line);
								break;
							}
							else{
								printf("第 %d 行 第五列出错\n", line);
								return NULL;
							}
						default:
							printf("第 %d 行出错 />后面有字符无法解析\n", line);
							result->line --;
							return NULL;
					}
					if(end == 1) break;
				}//end switch()
			}//end while(1)
			if(end != 1)
			{
				char buf[256];
				strcat(name_line, ".txt");
				FILE *fp = fopen(name_line, "r");
				if(fp == NULL)
				{
					printf("not find %s\n", name_line);
				}else{
					fgets(buf, sizeof(buf), fp);
					printf("buf =%s", buf);
					fclose(fp);
				}
			}
		}//end else
	}//end while(fgets)
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
