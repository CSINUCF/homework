#ifndef PARSE_H 
#define PARSE_H
/**************************************************************************************************
<It is project about Compiler for PL/0>
Copyright (C) <2017>  <Bingbing Rao> <Bing.Rao@outlook.com>
@https://github.com/CSINUCF


This program is free software: you can redistribute it and/or modify it under the terms 
of the GNU General Public License as published by the Free Software Foundation, 
either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "dfa.h"
#include "symtable.h"
#include "gencode.h"
#include "ast_node.h"

/*
  *
  **/
typedef struct CurrentToken{
	Token_t cToken;
	union{
  		int numeric;
  		char string[MAX_IDENT_LENGTH + 1];
	}cTokenVal;
}CurrentToken_t;


typedef struct Parse{
	FILE *stdout; // output console, a file or screen
	FILE *tokenFile;// token file
	CurrentToken_t curToken;// to record current token
	int tokenCount;
	struct SymTable *symbolTable;// symbol table
	struct programNode *ast; // root of ast
	int (*parse)(struct Parse *this); // parse funciton
	void (*printAST)(struct Parse *this,FILE *stdout);
	void (*unParse)(struct Parse *this);
	void (*cleanup)(struct Parse *this);
	int (*run)(struct Parse *this,FILE *tFile);
	void (*exit)(struct Parse *this);
	void (*unParsePrint)(struct Parse *this,FILE *stdout);
}Parse_t;
extern char *opSymbol[];
extern struct Parse * parse_init(struct SymTable *syms);
#endif
