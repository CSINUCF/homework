/***
 * @Author: Bingbing Rao
 * @E-mail: Bing.Rao@outlook.com
 */
#include "../include/virtualMachine.h"

int main(int argc, char* argv[]){
	virtualMachine_t *vm = (virtualMachine_t *)calloc(1,sizeof(virtualMachine_t));
	vm->init = init_vm;
	if(vm->init(vm,argc,argv) == 0){
		vm->run(vm);
		vm->exit(vm);
	}
	return 0;
}
