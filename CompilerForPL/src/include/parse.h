#ifndef PARSE_H 
#define PARSE_H

#include "common.h"
#include "dfa.h"
#include "symtable.h"


typedef struct identNode{
	char *string;
}identNode_t;
typedef struct numberNode{
	int value;
}numberNode_t;

typedef struct relOpNode{
	/*"="|"<>"|"<"|"<="|">"|">="*/ 
	/*odd*/
	Token_t type;
}relOpNode_t;
typedef struct AriOpNodepm{
	/*"+"|"-"*/
	Token_t type;
}AriOpNodepm_t;
typedef struct AriOpNodemd{
	/*"*"|"/"*/
	Token_t type;
}AriOpNodemd_t;

typedef struct factorNode{
	enum{
		F_IDENT=1,
		F_DIGIT=2,
		F_EXP=3
	}tag;	
	/*factor ::= ident | number | "(" expression ")".*/
	union{
		struct identNode ident;
		struct numberNode digit;
		struct expressionNode *exp;
	}element;
}factorNode_t;
typedef struct factorExp{
	/*{("*"|"/") factor}*/
	struct AriOpNodemd op;
	struct factorNode *factor;
	struct factorExp *next;
}factorExp_t;

typedef struct termNode{
	/*term ::= factor {("*"|"/") factor}. */
	struct factorExp *factorEList;
	int numsFacorExp;
}termNode_t;


typedef struct termExp{
	/* ("+"|"-") term*/
	struct AriOpNodepm op;
	struct termNode *term;
	struct termExp *next;
}termExp_t;

typedef struct expressionNode{
/*
  *expression ::= [ "+"|"-"] term { ("+"|"-") term}.
  */	
	struct termExp *termEList;
	int numsTermExp;
}expressionNode_t;



typedef struct conditionNode{
/*
condition ::= "odd" expression 
	  	| expression  rel-op  expression.
*/	
	struct relOpNode op;
	struct expressionNode *left;
	struct expressionNode *right;
}conditionNode_t;




typedef struct callNode{
	struct identNode ident;
}callNode_t;

typedef struct writeSNode{
	/*"write"  ident */
	struct identNode ident;
}writeSNode_t;

typedef struct readSNode{
	/*"read" ident */
	struct identNode ident;
}readSNode_t;

typedef struct whileSNode{
	/*"while" condition "do" statement*/
	struct conditionNode *cond;
	struct statementNode *sts;
}whileSNode_t;

typedef struct ifSNode{
	/*"if" condition "then" statement*/
	struct conditionNode *cond;
	struct statementNode *ifsts;
	struct statementNode *elsests;
}ifSNode_t;

typedef struct beginSNode{
	/*"begin" statement { ";" statement } "end" */
	struct statementNode *stsList;
	int numsStatement;
}beginSNode_t;

typedef struct assignSNode{
	/*ident ":=" expression*/
	struct identNode ident;
	struct expressionNode *exp;
}assignSNode_t;

typedef struct statementNode{
	enum{
		ASSIGN_S=1,
		BEGIN_S=2,
		IF_S=3,
		WHILE_S=4,
		READ_S=5,
		WRITE_S=6,
		CALL_S=7
	}tag;
	union{
		struct assignSNode *assigns_p;
		struct beginSNode *begins_p;
		struct ifSNode *ifs_p;
		struct whileSNode *whiles_p;
		struct readSNode *reads_p;
		struct writeSNode * writes_p;
		struct callNode *calls_p;
	}super;
	struct statementNode *next;
}statementNode_t;


typedef struct prodefNode{
	struct identNode ident;
	struct blockNode *body;
	struct prodefNode *next;
	int lexical;
	int addr;
	int paraProc;
}prodefNode_t;


typedef struct varDeclNode{	
	struct identNode ident;
	struct varDeclNode *next;
	int lexical;
	int paraProc;
	int addr;
}varDeclNode_t;


typedef struct constDeclNode{
	struct identNode ident;
	struct numberNode numer;
	struct constDeclNode *next;
	int lexical;
	int paraProc;
}constDeclNode_t;

typedef struct blockNode{
	struct constDeclNode *constDecl;
	int numsConstDecl;
	
	struct varDeclNode *varDecl;
	int numsVarDecl;
	
	struct prodefNode *procDef;
	int numsProcDef;
	
	struct statementNode *statement;
	int numsStatement;
	
	struct blockNode *next;
}blockNode_t;

typedef struct programNode{
	struct blockNode *block;
	int numsBlock;
}programNode_t;




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
	FILE *stdout;
	FILE *tokenFile;
	CurrentToken_t curToken;
	int tokenCount;
	struct SymTable *symbolTable;
	struct programNode *ast;
	int (*parse)(struct Parse *this);
	void (*printAST)(struct Parse *this,FILE *stdout);
	void (*unParse)(struct Parse *this);
	void (*cleanup)(struct Parse *this);
	void (*run)(struct Parse *this,FILE *tFile);
	void (*exit)(struct Parse *this);
	void (*unParsePrint)(struct Parse *this,FILE *stdout);
}Parse_t;

extern void cleanup(struct Parse *this);
extern void unParse(struct Parse *this);
extern int parse(struct Parse *this);
extern void printAST(struct Parse *this,FILE *stdout);

extern struct Parse * parse_init(struct SymTable *syms);
extern char *opSymbol[];
#endif
