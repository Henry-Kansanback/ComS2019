 #ifndef OPCODETABLE_H_
 #define OPCODETABLE_H_

enum EnumOpFormat {
  DEFAULT,
  R,
  I,
  D,
  B,
  CB,
  IW,
  JS
};

enum EnumOpName {
  BRANCH,
  BL,   //End 6 bit opcodes
  BCOND,
  CBZ,
  CBNZ,   //End 8 bit opcodes
  ADDI,
  ANDI,
  ORRI,
  SUBI,
  EORI,
  SUBIS,   //End of 10 bit opcodes
  STURB,
  LDURB,
  STURH,
  LDURH,
  AND,
  ADD,
  SDIV,
  UDIV,   //For now, all division is unsigned, be careful
  MUL,
  SMULH,
  UMULH,
  ORR,
  STURW,
  LDURSW,
  LSR,
  LSL,
  BR,
  EOR,
  SUB,
  SUBS,
  STUR,
  LDUR,
  PRNL,
  PRNT,
  DUMP,
  HALT//End 11 bit opcodes
};


typedef struct OpPair {
  int opcode;
  enum EnumOpName opname;
  enum EnumOpFormat opformat;
} OpPair;

int searchTable(int opcode, OpPair* opcodeTable);

#endif
