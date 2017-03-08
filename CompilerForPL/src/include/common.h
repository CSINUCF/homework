#ifndef COMMON_H
#define COMMON_H

/***
 * @Author: Bingbing Rao
 * @E-mail: Bing.Rao@outlook.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>



#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define COMMON_REGISTER_NUMBER 16

#define MAX_IDENT_LENGTH 11
#define MAX_RESERVED_WORD 15
#define MAX_BUFFER_SIZE (MAX_IDENT_LENGTH+2)
#define MAX_NUMBER_LENGTH 5

#define DEBUG

#define loginfo(format, ...)\
	printf("INFO " format,##__VA_ARGS__)

#define logpretty(format, ...)\
	printf(format,##__VA_ARGS__)

#define logerror(format, ...)\
	printf("ERROR %s:%d " format,__FILE__,__LINE__,##__VA_ARGS__)

/*
  * out_fp:  a file pointer to point to a open file
  */
#define CompilerStdout(out_fp,format, ...) do{\
		if(out_fp != NULL){\
			fprintf(out_fp,format,##__VA_ARGS__);\
		}\
		else\
			printf(format,##__VA_ARGS__);\
	}while(0)
	
#ifdef DEBUG
#define logdebug(format, ...)\
	printf("DEBUG %s:%d " format,__FILE__,__LINE__,##__VA_ARGS__)
#else
#define logdebug(format, ...) 
#endif



typedef struct instruction{
	int op; /*Operation Code*/
	int r; /*Register*/
	int l; /*Lexicographical Level*/
	int m; /*Modifier*/
}instruction_t;

typedef enum {
	LIT = 1,
	RTN = 2,
	LOD = 3,
	STO = 4,
	CAL = 5,
	INC = 6,
	JMP = 7,
	JPC = 8,
	SIO = 9,

	//There are another two instructions with SIO Prefix
	SIO2 = 10,  // Reserve
	SIO3 = 11,  // Reserve
	
	NEG = 12,
	ADD = 13,
	SUB = 14,
	MUL = 15,
	DIV = 16,
	ODD = 17,
	MOD = 18,
	EQL = 19,
	NEQ = 20,
	LSS = 21,
	LEQ = 22,
	GTR = 23,
	GEQ = 24
}opcode_e;


//Lexeme Types
typedef enum token {
    nulsym = 1,        // "null"
    identsym = 2,      // a variable
    numbersym = 3,     // a number
    plussym = 4,       // "+"
    minussym = 5,      // "-"
    multsym = 6,       // "*"
    slashsym = 7,      // "/"
    oddsym = 8,        // "odd"
    eqlsym = 9,        // "="
    neqsym = 10,       // "<>"
    lessym = 11,       // "<"
    leqsym = 12,       // "<="
    gtrsym = 13,       // ">"
    geqsym = 14,       // ">="
    lparentsym = 15,   // "("
    rparentsym = 16,   // ")"
    commasym = 17,     // ","
    semicolonsym = 18, // ";"
    periodsym = 19,    // "."
    becomessym = 20,   // ":="
    beginsym = 21,     // "begin"
    endsym = 22,       // "end"
    ifsym = 23,        // "if"
    thensym = 24,      // "then"
    whilesym = 25,     // "while"
    dosym = 26,        // "do"
    callsym = 27,      // "call"
    constsym = 28,     // "const"
    varsym = 29,       // "var"
    procsym = 30,      // "procedure"
    writesym = 31,     // "write"
    readsym = 32,      // "read"
    elsesym = 33,      // "else"
    commentsym = 34    // "comments"
}Token_t;


typedef enum {
	FALSE = 0,
	TRUE = 1
}boolean;

static inline char *repeatString(char *str,int count) {
    if (count == 0) return NULL;
    char *ret = calloc(1,strlen(str)*count+count);
    if (ret == NULL) return NULL;
    strcpy (ret, str);
    while (--count > 0) {
        strcat (ret, " ");
        strcat (ret, str);
    }
    return ret;
}


static inline char *getIndent(int count){
	return repeatString("\t",count);
}

static inline char *getSpace(int count){
	return repeatString(" ",count);
}


static inline int findIndex(int argc, char* argv[],char *p){
	int i,idx = -1;
	for(i=0;i<argc;i++){
		if(strcmp(argv[i],p) == 0){
			idx = i;
			break;
		}
	}
	return idx;
}

#endif
