#ifndef DRIVE_H
#define DRIVE_H

#include "common.h"
#include "virtualMachine.h"
#include "scanner.h"
#include "symtable.h"
#include "parse.h"
#include "gencode.h"


typedef struct CompilerDriver{
	struct Scanner *scanner;
	struct Parse *parse;
	struct GenCode *gc;
	struct SymTable *sym;
	struct virtualMachine *vm;
	void (*cleanup)(struct CompilerDriver *this);
	void (*run)(struct CompilerDriver *this,char *path);
}CompilerDriver_t;
#endif
