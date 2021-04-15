#include <stdio.h>
#include <string.h>
#include "hashtable.h"

int hashCode(char* key){
  return key % SIZE;
}

OpPair *search(char* key){
  int hashIndex = hashCode(key);

  while(hashArray[hashIndex] != NULL){
    if(hashArray[hashIndex]->opcode == key)
      return hashArray[hashIndex];

      ++hashIndex;

      hashIndex %= SIZE
  }

  return NULL;
}

void insert(char* key, char* data){
  OpPair *op = (OpPair*) malloc(sizeof(OpPair));
  op->opname = data;
  op->opcode = key;

  int hashIndex = hashCode(key);

  while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->opcode != -1){
    ++hashIndex;

    hashIndex %= SIZE;
  }

  hashArray[hashIndex] = op;
}

OpPair* delete(OpPair* op){
  char *key = op->opcode;

  int hashIndex = hashCode(key);

  while(hashArray[hashIndex] != NULL){
    if(hashArray[hashIndex]->opcode == key){
      OpPair* temp = hashArray[hashIndex];

      hashArray[hashIndex] = dummyItem;
      return temp;
    }
    ++hashIndex;
    hashIndex %= SIZE;
  }
  return NULL;
}

void display(){
  int i = 0;

  for(i = 0; i < SIZE; i++){
    if(hashArray[i] != NULL)
      printf(" (%s, %s)", hashArray[i]->opcode, hashArray[i]->opname);
    else
      printf(" ~~ ");
  }
  printf("\n");
}

/*
int main() {
  dummyItem = (OpPair*) malloc(sizeof(OpPair));
  dummyItem->opname = "-1";
  dummyItem->opcode = "-1";

  insert("101", "ADD");
  insert("000", "SUB");
  insert("001", "AND");

  display();
  item = search("101");

  if(item != NULL){
    printf("Element found: %s\n", item->opname);
  } else{
    printf("Element not found\n");
  }

  delete(item);
}
*/
