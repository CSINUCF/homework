#ifndef DFA_H
#define DFA_H

#include "common.h"
#include "scanner.h"

#define MAX_IDENT_LENGTH 11
#define MAX_RESERVED_WORD 15
#define MAX_BUFFER_SIZE (MAX_IDENT_LENGTH+2)
#define MAX_NUMBER_LENGTH 5


// a set of State for DFA machine
typedef enum{
	D_START=0,
	D_DIGIT=1, 		//digital number [0-9]
	D_ALPHA=2, 		//[a-z,A-Z]
	D_PLUS=3,  		//"+"
	D_MINUS=4, 		//"-"
	D_MULT=5,	 	//"*"
	D_DIV=6,   		//"/"
	D_EQL=7,   		//"="
	D_LESS=8,  		//"<"
	D_LARE=9,  		//">"
	D_LEQ=10,   	//"<="
	D_GEQ=11,   	//">="
	D_NEQ=12,   	//"<>"
	D_LPARE=13, 	//"("
	D_RPAPE=14, 	//")"
	D_COMMA=15, 	//","
	D_SEMI=16,  	//";"
	D_PERIOD=17, 	//"."
	D_COLON=18, 	//":"
	D_BECOMES=19, 	//":="
	D_LCOMM=20,    	//"/*"
	D_COMM=21,    	//"/* *"
	D_RCOMM=22,     // "/*  */"
	D_ACCEPT=23,    // accepted
	D_REJECT=24,   //  rejected
	D_FINAL=25    // last state and reserved
}DFA_Q;



typedef enum {
	M_IDLE=1,
	M_RUN, //looking for the lexeme
	M_HALT //end of looking for next lexeme
}MachineState;



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


typedef struct lexeme{
	Token_t type;
	char buffer[MAX_BUFFER_SIZE];
	int indexBuffer;
	boolean printable;
	struct lexeme* next;
}LexemeEntry_t;

typedef struct DFA{

	MachineState MState;
	DFA_Q DFAState;
	boolean rewind;
	

	//Record the location in input file
	unsigned int line;  // current line number
	unsigned int column;// current colum number
	unsigned int lastLineLength;

	
	LexemeEntry_t curLexeme;
	LexemeEntry_t* tokenTable;
	LexemeEntry_t* lastToken;
	
	// Transition Diagram
	void (*transition)(struct DFA* this,int input);
	void (*reset)(struct DFA* this);
	void (*accept)(struct DFA* this);
	void (*reject)(struct DFA* this);
	void (*exit)(struct DFA *this);
	boolean (*getNextLexeme)(struct DFA* this,FILE *input);
	void (*toString)(struct DFA* this);
	void (*updateReservedWords)(struct DFA* this);
	int  (*run)(struct DFA *this,char *path);
	void (*putLexeme)(struct DFA *this);
	void (*printLexme)(struct DFA *this,char *path);
}DFA_T;


extern struct DFA *dfa_init();
#endif
