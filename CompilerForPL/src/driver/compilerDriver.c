#include "../include/driver.h"

boolean stdoutParse = FALSE;
boolean stdoutToken = FALSE;
boolean stdoutVirtm = FALSE;
void driver_run(struct CompilerDriver *this,char *path){
	
	char *tokenFile = "output/tokenFile.txt";
	remove(tokenFile);
	FILE *tokenF = fileCreateAndOpen(tokenFile);
	char *stdout_parse = "output/stdout_parse.txt";remove(stdout_parse);
	FILE *std_parse = fileCreateAndOpen(stdout_parse);
	// run scanner to scan the source code and generate token file
	this->scanner->run(this->scanner,path);

	if(stdoutToken == TRUE){
		this->scanner->printLexmeList(this->scanner,path);
	}
	
	//write the current token list to the file which is the input of parser.
	this->scanner->outputLexmeList(this->scanner,tokenF);
	fclose(tokenF);


	
	// run parser on the token file
	this->parse->run(this->parse,fileCreateAndOpen(tokenFile));
	this->parse->unParsePrint(this->parse,std_parse);


	char *astFile = "output/ast.txt";remove(astFile);
	FILE *std_ast = fileCreateAndOpen(astFile);
	this->parse->printAST(this->parse,std_ast);
	fclose(std_ast);
	fclose(std_parse);

	printProcedureCode();
	if(stdoutParse == TRUE){
		// output the machine code to the screen
	}
	this->sym->printinfo(this->sym,2);
	
	if(stdoutVirtm == TRUE){
		// output the vm trace to the screen
	}
	return;	
}

void driver_cleanup(struct CompilerDriver *this){
	struct Scanner *scanner = this->scanner;
	struct Parse *parse = this->parse;
	struct SymTable *sym = this->sym;
	struct virtualMachine *vm = this->vm;

	if(scanner != NULL){
		scanner->exit(scanner);
	}
	
	if(parse != NULL){
		parse->exit(parse);
	}
	
	if(sym != NULL){
		sym->clean(sym);
	}
	
	if(vm != NULL){
		vm->exit(vm);
	}
	return;

}
struct CompilerDriver *init_driver(){

	#define SYMBOL_MAX_NUMS	1000
	CompilerDriver_t *driver = (CompilerDriver_t *)calloc(1,sizeof(CompilerDriver_t));
	if(driver == NULL){
		logerror("initial a compiler driver object failed\n");
		return NULL;
	}else{
		
		driver->cleanup = driver_cleanup;
		driver->run = driver_run;
		
		driver->sym = SymTable_init(SYMBOL_MAX_NUMS);
		if(driver->sym  == NULL){
			logerror("init a symbol table failed\n");
			driver->cleanup(driver);
			return NULL;
		}
		driver->scanner = scanner_init(driver->sym);
		if(driver->scanner == NULL){
			logerror("init a scanner failed\n");
			driver->cleanup(driver);
			return NULL;
		}
		driver->parse = parse_init(driver->sym);
		if(driver->parse == NULL){
			logerror("init a parse failed\n");
			driver->cleanup(driver);
			return NULL;
		}
		driver->vm = init_vm();
		if(driver->vm == NULL){
			logerror("init a vm failed\n");
			driver->cleanup(driver);
			return NULL;
		}
		logdebug("init a driver successfully\n");
		return driver;
	}	
}

static inline void help()
{
	#define USAGE1 "  1 Usage: ./compile -h \n"
	#define USAGE2 "  3 Usage: ./compile -i inFile [-l|-a|-v]\n"
	logpretty("\n\n*************************************************************\n");
	logpretty("The help instruction for Compile for PL\\0 Language\n");
	logpretty("\nUsage: ./compile [OPTION] ... [FILE] ...\n");
	logpretty("Options:\n");
	logpretty("   -h\t\tprint out the help info\n");
	logpretty("   -i inFile\tspecific the input files that compiler will execute\n");
	logpretty("   -l \t\tprint the list of lexemes/tokens (scanner output) to the screen\n");
	logpretty("   -a \t\tprint the generated assembly code (parser/codegen output) to the screen\n");
	logpretty("   -v \t\tprint virtual machine execution trace (virtual machine output) to the screen\n");
	logpretty("Example:\n");
	logpretty(USAGE1);
	logpretty(USAGE2);
	logpretty("*************************************************************\n");
}

void printCommand(int argc, char* argv[]){
	int i = 0;
	for(i=0;i<argc;i++){
		logpretty("%s ",argv[i]);
	}
	logpretty("\n");
}

int main(int argc, char* argv[]){
	char *path = NULL;
	int i = 0;
	//check the input parameters
	for(i=1;i<argc;i++){
		if(strcmp(argv[i],"-h") == 0){
			help();
			return -1;
		}else if(strcmp(argv[i],"-i") == 0){
			if((i+1)<argc){
				path = argv[++i];
			}else{
				logerror("input parameter error\n");
				printCommand(argc,argv);
				help();
				return -1;
			}
		}else if(strcmp(argv[i],"-l") == 0){
			stdoutToken = TRUE;// print tokens to the screen
		}else if(strcmp(argv[i],"-a") == 0){
			stdoutParse = TRUE; //print the assembly code to the screen
		}else if(strcmp(argv[i],"-v") == 0){
			stdoutVirtm = TRUE; // Print the machine execution trace to the screen
		}else{
			logerror("input parameter error\n");
			printCommand(argc,argv);
			help();
			return -1;
		}
	}
	if(path == NULL){
		logerror("input parameter error\n");
		printCommand(argc,argv);
		help();
		return -1;
	}	
	loginfo("The input file is:%s\n",path);
	struct CompilerDriver *driver = init_driver();
	if(driver != NULL){
		driver->run(driver,path);
		driver->cleanup(driver);
	}
	free(driver);
	return 0;
}

