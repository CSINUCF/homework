CC=gcc
ROOT_DIR=$(shell pwd)
BIN=compiler
OBJS_DIR=obj
BIN_DIR=bin
SRC_DIR=src
TEST_INPUT=input
TEST_OUTPUT=output
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR

CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

all:build test

################################################               
#		    build all c files                  #
################################################
build:check_deps $(SRC_DIR) $(CUR_OBJS)

check_deps:
	@mkdir -p ${ROOT_DIR}/${OBJS_DIR}
	@mkdir -p ${ROOT_DIR}/${BIN_DIR}
	@mkdir -p ${ROOT_DIR}/${TEST_OUTPUT}


#compile the targer recursively
$(SRC_DIR):ECHO
	make -C $@
ECHO:
	@echo $(SRC_DIR)
#if current dir contains *.c file then compile them
$(CUR_OBJS):%.o:%.c
	$(CC) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@
	

test:build test_vm test_scanner test_symbol

################################################               
#		    Virtual Machine Test               #
################################################
VMOBJS=${OBJS_DIR}/test_vm.o ${OBJS_DIR}/virtualMachine.o
test_vm:${VMOBJS}
	${CC} -o ${BIN_DIR}/$@ $^
	${BIN_DIR}/$@ -i ${TEST_INPUT}/vm-default.txt -o ${TEST_OUTPUT}/vm-default-out.txt
	${BIN_DIR}/$@ -i ${TEST_INPUT}/vm-call.txt -o ${TEST_OUTPUT}/vm-call-out.txt
	${BIN_DIR}/$@ -i ${TEST_INPUT}/vm-call.txt

	
################################################               
#		    Scanner Test               	       #
################################################
SCANNEROBJS=${OBJS_DIR}/test_scanner.o ${OBJS_DIR}/scanner.o ${OBJS_DIR}/dfa.o ${OBJS_DIR}/symbolTable.o
test_scanner:${SCANNEROBJS}
	${CC} -o ${BIN_DIR}/$@ $^
	${BIN_DIR}/$@ -i ${TEST_INPUT}/dfa.txt


################################################               
#		    symbol Table Test                  #
################################################
SYMBOLOBJS=${OBJS_DIR}/test_symtable.o ${OBJS_DIR}/symbolTable.o
test_symbol:${SYMBOLOBJS}
	${CC} -o ${BIN_DIR}/$@ $^
	${BIN_DIR}/$@

clean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(BIN_DIR)
	@rm -fr ${TEST_OUTPUT}
