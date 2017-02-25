/***
 * @Author: Bingbing Rao
 * @E-mail: Bing.Rao@outlook.com
 */

#include "../include/virtualMachine.h"

char *opcode[] = {
	"NUL", 		//0
	"LIT",		//1
	"RTN",		//2
	"LOD",		//3
	"STO",		//4
	"CAL",		//5
	"INC",		//6
	"JMP",		//7
	"JPC",		//8
	"SIO",		//9
	"NUL",		//10
	"NUL",		//11
	"NEG",		//12
	"ADD",		//13
	"SUB",		//14
	"MUL",		//15
	"DIV",		//16
	"ODD",		//17
	"MOD",		//18
	"EQL",		//19
	"NEQ",		//20
	"LSS",		//21
	"LEQ",		//22
	"GTR",		//23
	"GEQ"		//24
};
/****
 * Define VM API
 */	
static inline void run_vm(struct virtualMachine *vm){

	int run_cnt = 1;
	if(vm->status != IDLE){
		loginfo("The current vm is not idle, please double check the state of vm: %d\n",vm->status);	
	}else{
	   /*get the control of vm and set the vm status is running*/
		vm->status = RUNNING;
	    vm->prettyinfo(vm,0);
		vm->prettyinfo(vm,1);
		vmstdout(vm,"The initial state of Virtual machine is:\n");
		vmstdout(vm,"Line OP   R  L  M\tPC BP SP\n");
	    vm->prettyinfo(vm,2);
		vm->prettyinfo(vm,3);
	#ifdef RPINT_VM_STATE
		vmstdout(vm,"\nLine	Idx OP	 R	L  M\tPC BP SP\n");
	#endif
	}	
	while(vm->status == RUNNING){
		boolean reg_pre = vm->prefetch(vm);
		if(reg_pre == FALSE){
			vm->status = ERROR;
		}else{
			vm->execute(vm);
			vmstdout(vm,"%4d  ",run_cnt++);
			vm->prettyinfo(vm,2);
		}
	}
}


static inline boolean prefetch(struct virtualMachine *vm){
	int cur_pc = vm->pc;
	int new_pc = 0;
	
	if(cur_pc >= MAX_CODE_LENGTH){
		logerror("The code segment is overflow\n");
		return FALSE;
	}else if(cur_pc >= vm->numInstructions){
		logerror("The current instructions are excuted over\n");
		return FALSE;
	}else{
		vm->ir = vm->code[cur_pc];
		new_pc = cur_pc + 1;
	}
	/*update pc to point to next instruction*/
	vm->pc = new_pc;
	vm->pre_pc = cur_pc;
	
	return TRUE;
}


static inline void instruction_execution(struct virtualMachine *vm){
	
	int op = vm->ir.op;
	int i = vm->ir.r;
	int j = vm->ir.l;
	int k = vm->ir.m;
	
	int sp = vm->sp;
	int bp = vm->bp;
	int pc = vm->pc;
	int *r = vm->r;
	int *stack = vm->stack;
	int *frameBreak = vm->frameBreak;
	int frameLevel = vm->frameLevel;
	switch(op){
		case LIT:{
			//Loads a constant value (literal) M into Register R
			r[i] = k;
		}break;
		case RTN:{			
			sp = bp -1;
			//Returns from a subroutine and restore the caller environment
			if(bp == 1){
				vm->status = IDLE;/*Kill the simulation if we're at the base level*/
			}else{
				bp = stack[sp+3];
			}			
			pc = stack[sp+4];
			frameBreak[frameLevel] = 0;
            frameLevel--;
		}break;
		case LOD:{
			/*Load value into a selected register from the stack 
			 *location at offset M from L lexicographical levels down
			 */
			r[i] = stack[vm->base(vm,j,bp)+k];
		}break;
		case STO:{
			/*Store value from a selected register in the 
			 *stack location at offset M from L lexicographical levels down*/
			stack[vm->base(vm,j,bp)+k] = r[i];
		}break;
		case CAL:{ 
			//Call procedure at code index M (generates new Activation Record and pc <- M)
			stack[sp+1] = 0;
			stack[sp+2] = vm->base(vm,j,bp);
			stack[sp+3] = bp;
			stack[sp+4] = pc;
			bp = sp +1;
			pc = k;
			frameBreak[frameLevel] = sp;
    		frameLevel++;
		}break;
		case INC:{
			/*
			 *Allocate M locals (increment sp by M).First four are Functional Value, 
			 *Static Link (SL), Dynamic Link (DL), and Return Address (RA)
			 */
			sp = sp + k;
		}break;
		case JMP:{
			//Jump to instruction M
			pc = k;
		}break;
		case JPC:{ //Jump to instruction M if R = 0
			if(r[i] == 0)
				pc = k;
		}break;
		case SIO:{
			switch(k){
				case 1:{ //print(R[i]); Write a register to the screen
					#ifdef OUTPUT_TO_CONSOLE
					loginfo("\tOuput:%d\n",r[i]);
					#endif
				}break;
				case 2:{ //read(R[i]); Read in input from the user and store it in a register
					scanf("%d",&r[i]);
				}break;
				case 3:{//Set Halt flag to one, End of program (program stops running)
					vm->status = IDLE;
				}break;
				default:{
					logerror("The modify code is wrong: %d\n",k);
				}
			}
			
		}break;
		case NEG:{
			r[i] = -r[j];
		}break;
		case ADD:{
			r[i] = r[j]+r[k];
		}break;
		case SUB:{
			r[i] = r[j]-r[k];
		}break;
		case MUL:{
			r[i] = r[j]*r[k];
		}break;
		case DIV:{
			r[i] = r[j]/r[k];
		}break;
		case ODD:{
			r[i] = r[i]%2;
		}break;
		case MOD:{
			r[i] = r[j]%r[k];
		}break;
		case EQL:{
			r[i] = r[j] == r[k];
		}break;
		case NEQ:{
			r[i] = r[j] != r[k];
		}break;
		case LSS:{
			r[i] = r[j] < r[k];
		}break;
		case LEQ:{
			r[i] = r[j] <= r[k];
		}break;
		case GTR:{
			r[i] = r[j] > r[k];
		}break;
		case GEQ:{
			r[i] = r[j] >= r[k];
		}break;
		default:{
			logerror("The op code is fatal to the virtual machine, please double check\n");
		}
	}
	/*update the current pointer values*/
	vm->pc = pc;
	vm->bp = bp;
	vm->sp = sp;
	vm->frameLevel = frameLevel;
}

static inline void prettyprintInstruction(struct virtualMachine *vm,int idx,instruction_t *ir){
	vmstdout(vm,"%4d %3s %2d %2d %2d",idx,opcode[ir->op],ir->r,ir->l,ir->m);
}
static inline void prettyprintInstructionInDigital(struct virtualMachine *vm,int idx,instruction_t *ir){
	vmstdout(vm,"%4d %2d %2d %2d %2d",idx,ir->op,ir->r,ir->l,ir->m);
}
static inline int unparseInstruction(struct virtualMachine *vm,int idx,instruction_t *ir){
	int op = ir->op;
	int r = ir->r;
	int l = ir->l;
	int m = ir->m;
	switch(op){
		case LIT:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case RTN:{
			vmstdout(vm,"%4d\t%s %d,%d,%d",idx,opcode[op],r,l,m);
		}break;
		case LOD:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case STO:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case CAL:{
			vmstdout(vm,"%4d\t%s %d,%d,%d",idx,opcode[op],r,l,m);
		}break;
		case INC:{
			vmstdout(vm,"%4d\t%s %d,%d,%d",idx,opcode[op],r,l,m);
		}break;
		case JMP:{
			vmstdout(vm,"%4d\t%s %d,%d,%d",idx,opcode[op],r,l,m);
		}break;
		case JPC:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case SIO:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case NEG:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],%d",idx,opcode[op],r,l,m);
		}break;
		case ADD:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case SUB:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case MUL:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case DIV:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case ODD:{
			vmstdout(vm,"%4d\t%s R[%d],%d,%d",idx,opcode[op],r,l,m);
		}break;
		case MOD:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case EQL:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case NEQ:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case LSS:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case LEQ:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case GTR:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		case GEQ:{
			vmstdout(vm,"%4d\t%s R[%d],R[%d],R[%d]",idx,opcode[op],r,l,m);
		}break;
		default:{
			logerror("The opcode is wrong,please double check");
			return -1;
		}
	}
	return 0;
}
static inline void printVMState(struct virtualMachine *vm){
	int i = 0;
	int j = 0;
	prettyprintInstruction(vm,vm->pre_pc,&vm->ir);
	vmstdout(vm,"\t%2d %2d %2d\t",vm->pc,vm->bp,vm->sp);
	
	/*Print the current register info*/
	vmstdout(vm,"Register[%d]={",COMMON_REGISTER_NUMBER);
	for(i=0;i<COMMON_REGISTER_NUMBER;i++){
		if(i == COMMON_REGISTER_NUMBER-1)
			vmstdout(vm,"%d",vm->r[i]);
		else
			vmstdout(vm,"%d ",vm->r[i]);
	}
	vmstdout(vm,"}\t");

#if 0
	/*Print the info of current AR*/
	vmstdout(vm,"FrameLevel[%d]={",MAX_LEXI_LEVELS);
	for(i=0;i<MAX_LEXI_LEVELS;i++){
		if(i == MAX_LEXI_LEVELS -1)
			vmstdout(vm,"%d",vm->frameBreak[i]);
		else
			vmstdout(vm,"%d ",vm->frameBreak[i]);
	}
	logpretty("}\t");
#endif
	
	/*Print the current active stack info*/
	int pcnt = vm->sp >=1?vm->sp:1;
	vmstdout(vm,"Stack[%d]={",pcnt);
	for(i=0;i<pcnt;i++){
		if(i == pcnt-1)
			vmstdout(vm,"%d",vm->stack[i]);
		else
			vmstdout(vm,"%d ",vm->stack[i]);

		for(j = 0;j<MAX_LEXI_LEVELS;j++){
			if((i == vm->frameBreak[j])&&(i>1)){
				vmstdout(vm,"| ");
			}
		}
	}
	vmstdout(vm,"}\t");	
}


static inline void prettyPrintVMinfo(struct virtualMachine *vm,int option){	
	switch(option){
		case 0:{//print the machine code pretty
			#ifdef PRETTY_PRINT_CODE
			int i = 0;
			instruction_t *code = vm->code;
			//Printing the header
			vmstdout(vm,"The total number of ir is:%d\n",vm->numInstructions);
			vmstdout(vm,"Line OP   R  L  M\n");
			int numIr = vm->numInstructions;
			for(i=0;i<numIr;i++){
				prettyprintInstruction(vm,i,&code[i]);
				vmstdout(vm,"\n");
			}
			#endif
		}break;
		case 1:{//unparse the machine code to assemble code and print them out		
			#ifdef UNPARSER_CODE
			int i = 0;
			instruction_t *code = vm->code;
			//Printing the header
			vmstdout(vm,"The total number of ir is:%d\n",vm->numInstructions);
			vmstdout(vm,"Line\tInstruction\n");
			int numIr = vm->numInstructions;
			for(i=0;i<numIr;i++){
				unparseInstruction(vm,i,&code[i]);
				vmstdout(vm,"\n");
			}
			vmstdout(vm,"\n");
			#endif
		}break;
		case 2:{//Print current the state of Virtual Machine
			#ifdef RPINT_VM_STATE
			printVMState(vm);
			vmstdout(vm,"\n");
			#endif
		}break;
		case 3:{
			int i = 0;
			instruction_t *code = vm->code;
			vmstdout(vm,"The total number of ir is:%d\n",vm->numInstructions);
			vmstdout(vm,"Line OP  R  L  M\tLine\tInstruction\n");
			int numIr = vm->numInstructions;
			for(i=0;i<numIr;i++){
				prettyprintInstructionInDigital(vm,i,&code[i]);
				vmstdout(vm,"\t");
				unparseInstruction(vm,i,&code[i]);
				vmstdout(vm,"\n");
			}
		}break;
		case 4:{
			//TODO
		}break;
		default:{
			logerror("The option[%d] is wrong,please make sure that\n",option);
		}
	}
}
/*
 * looking for the base pointer
 */
static inline int base(struct virtualMachine *vm,int l,int b){
	int base = b;
	int level = l;
	while(level>0){
		base = vm->stack[base + 1];
		level--;
	}
	return base;
}


static inline void help()
{
	#define USAGE1 "  1 Usage: ./vm -h \n"
	#define USAGE2 "  2 Usage: ./vm -i inFile\n"
	#define USAGE3 "  3 Usage: ./vm -i inFile -o outFile\n"
	#define USAGE4 "  4 Usage: ./vm -o outFile -i inFile\n"
	logpretty("\n\n*************************************************************\n");
	loginfo("The help instruction for Virtual Machine\n");
	logpretty("\nUsage: ./vm [OPTION] ... [FILE] ...\n");
	logpretty("Options:\n");
	logpretty("   -h\t\tprint out the help info\n");
	logpretty("   -i inFile\tspecific the input files that p-machine virtual machine will execute\n");
	logpretty("   -o outFile\tspecific the output files, if there is no output files, the virtual machine will output to system stdout\n");
	logpretty("Example:\n");
	logpretty(USAGE1);
	logpretty(USAGE2);
	logpretty(USAGE3);
	logpretty(USAGE4);
	logpretty("*************************************************************\n");
}
static inline void usage(){
	logpretty("\n\nUsage: ./vm [OPTION] ... [FILE] ...\n\n");
}




/*load code from the text file and parse it into the instruction format*/
static inline int load_code(struct virtualMachine *vm){
	char *path = vm->in_path;
	FILE* fin = NULL;
	int pcounter = 0;
	fin = fopen(path,"r");
	if(fin == NULL){
		logerror("Open file failed, please double check: %s\n",path);
		return -1;
	}
	while(!feof(fin)){
		fscanf(fin, "%d ",&vm->code[pcounter].op);
		fscanf(fin, "%d ",&vm->code[pcounter].r);
		fscanf(fin, "%d ",&vm->code[pcounter].l);
		fscanf(fin, "%d ",&vm->code[pcounter].m);
		pcounter++;
		/*Checking for code overflow*/
		if(pcounter >= MAX_CODE_LENGTH){
			logerror("The code size is overflow\n");
			fclose(fin);
			return -1;
		}
	}
	fclose(fin);
	return pcounter;
}

static inline void exit_vm(struct virtualMachine *vm)
{
	if(vm->out_fp != NULL)
		fclose(vm->out_fp);
}

int init_vm(struct virtualMachine *vm,int argc, char* argv[]){

	switch(argc){
		case 1:{
			logerror("The command parameter does not work properly,please make sure, option[%d]\n",argc);
			help();
			return -1;
		}break;
		case 2:{
			/*./vm -h*/
			
			if(strcmp(argv[1],"-h") == 0){
				help();	
			}else{
				logerror("The command parameter does not work properly,please make sure, option[%d]\n",argc);
				usage();
			}
			vm->status = ERROR;
			return -1;
		}break;
		case 3:{
			//TODO
		}break;
		case 4:{
			logerror("The command parameter does not work properly,please make sure, option[%d]\n",argc);
			help();
			vm->status = ERROR;
			return -1;
		}break;		
		case 5:{
			//TODO
		}break;
		default:{
			int i = 0;
			logerror("The command parameter does not work properly,please make sure, option[%d]\n",argc);
			for(i=0;i<argc;i++)
				logerror("argc[%d] %s\n",i,argv[i]);
			logerror("\n");
			help();
			vm->status = ERROR;
			return -1;
		}
	}
	
	int in_idx = findIndex(argc,argv,"-i");
	int out_idx = findIndex(argc,argv,"-o");
	vm->in_path = NULL;
	vm->out_path = NULL;

	
	if((in_idx != -1)&&(in_idx + 1 <argc)){
		vm->in_path = argv[in_idx+1];
	}
	
	if((out_idx != -1)&&(out_idx+1<argc)){
		vm->out_path = argv[out_idx+1];
		if( access(vm->out_path, F_OK ) == -1){			
			//ftruncate(vm->out_path,0);
			fclose(fopen(vm->out_path, "w"));
		}
		
		vm->out_fp = fopen (vm->out_path, "ab+");
	}
	vm->sp = 0;
	vm->bp = 1;
	vm->pc = 0;
	vm->pre_pc = 0;
	vm->ir.op = 0;
	vm->ir.r = 0;
	vm->ir.l = 0;
	vm->ir.m = 0;
	int i = 0;
	/*common register file set 0 initially*/
	for(i=0;i<COMMON_REGISTER_NUMBER;i++)
		vm->r[i] = 0;
	/*set stack with 0 value*/
	for(i=0;i<MAX_STACK_HEIGHT;i++)
		vm->stack[i] = 0;
	/*Set 0 for all the code segment*/
	for(i=0;i<MAX_CODE_LENGTH;i++){
		vm->code[i].op = 0;
		vm->code[i].r = 0;
		vm->code[i].l = 0;
		vm->code[i].m = 0;
	}
	for(i = 0; i < MAX_LEXI_LEVELS; i++)
	{
        vm->frameBreak[i] = 0;
	}
	vm->frameLevel = 0;	
	vm->base = base;
	vm->prettyinfo = prettyPrintVMinfo;
	vm->load_code = load_code;
	vm->execute = instruction_execution;
	vm->prefetch = prefetch;
	vm->run = run_vm;
	vm->exit = exit_vm;
	
	int numIR = vm->load_code(vm); //load the code to machine
	if(numIR == -1){
		logdebug("Error\n");
		vm->status = ERROR;
		return -1;
	}else{
		vm->numInstructions = numIR;
	}
	
	vm->status = IDLE;

	return 0;
}

