#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "common.h"
#include "scanner.h"


typedef struct SymTableNode
{
  /* Key */
  char *Key;
  /* Value */
  void *Value;
  /*hash Value for the key*/
  int hashValue;
  /* Address of next SymTableNode */
  struct SymTableNode *NextNode;
  struct SymTableNode *PreNode;
}SymTableNode_T;

typedef struct SymTable
{
  /* Number of items in SymTable*/
  int numsNode;

  /* Number of buckets in the table*/
  int numsBucket;

  /*The head symtable node*/
  struct SymTableNode *head;
  /*The tail */
  struct SymTableNode *tail;
  
  /* Address of the first bucket*/
  struct SymTableNode **Buckets;



  /*Operation*/
  void (*clean)(struct SymTable *this);
  int (*getHashValue)(struct SymTable *this,char *key, int lexical);
  boolean (*contain)(struct SymTable *this,char *key,int lexical);
  boolean (*put)(struct SymTable *this,char *key,int lexical,void *value);
  void *(*get)(struct SymTable *this,char *key,int lexical);
  void *(*update)(struct SymTable *this,char *key,int lexical,void *value);
  void *(*remove)(struct SymTable *this,char *key,int lexical);

  int (*getNumsOfsymbol)(struct SymTable *this);
  int (*getNumsOfBuckets)(struct SymTable *this);

  void (*printinfo)(struct SymTable *this,int option);
  void (*outputSymTable)(struct SymTable *this,FILE *out);
}SymTable_T;

extern struct SymTable *SymTable_init(int bucketCount);
#endif
