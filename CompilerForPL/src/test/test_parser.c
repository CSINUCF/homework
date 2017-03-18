#include "../include/parse.h"
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

	
	
	FILE* fin = fopen(path,"r");
	if(fin == NULL){
		logerror("Open file failed, please double check: %s\n",path);
		return -1;
	}
	loginfo("read file successfuly\n");

	struct Parse *p = parse_init(NULL);
	if(p != NULL){
		p->run(p,fin);
		p->unParsePrint(p,NULL);
		p->exit(p);
	}else{
		logerror("init a parse failed\n");
	}

	free(p);

	
	fclose(fin);
	
	return 0;
}

