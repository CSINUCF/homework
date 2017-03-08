#include "../include/scanner.h"

int main(int argc, char* argv[]){
	char *path = NULL;
	switch(argc){
		case 1:{
			path = "../input/dfa.txt";
		}break;
		case 2:{
			
		}break;
		case 3:{
			path = argv[2];
		}break;
		default:{
			logerror("The input parameters error\n");
			return -1;
		}
	}
	Scanner_t *scanner = scanner_init(100);
	if(scanner == NULL){
		logerror("init a scanner failed\n");
		return -1;
	}
	scanner->run(scanner,path);

	//scanner->printLexmeList(scanner,path);

	struct SymTable *symbolTable = scanner->symbolTable;

	//symbolTable->printinfo(symbolTable,2);

	scanner->exit(scanner);

	free(scanner);
	return 0;
}

