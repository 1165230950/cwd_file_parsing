#include "read_information.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_module_file_information(struct xml_module* module)
{
	char dir[256];
	sprintf(dir, "%s/%s.txt", module->dir, module->file_name[module->sign-1]);
//	printf("path =%s\n", dir);
	FILE *fp = fopen(dir, "r");
	if(fp == NULL)
	{
		printf("%s.txt打开失败", module->file_name[module->sign-1]);
		return 1;
	}
	char *buf = malloc(sizeof(char) * 256);
	char p[256];
	while(fgets(p, sizeof(p), fp))
	{
		int i=0, j=0;
		while(p[i] == ' ' || p[i] == '\t') i++;
		if(p[i] == '\n') continue;
		while(p[i] != ' ' && p[i] != '\t' && p[i] != '\n')
		{
			buf[j] = p[i];
			i++;
			j++;
		}
		buf[j] = '\0';
		while(p[i] == ' ' || p[i] == '\t') i++;
		while(p[i] != '\n')
		{
			fclose(fp);
			return 1;
		}
		int len_buf = strlen(buf);
		if(strncmp(buf, "id=", 3) == 0)
		{
			if(buf[3] == '\"' && buf[len_buf-1] == '\"')
			{
				if(strcmp(module->id, buf) != 0)
				{
					printf("module_id =%s module_id =%s\n", module->id, buf);
					fclose(fp);
					return 1;
				}
			}
		}else if(strncmp(buf, "name=", 5) == 0)
		{
			if(buf[5] == '\"' && buf[len_buf-1] == '\"')
			{
				if(strcmp(module->name, buf) != 0)
				{
					printf("module_name =%s module_name =%s\n", module->name, buf);
					fclose(fp);
					return 1;
				}
			}
		}
	}
	fclose(fp);
	return 0;
}
int read_hal_file_information(struct xml_hal *hal)
{
	char dir[256];
	sprintf(dir, "%s/%s.txt", hal->dir, hal->file_name[hal->sign-1]);
	FILE *fp = fopen(dir, "r");
	if(fp == NULL)
	{
		printf("%s.txt打开失败", hal->file_name[hal->sign-1]);
		fclose(fp);
		return 1;
	}
	char *buf = malloc(sizeof(char) * 256);
	char p[256];
	while(fgets(p, sizeof(p), fp))
	{
		int i=0, j=0;
		while(p[i] == ' ' || p[i] == '\t') i++;
		if(p[i] == '\n') continue;
		while(p[i] != ' ' && p[i] != '\t' && p[i] != '\n')
		{
			buf[j] = p[i];
			i++;
			j++;
		}
		buf[j] = '\0';
		while(p[i] == ' ' || p[i] == '\t') i++;
		while(p[i] != '\n')
		{
			fclose(fp);
			return 1;
		}
		int len_buf = strlen(buf);
		if(strncmp(buf, "id=", 3) == 0)
		{
			if(buf[3] == '\"' && buf[len_buf-1] == '\"')
			{
				if(strcmp(hal->id, buf) != 0)
				{
					printf("module_id =%s module_id =%s\n", hal->id, buf);
					fclose(fp);
					return 1;
				}
			}
		}else if(strncmp(buf, "name=", 5) == 0)
		{
			if(buf[5] == '\"' && buf[len_buf-1] == '\"')
			{
				if(strcmp(hal->name, buf) != 0)
				{
					printf("xml_name =%s txt_name =%s\n", hal->name, buf);
					fclose(fp);
					return 1;
				}
			}
		}
	}
	fclose(fp);
	return 0;
}
