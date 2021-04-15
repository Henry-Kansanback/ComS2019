#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#define SIZE 40

typedef struct OpPair {
  char* opcode;
  char* opname;
} OpPair;

OpPair* hashArray[SIZE];
OpPair* dummyOp;
OpPair* op;

int hashCode(char* key);

OpPair *search(int key);

void insert(char* key, char* data);

OpPair* delete(OpPair op);

void display();

#endif // HASHTABLE_H_
