#ifndef _STACK_H_
#define _STACK_H_
#include "token.h"

#define MAX_VARS 100

struct stack {
	int size;
	struct token** body;
};

int createStack();
void destroyStack();
int isFull();
int isEmpty();
void push(struct token* newToken);
void pop();
int find(struct token* targetTok);


#endif
