#ifndef DFA_H
#define DFA_H

#include "common.h"
#include "scanner.h"



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
	void (*outputLexme)(struct DFA *this,FILE *out);
}DFA_T;


extern struct DFA *dfa_init();
#endif
