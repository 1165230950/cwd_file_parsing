#ifndef txt_h
#define txt_h

typedef struct result{
	int line;
	char **parsing_result;
}result;

typedef struct message{
	char *buf;
}message;
struct result* parsing_txt(struct message* head);


#endif
