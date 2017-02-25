# 1. Description

Bingbing Rao     Robin.Rao@knights.ucf.edu

This is an implementation of a Compiler for PL/0 program language.

Directories and files:

- **bin:** C execute file compiled by gcc
- **include:** The commond c head files
- **input:** The input file that will be use in C-unit Test Case
- **output:** The directory stored the output files
- **Scanner:** the source code for scanner
- **SymTable:** the source code for symbol table
- **VirtualMachine:** The source code of P-Machine virtual machine
- **Makefile:** The build configuration for make command
- **README.md** Documents about this project.

To run the examples, you need the following programs installed:

- **Unix/Linux compile environment**
- **gcc**
- **make**

# 2. Finished Work

In this project, I have implemented the following components of a compiler for PL/0 program language.

- **Virtual Machine**
	- Using 16 common registers to facilitate the process of virtual machine;
	- handle the function or method invoke; 
	- output to a specific file or system stdout;
	- When we print the stack info, using "|" symbol to separate each AR; 

- **Symbol Table**
	- use hashtable to store symbol entry.
	- look up a symbol with a specific key.
	- for each key in a symbol, there is a struct named value to store its attributes.
	- Using a key of symbol to get its attribute.
	- get the hash value for each symbol.
	- judge a symobl exist via the key.
	- update the attribute value of a symbol in the table.
    
- **Scanner**
	- DFA machine to recognize a string to generate a lexeme.
	- Using a list to store the lexemes;
	- Print the lexemes with source code;
	- Parsing a symbol from the list of lexemes and then using a hashtable to store them.

	

  

# 3. User Mannual
## 3.1 Virtual Machine
The Makefile supports the following targets:

- **make vm-build:** compiles all c code in this directory;
- **make vm-test:** Run all test cases;

	1) **make vm-test1:**  Runing test case 1 to demonstrate the basic function of VM;
    2) **make vm-test2:**  Runing test case 2 to verify the invoking a function; 
    3) **make vm-test3:**  Runing test case 3 to output the results to the system stdout;

- **make clean:** clean the temp files;

When you use the "make" command to compile the project successfully, there would gernerate a execute file named "vm" in
bin directory. You can execute this file with some input parameters directly. You can get more help information via the
following command.

- **./bin/vm -h**

Here are some examples about "vm":

- **./bin/vm -i ./input/vm-default.txt -o ./output/vm-default-out.txt**
- **./bin/vm -i ./input/vm-call.txt -o ./output/vm-call-out.txt**
- **./bin/vm -i ./input/vm-call.txt**

The options of "vm" executing file:

- **"-h :"** print out the help info;
- **"-i :"** specific the input files that p-machine virtual machine will execute; 
- **"-o :"** specific the output files, if there is no output files, the virtual machine will output to system stdout;
## 3.2 Symbol Table

## 3.3 Scanner

- **make scanner-build:** compile scanner source code;
- **make scanner-test:** Run all scanner test cases;
- **make clean:** clean the temp files;

Here are some examples about "scanner":

 - **./bin/scanner -i ./input/dfa.txt**

The options of "scanner" executing file:

- **"-i :"** specific the input files; 

