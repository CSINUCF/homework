#include "../include/scanner.h"

void scanner_exit(struct Scanner *this){
	struct DFA *dfa = this->dfa;
	struct SymTable *symbolTable = this->symbolTable;
	dfa->exit(dfa);
	symbolTable->clean(symbolTable);
}


static inline int putSymbol(struct Scanner *this){

	LexemeEntry_t* currToken = this->dfa->tokenTable;
	LexemeEntry_t* nextToken = NULL;
	LexemeEntry_t* pToken = NULL;
	LexemeEntry_t* next2Token = NULL;
	struct SymTable *symbolTable = this->symbolTable;
	int numLex = 0;
	boolean errFlag = FALSE;
	Symbol_t * node = NULL;

	while(currToken != NULL){
		nextToken = currToken;
		numLex = 0;
		while((nextToken->type != semicolonsym)&&(nextToken->type != periodsym)){
			numLex++;
			nextToken = nextToken->next;
		}
		switch(currToken->type){
			case constsym:
			{
				if(numLex%4 == 0){
					pToken = currToken->next;
					while(pToken != nextToken->next){
						node = (Symbol_t *)calloc(1,sizeof(Symbol_t));
						if(node == NULL){
							logerror("new a symbol node failed constsym\n");
							errFlag = TRUE;
							break;
						}
						node->kind = 1;
						if(pToken->type == identsym){
							node->name = strdup(pToken->buffer);
							pToken = pToken->next->next;
						}else{
							logerror("Shoule be indent constsym\n");
							errFlag = TRUE;
							free(node);
							break;
						}
						if(pToken->type == numbersym){
							node->val = atoi(pToken->buffer);
							pToken = pToken->next->next;
						}else{
							logerror("Shoule be numbers constsym\n");
							free(node);
							errFlag = TRUE;
							break;
						}
						symbolTable->put(symbolTable,node->name,node);
						node = NULL;
					}
				}else{
					errFlag = TRUE;
					logerror("There are some error in const definition\n");
				}
			}break;
			case varsym:
			{
				if(numLex%2 == 0){
					pToken = currToken->next;
					while(pToken != nextToken->next){
						node = (Symbol_t *)calloc(1,sizeof(Symbol_t));
						if(node == NULL){
							logerror("new a symbol node failed varsym\n");
							break;
						}
						node->kind = 2;
						if(pToken->type == identsym){
							node->name = strdup(pToken->buffer);
							pToken = pToken->next->next;
						}else{
							logerror("Shoule be indent varsym\n");
							free(node);
							errFlag = TRUE;
							break;
						}
						symbolTable->put(symbolTable,node->name,node);
						node = NULL;
					}
				}else{
					errFlag = TRUE;
					logerror("There are some error in var definition\n");

				}
			}break;
			case procsym:
			{
				if(numLex == 2){
					pToken = currToken->next;
					while(pToken != nextToken->next){
						node = (Symbol_t *)calloc(1,sizeof(Symbol_t));
						if(node == NULL){
							logerror("new a symbol node failed procsym\n");
							break;
						}
						node->kind = 3;
						if(pToken->type == identsym){
							node->name = strdup(pToken->buffer);
							pToken = pToken->next->next;
						}else{
							logerror("Shoule be indent in [procsym]\n");
							free(node);
							errFlag = TRUE;
							break;
						}
						symbolTable->put(symbolTable,node->name,node);
						node = NULL;
					}
				}else{
					errFlag = TRUE;
					logerror("There are some error in procedure definition\n");
				}
			}break;
		}
		if(errFlag == TRUE){
			loginfo("numsOfLex:%3d ",numLex);
			while(currToken != nextToken->next){
				logpretty("<%s,%d> ",currToken->buffer,currToken->type);
				currToken = currToken->next;
			}
			logpretty("\n");
		}
		currToken = nextToken->next;
	}
	return 0;
}

static inline void _printLexmeList(struct Scanner *this,char *path){
	DFA_T *dfa = this->dfa;
	dfa->printLexme(dfa,path);
}


static inline void _outputLexmeList(struct Scanner *this,FILE *out){
	DFA_T *dfa = this->dfa;
	dfa->outputLexme(dfa,out);
}

int scanner_run(struct Scanner *this,char *path){
	struct DFA *dfa = this->dfa;

	FILE* fin = fopen(path,"r");
	if(fin == NULL){
		logerror("Open file failed, please double check: %s\n",path);
		return -1;
	}

	while(!feof(fin)){
		dfa->getNextLexeme(dfa,fin);
	}
	fclose(fin);
	this->putSymbol(this);
	return 0;
}

Scanner_t * scanner_init(int numsSymbol){
	Scanner_t *scanner = (Scanner_t *)calloc(1,sizeof(Scanner_t));
	if(scanner == NULL){
		logerror("Apply for scanner memory failed\n");
		return NULL;
	}
	scanner->dfa = dfa_init();
	if(scanner->dfa == NULL){
		logerror("Init DFA failed\n");
		free(scanner);
		return NULL;
	}
	scanner->symbolTable = SymTable_init(numsSymbol);
	if(scanner->symbolTable == NULL){
		logerror("Init Symbol Table failed\n");
		scanner->dfa->exit(scanner->dfa);
		free(scanner);
		return NULL;
	}
	scanner->run = scanner_run;
	scanner->exit = scanner_exit;
	scanner->printLexmeList = _printLexmeList;
	scanner->putSymbol = putSymbol;
	scanner->outputLexmeList = _outputLexmeList;
	return scanner;
}
