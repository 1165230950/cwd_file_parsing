#ifndef html_h
#define html_h

typedef struct result_html{
	int line;
	char **parsing_result;
}result_html;

typedef struct message_html{
	char *buf;
}message_html;
struct result_html* parsing_html(struct message_html* head);


#endif
