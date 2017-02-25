#include "../include/symtable.h"

/*
 * http://www.cse.yorku.ca/~oz/hash.html
 */
int hashcode(struct SymTable *this,char *key){
	unsigned long hash = 5381;
	int c;

	while (c = *key++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	  return hash%this->numsBucket;
}

boolean _contain(struct SymTable *this,char *key){
	assert(this != NULL);
	assert(key != NULL);
	int hashValue = this->getHashValue(this,key);
	SymTableNode_T * curNode = this->Buckets[hashValue];
	if((curNode != NULL)&&(strcmp(curNode->Key,key) == 0))
		return TRUE;
	else{
		return FALSE;
	}
}

boolean _put(struct SymTable *this,char *key, void *value){
	SymTableNode_T* newNode = NULL;
	
	assert(this != NULL);
	assert(key != NULL);
	assert(value != NULL);

	if(this->numsNode >= this->numsBucket){
		logerror("The buckets is full[%d,%d], please release or expend the buckets\n",
				this->numsNode,this->numsBucket);
		return FALSE;
	}
	
	if(this->contain(this,key)){
		// TODO update the value
		loginfo("The key[%s] exist in the table\n",key);
		return FALSE;
	}

	newNode = (SymTableNode_T*)calloc(1,sizeof(SymTableNode_T));
	if(newNode == NULL){
		logerror("Aplly for memory for a new symbol node failed\n");
		return FALSE;
	}
	
	
	newNode->Key = (char *)calloc(1,strlen(key)+1);
	if(newNode->Key == NULL){
		logerror("Aplly for memory to hold the new key failed\n");
		free(newNode);
		return FALSE;
	}

	strcpy(newNode->Key,key);
	newNode->Value = value;
	newNode->PreNode = newNode;
	newNode->NextNode = newNode;
	
	int hashValue = this->getHashValue(this,key);
	//loginfo("key[%s],value[%p],hash[%d]\n",key,value,hashValue);
	newNode->hashValue = hashValue;
	this->Buckets[hashValue] = newNode;
	
	if(this->numsNode == 0){
		this->head = newNode;
		this->tail = newNode;
	}else{
		this->tail->NextNode = newNode;
		newNode->NextNode = this->head;
		this->head->PreNode = newNode;
		newNode->PreNode = this->tail;
		this->tail = newNode;
	}
	
	this->numsNode++;
	
	return TRUE;
}

void* _getValue(struct SymTable *this,char *key){
	if(this->contain(this,key) == FALSE){
		logerror("The key[%s] does not exist in the table\n",key);
		return NULL;
	}
	int hashValue = this->getHashValue(this,key);
	return this->Buckets[hashValue]->Value;
}


void *_update(struct SymTable *this,char *key,void *value){
	
	if(this->contain(this,key) == FALSE){
		logerror("The key[%s] does not exist in the table\n",key);
		return NULL;
	}
	int hashValue = this->getHashValue(this,key);
	void *oldValue = this->Buckets[hashValue]->Value;
	this->Buckets[hashValue]->Value = value;
	return oldValue;
}


void *_remove(struct SymTable *this,char *key)
{	
	SymTableNode_T *cur = NULL;
	SymTableNode_T *pre = NULL;
	SymTableNode_T *next = NULL;
	void *oldValue = NULL;
	
	if(this->contain(this,key) == FALSE){		
		logerror("The key[%s] does not exist in the table\n",key);
		return oldValue;
	}
		
	int hashValue = this->getHashValue(this,key);
	
	cur = this->Buckets[hashValue];
	pre = cur->PreNode;
	next = cur->NextNode;

	if(cur == this->head)
		this->head = next;
	if(cur == this->tail)
		this->tail = pre;

	oldValue = cur->Value;
	
	pre->NextNode = next;
	next->PreNode = pre;
	cur->PreNode = NULL;
	cur->NextNode = NULL;
	free(cur->Key);
	free(cur);
	this->Buckets[hashValue] = NULL;
	
	this->numsNode--;
	return oldValue;
}


int _getNumsOfsymbol(struct SymTable *this){
	return this->numsNode;
}
int _getNumsOfBuckets(struct SymTable *this){
	return this->numsBucket;
}


void SymTable_clean(struct SymTable *this){

	int i = 0;
	SymTableNode_T *cur = NULL;
	for(i=0;i<this->numsBucket;i++){
		cur = this->Buckets[i];
		if(cur != NULL){
			free(cur->Key);
			free(cur);
			if(cur->Value != NULL){
				free(cur->Value);
			}
		}
	}
	free(this->Buckets);
	free(this);
}

void SymTable_print(struct SymTable *this,int option){
	int i = 0;
	SymTableNode_T *cur = NULL;
	Symbol_t* symbol = NULL;

	loginfo("The Symbol Table\t");
	logpretty("numOfSymbol:%d, numOfBucket:%d\n",this->numsNode,this->numsBucket);
	if(this->numsNode !=0){
		loginfo("head:{key[%s],value[%p]}\ttail{key[%s],value[%p]}\n",
			this->head->Key,this->head->Value,this->tail->Key,this->tail->Value);
	}
	switch (option){
		case 1:{
			// print all buckest
			for(i=0;i<this->numsBucket;i++){
				loginfo("Bucket[%d]\t-",i);
				cur = this->Buckets[i];
				if(cur != NULL){
					logpretty("Symbol{key[%s],value[%p],pre-key[%s],next-key[%s]}",
						cur->Key,cur->Value,cur->PreNode->Key,cur->NextNode->Key);
				}
				logpretty("\n");
			}
		}break;
		case 2:{
			cur = this->head;
			do{
				if(cur != NULL){
					symbol = (Symbol_t*)cur->Value;
					logpretty("Symbol Entry{Hash[%d],Key[%s],kind[%d],value[%d],level[%d],addr[%d]}\n",
							cur->hashValue,symbol->name,symbol->kind,symbol->val,symbol->level,symbol->addr);
					
					cur = cur->NextNode;
				}
			}while(cur != this->head);

		}break;
		case 3:{

		}break;
		default:{
			logerror("the option[%d] error\n",option);
		}
	}
}


struct SymTable *SymTable_init(int bucketCount){

	SymTable_T* symTable = NULL;
	
	symTable = (SymTable_T*)calloc(1,sizeof(SymTable_T));
	if(symTable == NULL){
		logerror("Aplly for memory for a new symbol table failed\n");
		return NULL;
	}
	
	symTable->Buckets = (SymTableNode_T **)calloc(bucketCount,sizeof(SymTableNode_T*));
	if(symTable->Buckets == NULL){
		logerror("Apply for memory for an array of buckets to hold symbol nodes failed\n");
		free(symTable);
		return NULL;
	}
	
	symTable->head = NULL;
	symTable->tail = NULL;
	symTable->numsNode = 0;
	symTable->numsBucket = bucketCount;

	/*init operation*/
	symTable->getHashValue = hashcode;
	symTable->contain = _contain;
	symTable->put = _put;
	symTable->get = _getValue;
	symTable->update = _update;
	symTable->remove = _remove;
	symTable->getNumsOfsymbol = _getNumsOfsymbol;
	symTable->getNumsOfBuckets = _getNumsOfBuckets;
	symTable->clean = SymTable_clean;
	symTable->printinfo = SymTable_print;
	loginfo("The Symbol Table init successfully\n");

	return symTable;
}
