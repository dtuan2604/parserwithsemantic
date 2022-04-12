#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generator.h"
#include "stack.h"
#include "scanner.h"

int* varCount = NULL;
int curIndex;
char* gen = "generator";
extern char* fileName;

void initVarcount(){
	varCount = (int*)malloc(MAX_VARS * sizeof(int));
	if(varCount == NULL){
		fprintf(stderr,"ERROR: %s: Cannot allocate varCount memory\n", gen);
		exit(-1);
	}
	curIndex = 0;	
	
	return;
}
void destroyVarcount(){
	while(varCount[curIndex] > 0){
		pop();
		varCount[curIndex]--;
	}
	
	free(varCount);
	varCount = NULL;

}
void semanticCheck(struct node_t* nodePtr){
	if(nodePtr != NULL){
		int isBlock = 0;
		if(strcmp(nodePtr->name,"block") == 0){
			curIndex++;
			varCount[curIndex] = 0;
			isBlock = 1;
		}else if(strcmp(nodePtr->name,"vars") == 0 && nodePtr->numToken > 0){
			//checking if the variable is already defined
			int flag = find(nodePtr->tokenList[1]); 
			struct token* curToken = nodePtr->tokenList[1];
			if(flag > -1){
				struct token* predefToken = getToken(flag);
				printRed();
				printf("%s:%d:%d: SEMANTIC ERROR: ",fileName, curToken->line, curToken->charN);
				
				printYellow();
				printf("Multiple definitions for '%s' ",predefToken->tokenIns);
				printf("(previously defined at %s:%d:%d)\n", fileName, predefToken->line, predefToken->charN);
				printReset();
				free(predefToken);
				exit(-1);
			}
			
			push(curToken);
			printf("Adding '%s' at %d:%d to the stack\n",curToken->tokenIns,curToken->line,curToken->charN);
			varCount[curIndex]++;
		}else{
			//process non-vars node
		}

                semanticCheck(nodePtr->left);
                semanticCheck(nodePtr->middle);
                semanticCheck(nodePtr->right);
		
		if(isBlock == 1){
			int i;
			for(i = 0; i < varCount[curIndex]; i++){
				printf("Popping\n");
				pop();
			}
			curIndex--;
		}
        }

}

