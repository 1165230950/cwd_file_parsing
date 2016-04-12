#ifndef xml_h
#define xml_h

#define xml_head "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

#define xml_module1 "<modules>"
#define xml_module2 "</modules>"
#define xml_hal1 "<hals>"
#define xml_hal2 "</hals>"

#define column1 "<module"
#define column1_1 "<hal"
#define column2 "number="
#define column3 "id="
#define column4 "name="
#define column5 "/>"
#define hal_moduleid "moduleid="
#define value_length 50


#define SUSSESS	0
#define ERROR	1

typedef struct result_xml{
	int sign;
	int line;
	char *module_id[1024];
	char *moduleid[1024];
	char *module_name[1024];
	char *hal_name[1024];
}result_xml;

typedef struct message_xml{
	char *buf;
}message_xml;

typedef struct module{
	int max;
	char *id[255];
	char *name[255];
}module_message;

typedef struct hal{
	int max;
	char *id[255];
	char *name[255];
}hal_message;

typedef struct line_message{
	char format[256];
	char column_number[256];
	char column_id[256];
	char column_moduleid[256];
	char column_name[256];
}line;

struct result_xml* parsing_xml(struct message_xml* head);


#endif
