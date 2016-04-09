#ifndef xml_h
#define xml_h

typedef struct result{
	int line;
	char **parsing_result;
}result;

typedef struct message{
	char *buf;
}message;
struct result* parsing_xml(struct message* head);


#define
