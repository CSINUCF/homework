#ifndef GENCODE_H
#define GENCODE_H
#include "common.h"
#include "parse.h"

typedef struct GenCode{
	instruction_t generatedCode[MAX_CODE_SETS][MAX_CODE_LENGTH];
	int instructionCount[MAX_CODE_SETS];
	int inUse[MAX_CODE_SETS];
}GenCode_t;


int generate(int mcodeDirective);

void prepareMcode();
void printMcode(int mcodeDirective);
void putInstruction(struct instruction instruction);

void appendCode(int set, int op,int r,int l,int m);
void prependCode(int set, int op,int r,int l,int m);
void insertCode(int set, int op, int r,int l, int m, int index);
void makeSpaceInCode(int set, int size, int index);

void generateLiteral(int set,int r, int value);
void generateLoad(int set, int r,struct Symbol *ident, int currentLevel);
void generateBecomes(int set,int r, struct Symbol *ident, int currentLevel);
void generateCalculation(int set,int token,int i,int j, int k);
void generateRelOp(int set, int token,int i,int j, int k);
void generateNegation(int set,int i,int j, int k);
void generatePosification(int set);
void generateVariableSpace(int set, int space);
void generateProcedureJumps(int set, int space);
void generateCall(int set, struct Symbol *ident, int currentLevel);
void generateRead(int set,int r,struct Symbol *ident, int currentLevel);
void generateWrite(int set,int r,struct Symbol *ident, int currentLevel,int type);
void generateJump(int set);
void generateConditionalJump(int set,int r);
void generateReturn(int set);
void generateHalt(int set);

int getCodeLength(int set);
void changeM(int set, int index, int m);
int getFreeSet();
void printProcedureCode();


#endif
