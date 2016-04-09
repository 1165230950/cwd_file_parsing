#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result_xml* parsing_xml(struct message_xml *head)
{
	if(head == NULL) return NULL;
	/*========================= ============================*/
	struct message_xml message;
	message.buf = (char *)malloc(strlen(head->buf)+1);
	strcpy(message.buf, head->buf);
	free(head);
	head = NULL;

	/*========================= ============================*/
	result_xml *result = malloc(sizeof(result_xml));

	char file_content[256];
	int line = 0, sign = 0, line_sign = 0;
	FILE *fd = fopen(message.buf, "r");
	while(fgets(file_content, sizeof(file_content), fd))
	{
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
			while(1)
			{
				char name[15];
				int i = 0, j = 0;
				while(file_content[i] == '\b' || file_content[i] == '\t')	//去除空格
				{
					i++;
				}
				while(file_content[i] != '\n')		//提取字符串
				{
					name[j] = file_content[i];
					i++;
					j++;
					line_sign++;		//给字符串上标记
				}

				if(line_sign > 0)	//提取信息
				{
					name[j] = '\0';
					switch(sign)
					{
						case 1:
							if(!strcmp(name, "<modules>"))
							{
								
							}
							break;
						case 2:
							
							break;
						case 3:
							
							break;
						case 4:
							
							break;
						case 5:
							
							break;
						case 6:
							printf("parsing error\n");
							break;
						case 7:
							printf("parsing error\n");
							break;
						case 8:
							printf("parsing error\n");
							
							break;
						case 9:
							printf("parsing error\n");
							
							break;
						case 10:
							printf("parsing error\n");
							break;
						default:
							printf("parsing error\n");
							break;
					}
				}
			}
		}
	}
	if(line == 0)
	{
		fclose(fd);
		printf("not content\n");
		return result;
	}else{
		fclose(fd);
		printf("read OK\n");
		return NULL;
	}
}
