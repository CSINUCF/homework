#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H
/***
 * @Author: Bingbing Rao
 * @E-mail: Bing.Rao@outlook.com
 */

#include "common.h"

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define COMMON_REGISTER_NUMBER 16

/*
 * Define "RPINT_VM_STATE" macro to debug the state of 
 * virtual machine when it executed an instruction
 */

#define RPINT_VM_STATE

/*
 *unparse the machine code to assembly code when you define
 * "UNPARSER_CODE" macro
 */

#define UNPARSER_CODE

/*
 * Print the machine code pretty when you define "PRETTY_PRINT_CODE"
 */

//#define PRETTY_PRINT_CODE

/*
 * The VM will output the result to the console when you define "OUTPUT_TO_CONSOLE" macro.
 */
//#define OUTPUT_TO_CONSOLE

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
typedef enum {
	OUTPUT = 1, //print the data to the console
	INPUT = 2, // read data from the console
	HALT = 3 //set halt flag to one
}sio_m;
typedef enum {
	UNINIT = 0,
	IDLE = 1,
	RUNNING = 2,
	HALTING = 3,
	ERROR = 4
}vm_status;
typedef struct virtualMachine{
	int sp; //stack pointer
	int bp; //base pointer 
	int pc; //program counter
	int pre_pc;
	instruction_t ir; //instruction register
	int r[COMMON_REGISTER_NUMBER];// register file
	int stack[MAX_STACK_HEIGHT]; //a stack which store the data
	instruction_t code[MAX_CODE_LENGTH]; //code segment to store the code
	int frameBreak[MAX_LEXI_LEVELS]; // To identify a AR
	int frameLevel;
	int numInstructions;
	int status;


	/*input file*/
	char* in_path;
	/*output file*/
	char* out_path;
	FILE * out_fp;

	/*define some core function to manage and execute vm*/
	int (*init)(struct virtualMachine *vm,int argc, char* argv[]);
	void (*exit)(struct virtualMachine *vm);
	int (*load_code)(struct virtualMachine *vm);
	void(*stdout)(struct virtualMachine *vm,char *format, ...);
	int (*base)(struct virtualMachine *vm,int level,int base);
	boolean (*prefetch)(struct virtualMachine *vm);
	void (*execute)(struct virtualMachine *vm);
	void (*run)(struct virtualMachine *vm);
	void (*prettyinfo)(struct virtualMachine *vm,int option);
}virtualMachine_t;
extern int init_vm(struct virtualMachine *vm,int argc, char* argv[]);
#endif
