#include "../include/parse.h"

int globalLevel = 0; //gives current level.
int offsetArray[MAX_LEXI_LEVELS]; //holds the offsets of each lexigraphical level
int procedureCount = 0; //holds the number of sub procedures
int currentProcedure = 0; //holds the current procedure
/*
 * For code generation, we are treating the register file as a stack.
 * The register pointer will point to top of "stack".
 * As calculations are performed, the register pointer will be appropriately incremented
 * and decremented. Calculation results should ultimately be stored in reg[0],
 * after which the result will be appropriately be stored back to memory.
 */
int reg_ptr = 0;


int inUse[MAX_CODE_SETS]; //stores whether a set is in use or not.
int getFreeSet()
{
    int i;
    for (i = 0; i < MAX_CODE_SETS; i++) {
        if (inUse[i] == 0) {
            inUse[i] = 1;
            return i;
        }
    }
    return -1;
}


/*
    Initializes offsets array.
*/
static inline void setOffsets()
{
    int x;
    for (x = 0; x < MAX_LEXI_LEVELS; x++)
    {
        offsetArray[x] = 4; //we are reserving stack space for activation record
    }
}
/*
    Adds a symbol to the symbol table.

    Input:
        token : The identifier of the symbol.
        type : The kind of the symbol.
*/
void makeSymbol(struct Parse *this,char *token, int type){
	CurrentToken_t *currentToken = &this->curToken;
	SymTable_T *symbolTable = this->symbolTable;
	Symbol_t * node = (Symbol_t *)calloc(1,sizeof(Symbol_t));
	if(node == NULL){
		logerror("create new a symbol node failed\n");
		return;
	}else{
		node->name = strdup(token);
		switch(type){
			case CONST_S:{				
				node->kind = CONST_S;
				node->val = currentToken->cTokenVal.numeric;
			}break;
			case VAR_S:{
				node->kind = VAR_S;
				node->val = 0;
				node->addr = offsetArray[globalLevel];
				offsetArray[globalLevel]++;//increment the offset inside by one, for any future variables
			}break;
			case PROC_S:{
				node->kind = PROC_S;
				node->val = 0;
				node->addr = ++procedureCount;
				currentProcedure = getFreeSet();
			}break;
		}
		node->level = globalLevel;

		symbolTable->put(symbolTable,node->name,node->level,node);
		
	}
}
static inline void getToken(struct Parse *this){
	CurrentToken_t *currentToken = &this->curToken;
	if(feof(this->tokenFile)){
		//if end of file, exit
		currentToken->cToken = nulsym;
		return;
	}
	//read the new token in
	fscanf(this->tokenFile, "%d ", (int*)(&currentToken->cToken));
	// update the content if current token is indent
	if(currentToken->cToken == identsym){
		fscanf(this->tokenFile, "%s ", currentToken->cTokenVal.string);
	}
	// update he content if current token is a number.
	if(currentToken->cToken == numbersym){
		fscanf(this->tokenFile, "%d ", &currentToken->cTokenVal.numeric);
	}
	this->tokenCount++;
}
/*
 factor ::= ident | number | "(" expression ")".
 */
int factorParse(struct Parse *this,factorNode_t *fn){
	int ret = 0;
	CurrentToken_t *curToken = &this->curToken;
	SymTable_T *symTable = this->symbolTable;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	switch (curToken->cToken){
		case identsym:{
			fn->tag = F_IDENT;
			fn->element.ident.string = strdup(curToken->cTokenVal.string);
			//get symbol from the symbol, and check
			struct Symbol *sym = symTable->get(symTable,fn->element.ident.string,globalLevel);
			if(sym == NULL){
				throwError(11);
			}else{
				switch (sym->kind){
					case VAR_S:{
						generateLoad(currentProcedure,reg_ptr,sym,globalLevel);					
					}break;
					case CONST_S:{
						generateLiteral(currentProcedure,reg_ptr,0,sym->val);
					}break;
					case PROC_S:{
						//error
						throwError(21);
					}break;
				}
			}
			reg_ptr++;
			getToken(this);
    	}break;
		case numbersym:{
			fn->tag = F_DIGIT;
			fn->element.digit.value = curToken->cTokenVal.numeric;
			generateLiteral(currentProcedure,reg_ptr,fn->element.digit.value);
			reg_ptr++; //Increment RP.
			getToken(this);
    	}break;
		case lparentsym:{
			fn->tag = F_EXP;
			expressionNode_t *exp = calloc(1,sizeof(expressionNode_t));
			if(exp == NULL){
				logerror("initial a expression node failed\n");
				return -1;
			}else{
				exp->numsTermExp = 0;
				getToken(this);
				ret += expressionParse(this,exp);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
			}
			fn->element.exp = exp;
			if(curToken->cToken != rparentsym){				
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(22);
			}
			getToken(this);
    	}break;
		default:{
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(23);
		}

	}
	return ret;
}
/*
term ::= factor {("*"|"/") factor}. 

*/
int termParse(struct Parse *this,termNode_t *tm,int preop){
	int ret = 0;
	CurrentToken_t *curToken = &this->curToken;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	factorExp_t *cur = NULL;
	tm->factorEList = calloc(1,sizeof(factorExp_t));
	if(tm->factorEList == NULL){
		logerror("initial a factor exp failed\n");
		return -1;
	}else{
		factorNode_t *fn = calloc(1,sizeof(factorNode_t));
		if(fn == NULL){
			logerror("initial a factor node failed\n");
			return -1;
		}
		ret += factorParse(this,fn);
		logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		tm->factorEList->factor =fn;
		tm->factorEList->next = NULL;
		tm->factorEList->op.type = nulsym;
		tm->numsFacorExp++;
		cur = tm->factorEList;		
	}
	
    switch (preop) {
    case plussym : //plus
        //generatePosification(currentProcedure);
        break;
    case minussym: //minus
        generateNegation(currentProcedure,reg_ptr-1,reg_ptr-1,0);
        break;
    }
    while(curToken->cToken == multsym || curToken->cToken == slashsym) {
		logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
		int op = curToken->cToken;
		factorExp_t *fe = calloc(1,sizeof(factorExp_t));
		if(fe == NULL){
			logerror("initial a factor EXP failed\n");
			return -1;
		}else{
			fe->next = NULL;
			fe->op.type = curToken->cToken; //6 if mult, 7 if div
			fe->factor = calloc(1,sizeof(factorNode_t));
			if(fe->factor == NULL){
				logerror("initial a factor node failed\n");
				return -1;
			}else{
				getToken(this);
				ret += factorParse(this,fe->factor);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
				generateCalculation(currentProcedure,op,reg_ptr-2,reg_ptr-2,reg_ptr-1);
				reg_ptr--;
			}
			cur->next = fe;
			cur = fe;	
			tm->numsFacorExp++;
		}
    }
	
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	return ret;
}
/*
expression ::= [ "+"|"-"] term { ("+"|"-") term}.

*/
int expressionParse(struct Parse *this, expressionNode_t *exp){
	int ret = 0;
	int preop = 0;
	CurrentToken_t *curToken = &this->curToken;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	termExp_t *cur = NULL;
	exp->termEList = calloc(1,sizeof(termExp_t));
	if(exp->termEList == NULL){
		logerror("initial term expe failed\n");
		return -1;
	}else{
		exp->termEList->next = NULL;
		if((curToken->cToken == plussym)||(curToken->cToken == minussym)){
			 exp->termEList->op.type= curToken->cToken;
			 preop = curToken->cToken;
			 getToken(this);
		}
		exp->termEList->term = calloc(1,sizeof(termNode_t));
		if(exp->termEList->term == NULL){
			logerror("initial a term node failed\n");
			return -1;
		}else{
			exp->termEList->term->factorEList = NULL;
			exp->termEList->term->numsFacorExp = 0;
		}
		ret += termParse(this,exp->termEList->term,preop);
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		exp->numsTermExp++;
		cur = exp->termEList;
	}
	while (curToken->cToken== plussym || curToken->cToken == minussym) {		
		logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
        int op = curToken->cToken;
		termExp_t *te = calloc(1,sizeof(termExp_t));
		if(te == NULL){			
			logerror("initial term expe failed\n");
			return -1;
		}else{
			te->next = NULL;
			te->op.type = op;
			te->term = calloc(1,sizeof(termNode_t));
			if(te->term == NULL){
				logerror("initial a term node failed\n");
				return -1;
			}else{
				getToken(this);
				ret += termParse(this,te->term,preop);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
				//generateCalculation(currentProcedure, op);
				cur->next = te;
				cur = te;
				exp->numsTermExp++;
			}
		}
        generateCalculation(currentProcedure,op,reg_ptr-2,reg_ptr-2,reg_ptr-1);
		reg_ptr--;
    }
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	return ret;
}
int relOp(struct Parse *this)
{
	int token = this->curToken.cToken;
    if (token != eqlsym && token != neqsym &&
        token != lessym && token != leqsym &&
        token != gtrsym && token != geqsym) {        
		logerror("The current token is:[%d,%s]\n",token,opSymbol[token]);
        throwError(20);
    }
    return token;
}

/*
condition ::= "odd" expression 
	  	| expression  rel-op  expression.  

*/
int conditionParse(struct Parse *this, conditionNode_t *cond){
	int ret = 0;
	CurrentToken_t *curToken = &this->curToken;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	cond->right = calloc(1,sizeof(expressionNode_t));
	if(cond->right == NULL){
		logerror("initial experssion left statement failed\n");
		return -1;
	}else{
		cond->right->termEList = NULL;
		cond->right->numsTermExp = 0;
	}
	if(curToken->cToken == oddsym){
		cond->op.type = oddsym;
		getToken(this);// get right experession
		ret += expressionParse(this,cond->right);
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		// generate a relop
        generateRelOp(currentProcedure,cond->op.type,reg_ptr-1,reg_ptr-1,0);
		//emit( 16 /*odd*/, reg_ptr - 1, reg_ptr - 1, 0 );
	}else{
		cond->left= calloc(1,sizeof(expressionNode_t));
		if(cond->left == NULL){
			logerror("initial experssion right statement failed\n");
			return -1;
		}else{
			cond->left->numsTermExp = 0;
			cond->left->termEList = NULL;
		}
		ret += expressionParse(this,cond->left); // set left experession
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		// generate a relop
		cond->op.type = relOp(this);
		
		getToken(this);//get right experession
		ret += expressionParse(this,cond->right);
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
        generateRelOp(currentProcedure,cond->op.type,reg_ptr-2,reg_ptr-2,reg_ptr-1);
		reg_ptr--;
	}
	return ret;
}

/*
 statement	 ::=    [ ident ":=" expression
 			 | "call" ident
			 | "begin" statement { ";" statement } "end" 
			 | "if" condition "then" statement 
		         | "while" condition "do" statement
			 | "read"  ident 
		 	 | "write" ident] .

 */
int statementParse(struct Parse *this,statementNode_t *sts){
	int ret =0;
	CurrentToken_t *curToken = &this->curToken;
	SymTable_T *symTable = this->symbolTable;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	switch (curToken->cToken){
		case identsym: {
			//  ident ":=" expression
			assignSNode_t *ass = calloc(1,sizeof(assignSNode_t));
			if(ass == NULL){
				logerror("initial a assignment state failed\n");
				return -1;
			}else{
				ass->ident.string = strdup(curToken->cTokenVal.string);
				ass->exp = calloc(1,sizeof(expressionNode_t));
				if(ass->exp == NULL){
					logerror("initial a expression failed\n");
					return -1;
				}else{
					ass->exp->numsTermExp = 0;
				}
			}
			//TODO get the symbol from table and check
			struct Symbol *sym = symTable->get(symTable,ass->ident.string,globalLevel);
			if(sym == NULL){
				throwError(11);
			}else{
				if(sym->kind != VAR_S){
					throwError(12);
				}
			}
			getToken(this);// get ":="
			if(curToken->cToken != becomessym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(29);
			}
			getToken(this); //get experession
			ret += expressionParse(this,ass->exp);
			if(ret != 0){ 
				logdebug("\n");
				return ret;
			}
			// Generate assig code
			generateBecomes(currentProcedure,reg_ptr - 1,sym, globalLevel);
			reg_ptr--;
			sts->super.assigns_p = ass;
			sts->tag = ASSIGN_S;
		}break;
		case callsym: {
			// "call" ident
			callNode_t *call = calloc(1,sizeof(callNode_t));
			if(call == NULL){
				logerror("initial call statement failed\n");
				return -1;
			}
			getToken(this); // get ident
			if(curToken->cToken != identsym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(14);
			}else{
				call->ident.string = strdup(curToken->cTokenVal.string);
			}
			// get symbol from table and check
			struct Symbol *sym = symTable->get(symTable,call->ident.string,globalLevel);
			if(sym == NULL){
				throwError(11);
			}else{
				if(sym->kind != PROC_S){
					throwError(15);
				}else{
					// generate call code
					//TODO	paramter input
					generateCall(currentProcedure, sym, globalLevel);
				}
			}
			sts->super.calls_p = call;
			sts->tag = CALL_S;
			getToken(this);
		}break;
		case beginsym: {
			//"begin" statement {";" statement } "end" 
			statementNode_t *stsP= NULL;
			beginSNode_t *beginS = calloc(1,sizeof(beginSNode_t));
			if(beginS == NULL){
				logerror("inital begin state failed\n");
				return -1;
			}else{
				beginS->numsStatement=0;
			}
			beginS->stsList = calloc(1,sizeof(statementNode_t));
			if(beginS->stsList == NULL){
				logerror("initial a statement failed\n");
				return -1;
			}else{
				getToken(this); // get next token
				ret += statementParse(this,beginS->stsList);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
				beginS->numsStatement++;
				stsP = beginS->stsList;
			}
			logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			while(curToken->cToken == semicolonsym){
				statementNode_t *s = calloc(1,sizeof(statementNode_t));
				getToken(this); // get next token
				ret += statementParse(this,s);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
				stsP->next = s;
				stsP = s;
				beginS->numsStatement++;
				logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			}		
			if(curToken->cToken != endsym){				
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(19);
			}
			sts->super.begins_p = beginS;
			// generated return function
			if (currentProcedure > 0) //no return for main procedure
				generateReturn(currentProcedure);
			sts->tag = BEGIN_S;
			getToken(this); // get next token
		}break;
		case ifsym: {
			// "if" condition "then" statement ["else" statement]
			ifSNode_t *ifs = calloc(1,sizeof(ifSNode_t));
			if(ifs == NULL){
				logerror("initial if statement failed\n");
				return -1;
			}else{
				ifs->cond = calloc(1,sizeof(conditionNode_t));
				if(ifs->cond == NULL){
					logerror("initial condition statement failed\n");
					return -1;
				}else{
					ifs->cond->left = NULL;
					ifs->cond->right = NULL;
				}
				ifs->ifsts = calloc(1,sizeof(statementNode_t));
				if(ifs->ifsts == NULL){
					logerror("initial statement failed\n");
					return -1;
				}else{
					ifs->ifsts->next = NULL;
				}
			}
			getToken(this);
			ret += conditionParse(this,ifs->cond);
			if(ret != 0){ 
				logdebug("\n");
				return ret;
			}
			if(curToken->cToken != thensym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(16);
			}
			getToken(this);
			// generate a jump function
			generateConditionalJump(currentProcedure,reg_ptr-1);
			int j1 = getCodeLength(currentProcedure) - 1;

			
			ret += statementParse(this,ifs->ifsts);
			if(ret != 0){ 
				logdebug("\n");
				return ret;
			}
			
			generateJump(currentProcedure);
			int j2 = getCodeLength(currentProcedure) - 1;
			
			changeM(currentProcedure, j1, getCodeLength(currentProcedure));
			
			if(curToken->cToken == elsesym){
				ifs->elsests = calloc(1,sizeof(statementNode_t));
				if(ifs->elsests == NULL){
					logerror("initial statement failed\n");
					return -1;
				}else{
					ifs->elsests->next = NULL;
				}
				getToken(this);
				ret += statementParse(this,ifs->elsests);
				if(ret != 0){ 
					logdebug("\n");
					return ret;
				}
			}
			reg_ptr--;
			changeM(currentProcedure, j2, getCodeLength(currentProcedure));
			sts->super.ifs_p = ifs;
			// update condition
			sts->tag = IF_S;
		}break;
		case whilesym: {
			// "while" condition "do" statement
			whileSNode_t *whileS = calloc(1,sizeof(whileSNode_t));
			if(whileS == NULL){
				logerror("initial while statement failed\n");
				return -1;
			}else{
				whileS->cond = calloc(1,sizeof(conditionNode_t));
				if(whileS->cond == NULL){
					logerror("initial while condition failed\n");
					return -1;
				}else{
					whileS->cond->left = NULL;
					whileS->cond->right = NULL;
				}
				
				whileS->sts= calloc(1,sizeof(statementNode_t));
				if(whileS->sts == NULL){
					logerror("initial statement failed\n");
					return -1;
				}else{
					whileS->sts->next = NULL;
				}
			}
			getToken(this);
			int c = getCodeLength(currentProcedure);
			
			ret += conditionParse(this,whileS->cond);
			if(ret != 0){ 
				logdebug("\n");
				return ret;
			}
			if(curToken->cToken != dosym){				
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(18);
			}
			getToken(this);
			// generate a jump  function
			generateConditionalJump(currentProcedure,reg_ptr-1);
			int j1 = getCodeLength(currentProcedure) - 1;

			
			ret += statementParse(this,whileS->sts);
			if(ret != 0){ 
				logdebug("\n");
				return ret;
			}
			// geerate a jubmp function to return the conditon
			
			generateJump(currentProcedure);
			int j2 = getCodeLength(currentProcedure) - 1;
			
			// update both jump function
			changeM(currentProcedure, j1, getCodeLength(currentProcedure));
			changeM(currentProcedure, j2, c);
			reg_ptr--;//Done with condition result. Free up register.
			sts->super.whiles_p = whileS;
			sts->tag = WHILE_S;
		}break;
		case readsym: {
			//"read"  ident 
			readSNode_t *readS = calloc(1,sizeof(readSNode_t));
			if(readS == NULL){
				logerror("initial read statement failed\n");
				return -1;
			}
			getToken(this);
			if(curToken->cToken != identsym){				
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(27);
			}else{
				readS->ident.string = strdup(curToken->cTokenVal.string);
			}
			//TODO get the symbol from the table and check
			struct Symbol *sym = symTable->get(symTable,readS->ident.string,globalLevel);
			if(sym == NULL){
				throwError(11);
			}else{
				if(sym->kind != VAR_S){
					throwError(12);
				}else{					
					generateRead(currentProcedure,reg_ptr,sym,globalLevel);
				}
			}
			// generate a read function
			sts->super.reads_p = readS;
			sts->tag = READ_S;
			getToken(this);
		}break;
		case writesym: {
			//"write"  ident 
			writeSNode_t *writeS = calloc(1,sizeof(writeSNode_t));
			if(writeS == NULL){
				logerror("initial write a statement failed\n");
				return -1;
			}
			getToken(this);
			if(curToken->cToken != identsym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(28);
			}else{
				writeS->ident.string = strdup(curToken->cTokenVal.string);
			}
			//TODO get the symbol from the table and check
			struct Symbol *sym = symTable->get(symTable,writeS->ident.string,globalLevel);
			if(sym == NULL){
				throwError(11);
			}else{
				if((sym->kind == VAR_S)||(sym->kind == CONST_S)){
					// generate a write function
					generateWrite(currentProcedure,reg_ptr,sym,globalLevel,sym->kind);
				}else {		
					throwError(12);
				}
			}
			sts->super.writes_p = writeS;
			sts->tag = WRITE_S;
			getToken(this);
		}break;
		default: {
			
		}
	}
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	return ret;
}


/*
block = [ "const" ident "=" number {"," ident "=" number} ";"]
            [ "var" ident {"," ident} ";"]
            { "procedure" ident ";" block ";" } statement .

*/
int blockParse(struct Parse *this,struct blockNode *blk){
	int ret = 0;
	CurrentToken_t *curToken = &this->curToken;
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	/*const variable declare*/
	if(curToken->cToken == constsym){
		logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
		 //[ "const" ident "=" number {"," ident "=" number} ";"]
		constDeclNode_t *contP = NULL;
		do{
			constDeclNode_t *cont = calloc(1,sizeof(constDeclNode_t));
			if(cont == NULL){
				logerror("initial const declare failed\n");
				return -1;
			}else{
				cont->next = NULL;
			}
			getToken(this); // get ident
			if(curToken->cToken != identsym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(4);
			}else{
				cont->ident.string = strdup(curToken->cTokenVal.string);
				//TODO make a symbol
			}
			getToken(this); // get =
			if(curToken->cToken != eqlsym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				if(curToken->cToken == becomessym){
					throwError(1);
				}else{
					throwError(3);
				}				
			}
			getToken(this); // get number
			if(curToken->cToken != numbersym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(2);
			}else{
				cont->numer.value = curToken->cTokenVal.numeric;
				cont->lexical = globalLevel;
				cont->paraProc = procedureCount;
				makeSymbol(this,cont->ident.string,CONST_S);
				//TODO update the const value
			}			
			if(blk->numsConstDecl++ == 0){
				blk->constDecl = cont;				
				contP = cont;
			}else{
				contP->next = cont;
				contP = cont;
			}
			getToken(this); // get comma or semiconlon
		}while(curToken->cToken == commasym);
		if (curToken->cToken != semicolonsym){			
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(5);
		}
        getToken(this);
	}

	
	/*varible declare*/
	if(curToken->cToken == varsym){
		logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
		// [ "var" ident {"," ident} ";"]
		varDeclNode_t *varP = NULL;
		do {
			varDeclNode_t *var = calloc(1,sizeof(varDeclNode_t));
			if(var == NULL){
				logerror("initial vardecl node failed\n");
				return -1;
			}else{
				var->next = NULL;
			}
			getToken(this);// get ident
			if(curToken->cToken != identsym){
				logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
				throwError(4);	
			}else{
				var->ident.string = strdup(curToken->cTokenVal.string);
				var->lexical = globalLevel;
				var->paraProc = procedureCount;
				var->addr = blk->numsVarDecl;
				//TODO make a symbol
				makeSymbol(this,var->ident.string,VAR_S);
			}
			// add new node to the list
			if(blk->numsVarDecl++ == 0){
				blk->varDecl = var;
				varP = var;
			}else{
				varP->next = var;
				varP = var;
			}
			
			getToken(this);// get next token, that is comma or semicon
		}while(curToken->cToken == commasym);
		if (curToken->cToken != semicolonsym){
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(5);
		}
        getToken(this);
	}
    int oldProcedure = currentProcedure;
	/*procedure clause*/
	prodefNode_t *procP = NULL;
	while(curToken->cToken == procsym){
		// { "procedure" ident ";" block ";" }
		prodefNode_t *proc = calloc(1,sizeof(prodefNode_t));
		if(proc == NULL){
			logerror("inital proc def failed\n");
			return -1;
		}else{
			proc->next = NULL;
		}
		getToken(this); // get ident, the name of proc
		if(curToken->cToken != identsym){			
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(4);	
		}else{
			proc->ident.string = strdup(curToken->cTokenVal.string);
			proc->lexical = globalLevel;
			proc->paraProc = oldProcedure;
			makeSymbol(this,proc->ident.string,PROC_S);
			proc->addr = procedureCount;
			//TODO make a symbol
		}
		getToken(this);// get next token
		if(curToken->cToken != semicolonsym){
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(6);	
		}
		getToken(this);// get next token
		proc->body = calloc(1,sizeof(blockNode_t));
		if(proc->body == NULL){
			logerror("initial procedue body failed\n");
			return -1;
		}else{
			proc->body->numsConstDecl = 0;
			proc->body->numsProcDef= 0;
			proc->body->numsStatement= 0;
			proc->body->numsVarDecl= 0;
		}
        globalLevel++;//within the block is a new level.
		ret += blockParse(this,proc->body);
        globalLevel--;
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		// add new node to the list
		if(blk->numsProcDef++ == 0){
			blk->procDef = proc;
			procP = proc;
		}else{
			procP->next = proc;
			procP = proc;
		}		
		if (curToken->cToken != semicolonsym){			
			logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
			throwError(6);
		}		
		// TODO generate var space
        generateVariableSpace(currentProcedure, proc->body->numsVarDecl+ 4);
		getToken(this);
	}
    currentProcedure = oldProcedure;
	/*statement clause*/
	//statement
	blk->statement = calloc(1,sizeof(statementNode_t));
	if(blk->statement == NULL){
		logerror("initial a statement failed\n");
		return -1;
	}else{
		ret += statementParse(this,blk->statement);
		if(ret != 0){ 
			logdebug("\n");
			return ret;
		}
		blk->numsStatement++;
	}	
	return ret;
}
/*
 *program ::= block "."
 */
int programParse(struct Parse *this){
	int ret = 0;
	programNode_t *prog = (programNode_t *)calloc(1,sizeof(programNode_t));
	if(prog == NULL){
		logerror("initial root ast tree failed\n");
		return -1;
	}else{
		prog->numsBlock = 0;
		prog->block = (blockNode_t*)calloc(1,sizeof(blockNode_t));
		if(prog->block == NULL){
			free(prog);
			logerror("inital block failed\n");
			return -1;
		}else{
			prog->block->next = NULL;
			prog->block->numsConstDecl = 0;
			prog->block->numsProcDef= 0;
			prog->block->numsStatement= 0;
			prog->block->numsVarDecl= 0;
		}
	}
	setOffsets();
	/*get current token*/
	getToken(this);
	CurrentToken_t *curToken = &this->curToken;
    currentProcedure = getFreeSet();
	logdebug("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
	ret += blockParse(this,prog->block);	
	if(ret != 0){ 
		logdebug("\n");
		return ret;
	}
	if(curToken->cToken != periodsym){
		logerror("The current token is:[%d,%s]\n",curToken->cToken,opSymbol[curToken->cToken]);
		throwError(9);
	}	
	prog->numsBlock++;
	this->ast = prog;
	//TODO generateVariableSpace
    generateVariableSpace(currentProcedure,prog->block->numsVarDecl+ 4);
	
	//TODO generateHalt
    generateHalt(currentProcedure);
	return ret;	
}


int parse(struct Parse *this){
	int ret = 0;
	ret += programParse(this);	
	if(ret != 0){ 
		logdebug("\n");
		return ret;
	}
	return ret;
}
