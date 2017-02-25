#ifndef COMMON_H
#define COMMON_H

/***
 * @Author: Bingbing Rao
 * @E-mail: Bing.Rao@outlook.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>



#define DEBUG

#define loginfo(format, ...)\
	printf("INFO " format,##__VA_ARGS__)

#define logpretty(format, ...)\
	printf(format,##__VA_ARGS__)

#define logerror(format, ...)\
	printf("ERROR %s:%d " format,__FILE__,__LINE__,##__VA_ARGS__)

#define vmstdout(vm,format, ...) do{\
	if(vm->out_fp != NULL){\
		fprintf(vm->out_fp,format,##__VA_ARGS__);\
	}\
	else\
		printf(format,##__VA_ARGS__);\
}while(0)




#ifdef DEBUG
#define logdebug(format, ...)\
	printf("DEBUG %s:%d " format,__FILE__,__LINE__,##__VA_ARGS__)
#else
#define logdebug(format, ...) 
#endif

typedef enum {
	FALSE = 0,
	TRUE = 1
}boolean;

static inline char *repeatString(char *str,int count) {
    if (count == 0) return NULL;
    char *ret = calloc(1,strlen(str)*count+count);
    if (ret == NULL) return NULL;
    strcpy (ret, str);
    while (--count > 0) {
        strcat (ret, " ");
        strcat (ret, str);
    }
    return ret;
}


static inline char *getIndent(int count){
	return repeatString("\t",count);
}

static inline char *getSpace(int count){
	return repeatString(" ",count);
}


static inline int findIndex(int argc, char* argv[],char *p){
	int i,idx = -1;
	for(i=0;i<argc;i++){
		if(strcmp(argv[i],p) == 0){
			idx = i;
			break;
		}
	}
	return idx;
}



#endif
