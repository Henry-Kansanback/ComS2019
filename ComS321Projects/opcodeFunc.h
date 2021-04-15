#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#ifndef OPCODEFUNC_H_
#define OPCODEFUNC_H_


void add(int des, int reg1, int reg2, u_int64_t *regArr);
void addI(int des, int reg1, int val, u_int64_t *regArr);
void and(int des, int reg1, int reg2, u_int64_t *regArr);
void andI(int des, int reg1, int val, u_int64_t *regArr);
void sub(int des, int reg1, int reg2, u_int64_t *regArr);
void subI(int des, int reg1, int val, u_int64_t *regArr);
int Bcond(int reg, u_int64_t *regArr, unsigned int *condFlag);
void subs(int des, int reg1, int reg2, u_int64_t *regArr, unsigned int *condFlag);
void subis(int des, int reg1, int val, u_int64_t *regArr, unsigned int *condFlag);
void udiv(int des, int reg1, int reg2, u_int64_t *regArr);
void sdiv(int des, int reg1, int reg2, u_int64_t *regArr);
void prnl();
void prnt(int reg, u_int64_t *regArr);
void eor(int des, int reg1, int reg2, u_int64_t *regArr);
void eorI(int des, int reg1, int val, u_int64_t *regArr);
void mul(int des, int reg1, int reg2, u_int64_t *regArr);
void orr(int des, int reg1, int reg2, u_int64_t *regArr);
void orrI(int des, int reg1, int val, u_int64_t *regArr);
void smulh(int des, int reg1, int reg2, u_int64_t *regArr);
void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr);
void ldurB(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr);
void ldurH(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr);
void ldurSW(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr);
void stur(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr);
void sturB(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr);
void sturH(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr);
void sturW(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr);
void lsl(int des, int reg1, int offset, u_int64_t *regArr);
void lsr(int des, int reg1, int offset, u_int64_t *regArr);
void umulh(int des, int reg1, int reg2, u_int64_t *regArr);
void dump(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f);
void halt(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f);
void hexdump(FILE *f, u_int64_t *start, size_t size);
char printable_char(u_int8_t c);
void setConditionals(int des, u_int64_t *regArr, unsigned int *condFlag);

#endif
