#include "xml.h"
#include "read_information.h"
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

	/*====================查看文件有多少行======================*/
	char file_content[1024];
	int line_number = 0;
	FILE *fq = fopen(message.buf, "r");
	if(fq == NULL)
	{
		printf("文件打开失败\n");
		return NULL;
	}
	while(fgets(file_content, sizeof(file_content), fq))
	{
		line_number++;
	}
	fclose(fq);
	/*======================开始解析文件========================*/
	line *line_result = malloc(sizeof(line));	//用来存储读到行的信息
	module *module = malloc(sizeof(module));
	hal *hal = malloc(sizeof(module));
	

	int line = 0, sign = 0, line_sign = 0;	//声明标记变量
	int end = 0;	
	FILE *aa = fopen(message.buf, "r");
	FILE *fd = fopen(message.buf, "r");
	if(fd == NULL)
	{
		printf("文件打开失败\n");
		return NULL;
	}


	while(fgets(file_content, sizeof(file_content), fd))	//一行一行的读出文件内容
	{
		if(end == 1)
		{
			int i =0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i+1] == '\n') continue;
			printf("检测到 </modules> 后面还有字符\n");
			return NULL;
		}
		line++;
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
		}else if(sign == 1)	//解析第二行信息(判断是tal还是module)
		{
			int i=0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_hal1, strlen(xml_hal1)))
				{
					i += strlen(xml_hal1);
					while(file_content[i] == ' ' || file_content[i] == '\t') i++;
					if(file_content[i+1] != '\n') exit(0);
					sign = 3;
					strncpy(hal->dir, xml_hal1+1, strlen(xml_hal1)-3);
					hal->dir[strlen(xml_hal1)-2] = '\0';
				}else{
					printf("解析第二行出错\n");
					return NULL;
				}
			}
		}else{				//解析body内容
			line_sign = 0;
			int i = 0;
			while(1)
			{
				char name[50];
				int j = 0;
				while(file_content[i] == ' ' || file_content[i] == '\t')	//去除空格
				{
					i++;
				}
				if(file_content[i+1] == '\n' && line_sign == 0) break;
				if(file_content[i] == '\n' && line_sign == 5) break;
				if(file_content[i] == '\n' && line_sign == 6) break;
				while(file_content[i] != ' ' && file_content[i] != '\t' && file_content[i] != '\n')		//提取字符串
				{
					name[j] = file_content[i];
					i++;
					j++;
				}
				line_sign++;		//给字符串上标记

				if(line_sign > 0)	//提取信息
				{
					name[j] = '\0';
					static int len2 = 0;
					int len, len1;
					switch(line_sign)
					{
						case 1:
								if(!strncmp(name, xml_hal2, strlen(xml_hal2)))
								{
									printf("检测到 </hals>\n");
									end = 1;
									break;
								}else if(!strcmp(name, column1_1)) 
								{
									break;
								}
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
								break;
							}else{
								printf("第 %d 行 第二列出错\n", line);
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
									strcpy(hal->id,name);
								len2 += len1;
								break;
							}else{
								printf("第 %d 行 第三列出错\n", line);
								return NULL;
							}
						case 4:
								len = strlen(hal_moduleid);
								len1 = strlen(name+len+1);
								if(name[len] != '\"' || name[j-1] != '\"')
								{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
								if(!strncmp(name, hal_moduleid, len))
								{
									strcpy(hal->moduleid, name+len+1);
									len2 += len1;
									break;
								}else{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
						case 5:
								len = strlen(column4);
								len1 = strlen(name+len+1);
								if(name[len] != '\"' || name[j-1] != '\"')
								{
									printf("第 %d 行 第五列出错\n", line);
									return NULL;
								}
								if(!strncmp(name, column4, len))
								{
									strncpy(hal->file_name, name+len+1,len1-1);
									strcpy(hal->name,name);
									len2 += len1;
									break;
								}else{
									printf("第 %d 行 第五列出错\n", line);
									return NULL;
								}
							
						case 6:
								if(!strncmp(name, column5, strlen(column5)))
								{
									break;
								}else{
									printf("第 %d 行 第六列出错\n", line);
									return NULL;
								}
								
						default:
							printf("line_sign=%d\n", line_sign);
							printf("第 %d 行出错 />后面有字符无法解析\n", line);
							return NULL;
					}//end switch()
					if(end == 1) break;
				}//end else
			}//end while(1)
			if(end != 1)
			{
				
					int ret = read_hal_file_information(hal);
					if(ret == 1)
					{
						printf("%s文件匹配失败\n", hal->file_name);
					}else{
						printf("%s文件匹配成功\n", hal->file_name);
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
		return NULL;
	}else{
		fclose(fd);
		printf("read OK\n");
		return NULL;
	}
}
