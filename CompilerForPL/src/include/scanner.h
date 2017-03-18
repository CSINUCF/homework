#ifndef SCANNER_H
#define SCANNER_H

#include "common.h"
#include "dfa.h"
#include "symtable.h"

typedef struct Scanner{
	struct DFA *dfa;
	struct SymTable *symbolTable;
	int (*run)(struct Scanner *this,char *path);
	int (*putSymbol)(struct Scanner *this);
	void (*exit)(struct Scanner *this);
	void (*printLexmeList)(struct Scanner *this,char *path);
	void (*outputLexmeList)(struct Scanner *this,FILE *out);
}Scanner_t;
extern Scanner_t * scanner_init(struct SymTable * syms);
#endif
