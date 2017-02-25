#include "../include/symtable.h"

int main(int argc, char* argv[]){
	char *keys[]={"a1","b2","c3","d5","eh","we","af","dddef","affed","3fffef"};
	int nBukcet = 20;
	int i = 0;
	SymTable_T *symbol = SymTable_init(20);
	for(i=0;i<10;i++){
		loginfo("[%d] - %s\t%p\n",i,keys[i],&keys[i]);
		symbol->put(symbol,keys[i],(void*)&keys[i]);
	}

	symbol->printinfo(symbol,1);



	for(i=0;i<10;i++){

		symbol->remove(symbol,keys[i]);
	}

	symbol->printinfo(symbol,1);

	
	symbol->clean(symbol);
	return 0;
}

