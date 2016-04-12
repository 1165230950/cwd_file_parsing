#include "xml.h"
#include "read_information.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result_xml* parsing_xml(struct message_xml *head)
{
//printf("input parsing_xml\n");
	if(head == NULL)
	{
		printf("head == NULL\n");
		return NULL;
	}
	/*======================将文件信息保存=======================*/
	struct message_xml message;
	message.buf = (char *)malloc(strlen(head->buf)+1);
	if(message.buf == NULL)
	{
		printf("message.buf malloc failed\n");
		return NULL;
	}
	strcpy(message.buf, head->buf);
	free(head);
	head = NULL;

	/*====================查看文件有多少行======================*/
	char file_content[1024];
	int line_number = 0;
	FILE *fd = fopen(message.buf, "r");
	if(fd == NULL)
	{
		printf("message.buf(fd1) open failed\n");
		return NULL;
	}
	while(fgets(file_content, sizeof(file_content), fd))
	{
		line_number++;
	}
	fclose(fd);

//printf("start parsing file\n");
	/*======================开始解析文件========================*/
	result_xml *xml = malloc(sizeof(result_xml));
	if(xml == NULL)
	{
		printf("xml malloc failed\n");
		return NULL;
	}
	xml->line = 0;
	module *module = malloc(sizeof(module));
	if(module == NULL)
	{
		printf("module malloc failed\n");
		return NULL;
	}
	module->sign = 0;
	hal *hal = malloc(sizeof(hal));
	if(hal == NULL)
	{
		printf("hal malloc failed\n");
		return NULL;
	}
	hal->sign = 0;
	

	int line = 0, sign = 0, line_sign = 0;	//声明标记变量
	int end = 0;	
	FILE *fdd = fopen(message.buf, "r");
	if(fdd == NULL)
	{
		printf("message.buf(fdd) open failed\n");
		return NULL;
	}
	fd = fopen(message.buf, "r");
	if(fd == NULL)
	{
		printf("message.buf(fd2) open failed\n");
		return NULL;
	}

//printf("start read file\n");
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
					if(file_content[i+1] != '\n')
					{
						printf("第一行检测到后面还有字符\n");
						exit(0);
					}
					else sign = 1;
				}
			}
		}else if(sign == 1)	//解析第二行信息(判断是tal还是module)
		{
			int i=0;
			while(file_content[i] == ' ' || file_content[i] == '\t') i++;
			if(file_content[i] != '\n')
			{
				if(!strncmp(file_content+i, xml_module1, strlen(xml_module1)))
				{
					i += strlen(xml_module1);
					while(file_content[i] == ' ' || file_content[i] == '\t') i++;
					if(file_content[i+1] != '\n')
					{
						printf("第二行检测到后面还有字符(module)\n");
						exit(0);
					}
					sign = 2;
					xml->sign = 2;
					strncpy(module->dir, xml_module1+1, strlen(xml_module1)-3);
					module->dir[strlen(xml_module1)-3] = '\0';
				}else 
			
				if(!strncmp(file_content+i, xml_hal1, strlen(xml_hal1)))
				{
					i += strlen(xml_hal1);
					while(file_content[i] == ' ' || file_content[i] == '\t') i++;
					if(file_content[i+1] != '\n')
					{
						printf("第二行检测到后面还有字符(hal)\n");
						exit(0);
					}
					sign = 3;
					xml->sign = 3;
					strncpy(hal->dir, xml_hal1+1, strlen(xml_hal1)-3);
					hal->dir[strlen(xml_hal1)-3] = '\0';
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
							if(sign == 2)
							{
								if(!strncmp(name, xml_module2, strlen(xml_module2)))
								{
									printf("检测到 </modules>\n");
									end = 1;
									break;
								}else if(!strcmp(name, column1)) break;
								else{
									printf("第 %d 行 第一列出错\n", line);
									return NULL;
								}
							}else
						
							{
								if(!strncmp(name, xml_hal2, strlen(xml_hal2)))
								{
									printf("检测到 </hals>\n");
									end = 1;
									break;
								}else if(!strcmp(name, column1_1)) break;
								else{
									printf("第 %d 行 第一列出错\n", line);
									return NULL;
								}
								
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
								if(sign == 2)
								{
									if(strcpy(module->id,name) == NULL)
									{
										printf("module->id copy failed\n");
										return NULL;
									}
									module->module_id[module->sign] = malloc(255);
									if(module->module_id[module->sign] == NULL)
									{
										printf("module_id[%d] malloc failed\n", module->sign);
										return NULL;
									}
									strncpy(module->module_id[module->sign], name+len+1,len1-1);
									module->module_id[module->sign][len1-1] = '\0';
								}else{
									strcpy(hal->id,name);
								}
								len2 += len1;
								break;
							}else{
								printf("第 %d 行 第四列出错\n", line);
								return NULL;
							}
						case 4:
							if(sign == 2)
							{
								len = strlen(column4);
								len1 = strlen(name+len+1);
								if(name[len] != '\"' || name[j-1] != '\"')
								{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
								if(!strncmp(name, column4, len))
								{
									module->file_name[module->sign] = malloc(255);
									if(module->file_name == NULL)
									{
										printf("module->file_name[%d] malloc failed\n", module->sign);
										return NULL;
									}
									strncpy(module->file_name[module->sign], name+len+1,len1-1);
									module->file_name[module->sign][len1-1] = '\0';
									strcpy(module->name,name);
									len2 += len1;
									break;
								}else{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
							}else{
						
								len = strlen(hal_moduleid);
								len1 = strlen(name+len+1);
								if(name[len] != '\"' || name[j-1] != '\"')
								{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
								if(!strncmp(name, hal_moduleid, len))
								{
									hal->moduleid[hal->sign] = malloc(255);
									if(hal->moduleid[hal->sign] == NULL)
									{
										printf("hal->moduleid[%d] malloc failed\n", hal->sign);
										return NULL;
									}
									strncpy(hal->moduleid[hal->sign], name+len+1,len1-1);
									hal->moduleid[hal->sign][len1-1] = '\0';
									len2 += len1;
									break;
								}else{
									printf("第 %d 行 第四列出错\n", line);
									return NULL;
								}
							}
						case 5:
							if(sign == 2)
							{
								if(!strncmp(name, column5, strlen(column5)))
								{
									module->sign ++;
									break;
								}else{
									printf("第 %d 行 第五列出错\n", line);
									return NULL;
								}
							}else{
						
								len = strlen(column4);
								len1 = strlen(name+len+1);
								if(name[len] != '\"' || name[j-1] != '\"')
								{
									printf("第 %d 行 第五列出错\n", line);
									return NULL;
								}
								if(!strncmp(name, column4, len))
								{
									hal->file_name[hal->sign] = malloc(255);
									if(hal->file_name[hal->sign] == NULL)
									{
										printf("hal->file_name[%d] malloc failed\n", hal->sign);
										return NULL;
									}
									strncpy(hal->file_name[hal->sign], name+len+1,len1-1);
									hal->file_name[hal->sign][len1-1] = '\0';
									strcpy(hal->name,name);
									len2 += len1;
									break;
								}else{
									printf("第 %d 行 第五列出错\n", line);
									return NULL;
								}
							
							}
						case 6:
							if(sign == 2);
							else{
								if(!strncmp(name, column5, strlen(column5)))
								{
									hal->sign ++;
									break;
								}else{
									printf("第 %d 行 第六列出错\n", line);
									return NULL;
								}
								
							}
						default:
							if(sign == 2) module->sign --;
							else hal->sign --;
							printf("line_sign=%d\n", line_sign);
							printf("第 %d 行出错 />后面有字符无法解析\n", line);
							return NULL;
					}//end switch()
					if(end == 1) break;
				}//end else
			}//end while(1)
			if(end != 1)
			{
				if(sign == 2)
				{
					int ret = read_module_file_information(module);
					if(ret == 1)
					{
						printf("%s文件匹配失败\n", module->file_name[module->sign-1]);
					}else{
						xml->module_name[xml->line] = malloc(255);
						if(xml->module_name[xml->line] == NULL)
						{
							printf("xml->module_name[%d] malloc failed\n", xml->line);
							return NULL;
						}
						xml->module_id[xml->line] = malloc(255);
						if(xml->module_id == NULL)
						{
							printf("xml->module_id[%d] malloc failed\n", xml->line);
							return NULL;
						}
						strcpy(xml->module_name[xml->line], module->file_name[module->sign-1]);
						strcpy(xml->module_id[xml->line], module->module_id[module->sign-1]);
					//	printf("%s文件匹配成功 id= %s\n", xml->module_name[xml->line], xml->module_id[xml->line]);
						xml->line ++;
					}
				}else{
			
				
					int ret = read_hal_file_information(hal);
					if(ret == 1)
					{
						printf("%s文件匹配失败\n", hal->file_name[hal->sign-1]);
					}else{
						xml->hal_name[xml->line] = malloc(255);
						if(xml->hal_name[xml->line] == NULL)
						{
							printf("xml->hal_name[%d] malloc failed\n", xml->line);
							return NULL;
						}
						xml->moduleid[xml->line] = malloc(255);
						if(xml->moduleid == NULL)
						{
							printf("xml->moduleid[%d] malloc failed\n", xml->line);
							return NULL;
						}
						strcpy(xml->hal_name[xml->line], hal->file_name[hal->sign-1]);
						strcpy(xml->moduleid[xml->line], hal->moduleid[hal->sign-1]);
					//	printf("%s文件匹配成功 id= %s\n", xml->hal_name[xml->line], xml->moduleid[xml->line]);
						xml->line ++;
					}
				
				}
				
			}
		}//end else
	}//end while(fgets)
	if(line == 0)
	{
		fclose(fd);
		fclose(fdd);
		printf("not content\n");
		return NULL;
	}else if(end == 1){
		if(xml->sign == 2){
			printf("module 匹配完成\n");
			return xml;
		}else{
			printf("hal 匹配完成\n");
			return xml;
		}
	}else{
		fclose(fd);
		fclose(fdd);
		printf("匹配中出错\n");
		return NULL;
	}
}
