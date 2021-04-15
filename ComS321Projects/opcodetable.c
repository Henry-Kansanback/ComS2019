#include <stdio.h>
#include <string.h>
#include "opcodetable.h"

int searchTable(int opcode, OpPair *opcodeTable){
  int i;
  for(i = 0; i < 37; i++){
    if(opcodeTable[i].opcode == opcode){
      return i;
    }
  }
  return -1;
}
