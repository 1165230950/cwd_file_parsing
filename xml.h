#ifndef xml_h
#define xml_h

#define xml_head "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
#define xml_body1 "<modules>"
#define xml_body2 "</modules>"

#define column1 "<module"
#define column2 "number="
#define column3 "id="
#define column4 "name="
#define column5 "/>"
#define value_length 50


#define SUSSESS	0
#define ERROR	1

typedef struct result_xml{
	int line;
	char **value;
}result_xml;

typedef struct message_xml{
	char *buf;
}message_xml;
struct result_xml* parsing_xml(struct message_xml* head);


#endif
