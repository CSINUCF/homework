#include "../include/gencode.h"


//Global Variables
struct instruction finalCode[MAX_CODE_SETS * MAX_CODE_LENGTH]; //holds the finalized code
int finalInstructionCount = 0;

struct instruction generatedCode[MAX_CODE_SETS][MAX_CODE_LENGTH]; //holds the compiled mcode sets
int instructionCount[MAX_CODE_SETS]; //stores the number of instructions in the code
int inUse[MAX_CODE_SETS]; //stores whether a set is in use or not.

FILE* mcode; //file pointer to the mcode file

#define FINAL_CODE -1

struct instruction newInstruction(int op,int r,int l,int m)
{
    struct instruction i;
    i.op = op;
    i.l = l;
	i.r = r;
    i.m = m;
    return i;
}
//Funcitons

/*
    Produces the mcode file.

    Output:
        0 if there was an error.
        1 otherwise.
*/

int generate(int mcodeDirective)
{
    prepareMcode();
#if 0
    if (!openMcodeFile(&mcode, "w"))
        return 0;

    printMcode(mcodeDirective);

    if (!closeMcodeFile(&mcode))
        return 0;

    if (DEBUG) printf("\n");
#endif
    return 1;
}

/*
    Combines code sets into the final mcode.
*/
void prepareMcode()
{
    int sets = 0;
    int i;
    for (i = 0; i < MAX_CODE_SETS; i++) {
        if (instructionCount[i] == 0) {
            sets = i;
            break;
        }
    }

    if (sets == 0)
        return;

    generateProcedureJumps(FINAL_CODE, sets);

    for (i = sets - 1; i >= 0; i--) {
        changeM(FINAL_CODE, i, finalInstructionCount); //update jump to accurate address

        int j;
        for (j = 0; j < instructionCount[i]; j++) {
            struct instruction inst = generatedCode[i][j];
            appendCode(FINAL_CODE, inst.op, inst.r,inst.l, inst.m);

            if (inst.op == JMP || inst.op == JPC) //increase jumps by procedure offset
                changeM(FINAL_CODE, finalInstructionCount - 1, inst.m+ finalCode[i].m + 1);
        }
    }
}

void printProcedureCode(){
	int i,j;
	for(i=0;i<MAX_CODE_SETS;i++){
		for(j=0;j<instructionCount[i];j++){
			unparseIr(NULL,i,&generatedCode[i][j]);
			logpretty("\n");
		}
	}

}
/*
    Prints the mcode instructions to the mcode file.
*/
void printMcode(int mcodeDirective)
{
    int i;
    for (i = 0; i < finalInstructionCount; i++) {
        if (DEBUG) printf("Producing Instruction: %d %d %d\n", finalCode[i].op, finalCode[i].l, finalCode[i].m);

        putInstruction(finalCode[i]);

        if (mcodeDirective == 1) {
            printf("%d %d %d\n", finalCode[i].op, finalCode[i].l, finalCode[i].m);
        }
    }
}

/*
    Prints an instruction to the mcode file.

    Input:
        instruction : The instruction to print.
*/
void putInstruction(struct instruction instruction)
{
    CompilerStdout(mcode, "%d %d %d %d\n", instruction.op,instruction.r,instruction.l, instruction.m);
}

/*
    Adds an instruction to the end of the instruction array.

    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
*/
void appendCode(int set, int op,int r,int l,int m)
{
    if (set == FINAL_CODE) {
        finalCode[finalInstructionCount] = newInstruction(op,r,l, m);
        finalInstructionCount++;
    }
    else {
        generatedCode[set][instructionCount[set]] = newInstruction(op,r,l, m);
        instructionCount[set]++;
    }
}

/*
    Adds an instruction at the beginning of the instruction array.
    
    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
*/
void prependCode(int set,int op,int r,int l,int m)
{
    insertCode(set,op,r,l,m,0);
}

/*
    Adds an instruction at a certain index of the instruction array.
    
    Input:
        set : The code set, -1 for final code set.
        op : The op-code of the instruction to add.
        l : The lexigraphical level of the instruction to add.
        m : The data element of the instruction to add.
        index : The index of where to insert the instruction.
*/
void insertCode(int set, int op, int r,int l,int m, int index)
{
    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1) {
            if (finalInstructionCount == 0)
                appendCode(set, op,r,l, m);
            return;
        }
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1) {
            if (instructionCount[set] == 0)
                appendCode(set, op,r,l, m);
            return;
        }
    }

    makeSpaceInCode(set, 1, index);

    if (set == FINAL_CODE) {
        finalCode[index] = newInstruction(op,r,l, m);
        finalInstructionCount++;
    }
    else {
        generatedCode[set][index] = newInstruction(op,r,l, m);
        instructionCount[set]++;
    }
}

/*
    Makes space for new instructions in the instruction array.

    Input:
        set : The code set, -1 for final code set.
        size : The size of the space to make.
        index : The starting index of the space.
*/
void makeSpaceInCode(int set, int size, int index)
{
    if (size <= 0)
        return;

    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1)
            return;
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1)
            return;
    }

    int i;
    if (set == FINAL_CODE) {
        for (i = finalInstructionCount - 1; i >= index; i--)
            finalCode[i + size] = finalCode[i];
    }
    else {
        for (i = instructionCount[set] - 1; i >= index; i--)
            generatedCode[set][i + size] = generatedCode[set][i];
    }
}

/*
    Produces code for a numerical literal which is load a constant value(literal) m into register r.
*/
void generateLiteral(int set,int r,int value)
{
    appendCode(set,LIT,r,0,value);
}
/*
    Produces code for a return.

    Input:
        set : The code set, -1 for final code set.
*/
void generateReturn(int set)
{
   	appendCode(set,RTN,0,0,0);
}
/*
    Produces code for loading a symbol identifier.

    Input:
        set : The code set, -1 for final code set.
        ident : The symbol identifier.
        currentLevel : The current level of the program.
*/
void generateLoad(int set,int r,struct Symbol *ident, int currentLevel)
{
    appendCode(set,LOD,r,currentLevel - ident->level,ident->addr);
}

/*
    Produces code for storing a symbol identifier value.

    Input:
        set : The code set, -1 for final code set.
        ident : The symbol identifier.
        currentLevel : The current level of the program.
*/
void generateBecomes(int set,int r,struct Symbol *ident, int currentLevel)
{
    appendCode(set,STO,r,currentLevel - ident->level, ident->addr);
}
/*
    Produces code for a call.

    Input:
        set : The code set, -1 for final code set.
        ident : The procedure identifier.
        currentLevel : The current level of the program.
*/
void generateCall(int set, struct Symbol *ident, int currentLevel)
{
    appendCode(set, CAL,0,currentLevel - ident->level, ident->addr);
}
/*
    Produces code for creating variable space.

    Input:
        set : The code set, -1 for final code set.
        space : The number of variables and constants in the block.
*/
void generateVariableSpace(int set, int space)
{
    prependCode(set,INC,0,0,space);
}
/*
    Produces code for a generic jump.

    Input:
        set : The code set, -1 for final code set.
*/
void generateJump(int set)
{
    appendCode(set,JMP,0,0,0);
}

/*
    Produces code for a generic conditional jump.

    Input:
        set : The code set, -1 for final code set.
*/
void generateConditionalJump(int set,int r)
{
    appendCode(set,JPC,r,0,0);
}
/*
    Produces code for a IO read.

    Input:
        set : The code set, -1 for final code set.
        ident : The variable identifier.
        currentLevel : The currentLevel of the program.
*/
void generateRead(int set,int r,struct Symbol *ident, int currentLevel)
{
    appendCode(set,SIO,r,0, 2);
    appendCode(set, STO,r,currentLevel - ident->level, ident->addr);
}

/*
    Produces code for an IO write.

    Input:
        set : The code set, -1 for final code set.
        ident : The identfier.
        currentLevel : The current level of the program.
*/
void generateWrite(int set,int r,struct Symbol *ident, int currentLevel,int type)
{
	if(type == VAR_S)
    	appendCode(set, LOD,r, currentLevel - ident->level, ident->addr);
	else if(type == CONST_S)
		appendCode(set, LOD,r, 0,ident->val);
    appendCode(set, SIO,r, 0, 1);
}

/*
    Produces code for a halt.

    Input:
        set : The code set, -1 for final code set.
*/
void generateHalt(int set)
{
    appendCode(set,SIO,0,0,3);
}

/*
    Produces code for a calculation.

    Input:
        set : The code set, -1 for final code set.
        token : The calculation token.
*/
void generateCalculation(int set,int token,int i,int j, int k)
{
	int op = 0;
	switch(token){
		case plussym: 	op = ADD;	break;
		case minussym: 	op = SUB;	break;
		case multsym:	op = MUL;	break;
		case slashsym:	op = DIV;	break;
	}
	
	appendCode(set,op,i,j,k);
}

/*
    Produces code for a relational operator.

    Input:
        set : The code set, -1 for final code set.
        token : The relational operator token.
*/
void generateRelOp(int set,int token,int i,int j,int k)
{
	int op = 0;
	switch(token)
	{
		case oddsym:	op = ODD;	break;
		case eqlsym:	op = EQL;	break;
		case neqsym:	op = NEQ;	break;
		case lessym:	op = LSS;	break;
		case leqsym:	op = LEQ;	break;
		case gtrsym:	op = GTR;	break;
		case geqsym:	op = GEQ;	break;
	}
	appendCode(set,op,i,j,k);
}

/*
    Produces code for a negation.

    Input:
        set : The code set, -1 for final code set.
*/
void generateNegation(int set,int i,int j,int k)
{
   appendCode(set,NEG,i,j,k);
}

/*
    Produces code for a posification.

    Input:
        set : The code set, -1 for final code set.
*/
void generatePosification(int set)
{
/*
    generateLiteral(set,0,generatedCode[set][getCodeLength(set) - 1].m);
    generateLiteral(set,0,0);
    //generateCalculation(set, OPR_LSS + (plussym - OPR_ADD));

    generateConditionalJump(set);
    int j1 = getCodeLength(set) - 1;

    //generateCalculation(set, OPR_NEG + (plussym - OPR_ADD));

    changeM(set, j1, getCodeLength(set));
    */
}


/*
    Produces code for preliminary procedure jumps.

    Input:
        set : The code set, -1 for final code set.
        space : The number of procedures to add jumps for.
*/
void generateProcedureJumps(int set, int space)
{
    int i;
    for (i = 0; i < space; i++)
        generateJump(set);
}
/*
    Returns the code length of a code set.

    Input:
        set : The code set, -1 for final code set.

    Output:
        The length of the code set.
*/
int getCodeLength(int set)
{
    if (set == FINAL_CODE)
        return finalInstructionCount;
    else
        return instructionCount[set];
}

/*
    Edits the M value of an instruction.

    Input:
        set : The code set, -1 for final code set.
        index : The index in the code set.
        m : The new m value.
*/
void changeM(int set, int index, int m)
{
    if (set == FINAL_CODE) {
        if (index < 0 || index > finalInstructionCount - 1)
            return;
        finalCode[index].m = m;
    }
    else {
        if (index < 0 || index > instructionCount[set] - 1)
            return;
        generatedCode[set][index].m = m;
    }
}
