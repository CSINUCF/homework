#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "dfa.h"
#include "symtable.h"


typedef struct Symbol{ 
	int kind; 			// const = 1, var = 2, proc = 3
	char *name;			// name up to 11 chars
	int val; 			// number (ASCII value)
	int level; 			// L level
	int addr; 			// M address
}Symbol_t; 

typedef struct Scanner{
	struct DFA *dfa;
	struct SymTable *symbolTable;
	int (*run)(struct Scanner *this,char *path);
	int (*putSymbol)(struct Scanner *this);
	void (*exit)(struct Scanner *this);
	void (*printLexmeList)(struct Scanner *this,char *path);
}Scanner_t;
extern Scanner_t * scanner_init(int numsSymbol);
#endif
