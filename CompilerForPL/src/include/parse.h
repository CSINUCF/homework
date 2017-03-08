#ifndef PARSE_H 
#define PARSE_H
#include "common.h"
#include "symtable.h"

typedef struct CurrentToken{
	Token_t cToken;
	union{
  		int numeric;
  		char string[MAX_IDENT_LENGTH + 1];
	}cTokenVal;
}CurrentToken_t;


typedef struct Parse{

	/*input file*/
	char* in_path;
	/*output file*/
	char* out_path;
	FILE * out_fp;

	FILE* tokenFile;
	CurrentToken_t curToken;
	int tokenCount;
	struct SymTable *symbolTable;
	void (*getToken)(struct Parse *this);
	int (*parse)(struct Parse *this);
}Parse_t;

void getToken(struct Parse *this){
	CurrentToken_t *currentToken = &this->curToken;
	if(feof(this->tokenFile)){
		//if end of file, exit
		currentToken->cToken = nulsym;
		return;
	}
	//read the new token in
	fscanf(this->tokenFile, "%d ", (int*)(&currentToken->cToken));
	// update the content if current token is indent
	if(currentToken->cToken == identsym){
		fscanf(this->tokenFile, "%s ", currentToken->cTokenVal.string);
	}
	// update he content if current token is a number.
	if(currentToken->cToken == numbersym){
		fscanf(this->tokenFile, "%d ", &currentToken->cTokenVal.numeric);
	}
	this->tokenCount++;
}
static inline int factor(struct Parse *this){
	

}

static inline int term(struct Parse *this){
	

}

static inline int expression(struct Parse *this){
	

}

static inline int condition(struct Parse *this){
	

}


static inline int statement(struct Parse *this){
	

}



static inline int block(struct Parse *this){
	

}

static inline int program(struct Parse *this){
	

}


int parse(struct Parse *this){

	this->getToken(this);
	program(this);

	
	return 1;
}

struct Parse * parse_init(int numsSymbol, FILE *tFile){
	Parse_t *parser = (Parse_t *)calloc(1,sizeof(Parse_t));
	if(parser == NULL){
		logerror("Apply for parser memory failed\n");
		return NULL;
	}
	parser->tokenFile = tFile;
	parser->symbolTable = SymTable_init(numsSymbol);
	if(parser->symbolTable == NULL){
		logerror("Init Symbol Table failed\n");
		return NULL;
	}
	parser->tokenCount = 0;
}
void parse_exit(struct Parse *this){
	free(this->symbolTable);
}


#endif
