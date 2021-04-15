enum _INSTRUCTIONS {
  ADD,
  ADDI,
  AND,
  ANDI,
  B,
  B_cond,
  BL,
  BR,
  CBNZ,
  CBZ,
  DUMP,
  EOR,
  EORI,
  HALT,
  LDUR,
  LDURB,
  LDURH,
  LDURSW,
  LSL,
  LSR,
  MUL,
  ORR,
  ORI,
  PRNL,
  PRNT,
  SDIV,
  SMULH,
  STUR,
  STURB,
  STURH,
  STURW,
  SUB,
  SUBI,
  SUBIS,
  SUBS,
  UDIV,
  UMULH
};

struct InstructionNode {
  enum _INSTRUCTIONS name;
  char parameters[10];
};
