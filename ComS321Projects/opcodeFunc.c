#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "opcodeFunc.h"
//uint32_t??
u_int8_t memory[4096]; //4096? //1025
u_int8_t stack[512]; //512?? //129
u_int8_t reg[32];

//PC is register 16??? 15??
//SP & FP registers should be initialized to size of stack
/*
void add(int *des, int *reg1, int *reg2); //adds two registers to a destination register
void addI(int *des, int *reg1, int val); //adds a value to a register
void and(int *des, int *reg1, int *reg2);
void andI(int *des, int *reg1, int val);
// FILE *b(int val, *char file, int *pcReg); //32 * number of lines you want to skip
void sub(int *des, int *reg1, int *reg2);
void subI(int *des, int *reg1, int val);
void udiv(int *des, int *reg1, int *reg2);
void sdiv(int *des, int *reg1, int *reg2);
void prnl();
void prnt(int *reg);
// FILE *bl(int val, *char file, int *pcReg, int *blReg);
// FILE *br(*char file, int *reg, int *pcReg);
// FILE *cbz(*char file, int *reg, int val, int *pcReg);
void eor(int *des, int *reg1, int *reg2);
void eorI(int *des, int *reg1, int val);
void mul(int *des, int *reg1, int *reg2);
void orr(int *des, int *reg1, int *reg2);
void orrI(int *des, int *reg1, int val);
void smulh(int *des, int *reg1, int *reg2); //????
void ldur(int *des, int *reg1, u_int64_t *memory, int offset);
void ldurB(int *des, int *reg1, u_int64_t *memory, int offset);
void ldurH(int *des, int *reg1, u_int64_t *memory, int offset);
void ldurSW(int *des, int *reg1, u_int64_t *memory, int offset);
void stur(int *des, u_int64_t *memory, int *reg, int offset);
void sturB(int *des, u_int64_t *memory, int *reg, int offset);
void sturH(int *des, u_int64_t *memory, int *reg, int offset);
void sturW(int *des, u_int64_t *memory, int *reg, int offset);
void lsl(int *des, int *reg1, int offset);
void lsr(int *des, int *reg1, int offset);
void umulh(int *des, int *reg1, int *reg2); //???????
void hexdump(FILE *f, int8_t *start, size_t size);
void dump(FILE *f);
char printable_char(uint8_t c);
*/

// int main()
// {
//
//   for(int i = 0; i < 32; i ++)
//   {
//     reg[i] = 0;
//   }
//
//   memory[0] = 10;
//   memory[1] = 12;
//   memory[2] = 46;
//   reg[28] = 512;
//   reg[29] = 512;
//
//   //stur(&reg[0], memory[], &reg[1], 24);
//   //smulh(&reg[1], &reg[2], &reg[3]);
//   //dump(&memory[0], 4096);
//   dump();
//   return 0;
// }


void add(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = *reg1 + *reg2;
    regArr[des] = regArr[reg1] + regArr[reg2];
}

void addI(int des, int reg1, int val, u_int64_t *regArr)
{
    //*des = *reg1 + val;
    regArr[des] = regArr[reg1] + val;
}

void and(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = *reg1 & *reg2;
    regArr[des] = regArr[reg1] & regArr[reg2];
}

void andI(int des, int reg1, int val, u_int64_t *regArr)
{
    //*des = *reg1 & val;
    regArr[des] = regArr[reg1] & val;
}


int Bcond(int reg, u_int64_t *regArr, unsigned int *condFlag) {
  if(condFlag[reg] == 1) {
    return 1;
  }

  return 0;
}

//DO DUMP
char printable_char(uint8_t c)
{
  return isprint(c) ? c : '.';
}

void hexdump(FILE *f, uint64_t *start, size_t size) //displays contents of registers, memory, and disassembled program
{
  size_t i;

  for (i = 0; i < size - (size % 2); i += 2)
  //for(i = 0; i < size; i++)
  {
	  /*
     fprintf(f,
             "%08x "
             " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
             " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
             " |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
             (int32_t) i,
             start[i +  0], start[i +  1], start[i +  2], start[i +  3],
             start[i +  4], start[i +  5], start[i +  6], start[i +  7],
             start[i +  8], start[i +  9], start[i + 10], start[i + 11],
             start[i + 12], start[i + 13], start[i + 14], start[i + 15],
             printable_char(start[i +  0]), printable_char(start[i +  1]),
             printable_char(start[i +  2]), printable_char(start[i +  3]),
             printable_char(start[i +  4]), printable_char(start[i +  5]),
             printable_char(start[i +  6]), printable_char(start[i +  7]),
             printable_char(start[i +  8]), printable_char(start[i +  9]),
             printable_char(start[i + 10]), printable_char(start[i + 11]),
             printable_char(start[i + 12]), printable_char(start[i + 13]),
             printable_char(start[i + 14]), printable_char(start[i + 15]));

    */
    printf(
                    "%08x "
                    //" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
                    //" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
                    "%016x %016x \n",
                    //" |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
                    (int32_t) i*8,
                    start[i], start[i+1]);
                    /*(start[i]>>56)&0xf, (start[i]>>48)&0xf, (start[i]>>40)&0xf, (start[i]>>32)&0xf,
                    (start[i]>>24)&0xf, (start[i]>>16)&0xf, (start[i]>>8)&0xf, (start[i])&0xf,
                    (start[i + 1]>>56)&0xf, (start[i + 1]>>48)&0xf, (start[i + 1]>>40)&0xf, (start[i + 1]>>32)&0xf,
                    (start[i + 1]>>24)&0xf, (start[i + 1]>>16)&0xf, (start[i + 1]>>8)&0xf, (start[i + 1])&0xf,
                    printable_char((start[i]>>56)&0xf), printable_char((start[i]>>48)&0xf),
                    printable_char((start[i]>>40)&0xf), printable_char((start[i]>>32)&0xf),
                    printable_char((start[i]>>24)&0xf), printable_char((start[i]>>16)&0xf),
                    printable_char((start[i]>>8)&0xf), printable_char((start[i])&0xf),
                    printable_char((start[i + 1]>>56)&0xf), printable_char((start[i + 1]>>48)&0xf),
                    printable_char((start[i + 1]>>40)&0xf), printable_char((start[i + 1]>>32)&0xf),
                    printable_char((start[i + 1]>>24)&0xf), printable_char((start[i + 1]>>16)&0xf),
                    printable_char((start[i + 1]>>8)&0xf), printable_char((start[i + 1])&0xf));*/
  }
  //printf("Mem at index %d is %d\n", i, start[i]);
  //fprintf(f, "%08x\n", (int32_t) size);
  printf("%08x\n", (int32_t) size*16);
}

void dump(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f)
{
    printf("%s\n", "Registers: ");
    for(int i = 0; i < 32; i ++)
    {


        if(i == 16)
        {
            printf("(IPO)  X%d: %d\n", i, regArr[i]);

        } else if(i == 17)
        {
            printf("(IP1)  X%d: %d\n", i, regArr[i]);

        } else if(i == 28)
        {
            printf(" (SP)  X%d: %d\n", i, regArr[i]);
        } else if(i == 29)
        {
            printf(" (FP)  X%d: %d\n", i, regArr[i]);
        } else if(i == 30)
        {
            printf(" (LR)  X%d: %d\n", i, regArr[i]);
        } else if(i == 31)
        {
            printf("(XZR)  X%d: %d\n", i, regArr[i]);
        }
        else if(i < 10)
        {
            printf("       X%d:  %d\n", i, regArr[i]);
        } else
        {
            printf("       X%d: %d\n", i, regArr[i]);
        }
    }
    prnl();
    prnl();
    //SP & FP - initialized to the size of the stack
    printf("%s\n", "Stack: ");
    hexdump(f, stack, 64);
    prnl();
    prnl();
    printf("%s\n", "Main Memory: ");
    hexdump(f, memory, 512);
}

void eor(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = ~(*reg1 & *reg2) & ~(~(*reg1) & ~(*reg2));
    //*des = *reg1 ^ *reg2;
    regArr[des] = regArr[reg1] ^ regArr[reg2];
}

void eorI(int des, int reg1, int val, u_int64_t *regArr)
{
    //*des = ~(*reg1 & val) & ~(~(*reg1) & ~(val));
    //*des = *reg1 ^ val;
    regArr[des] = regArr[reg1] ^ val;
}


void halt(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f)
{
    dump(regArr, memory, stack, f);
    exit(0); //terminates
}

void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    //*des = memory[val + *reg1]; //loads into des register from the memory array with specified value
    regArr[des] = memory[val + regArr[reg1]];
}


void ldurB(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    //*des = memory[val + *reg1] & 0x1; //????
    regArr[des] = memory[val + regArr[reg1]] & 0x1;
}


void ldurH(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    //*des = memory[val + *reg1] & 0xFFFF; //half word representation
    regArr[des] = memory[val + regArr[reg1]] & 0xFFFF;
}


void ldurSW(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    //*des = memory[val + *reg1] & 0xFFFFFFFF; //in theory, word representation, if a half word --> 0xFFFF a word --> 0xFFFFFFFF
    regArr[des] = memory[val + reg1] & 0xFFFFFFFF;
}

void lsl(int des, int reg1, int offset, u_int64_t *regArr)
{
  // does the offset need to be divded by 8??
  //int val = offset / 8;
  //*des = *reg1 << offset;
  regArr[des] = regArr[reg1] << offset;
}

void lsr(int des, int reg1, int offset, u_int64_t *regArr)
{
    //*des = *reg1 >> offset;
    regArr[des] = regArr[reg1] >> offset;
}

void mul(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = (*reg1) * (*reg2);
    regArr[des] = regArr[reg1]*regArr[reg2];
}

void orr(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = *reg1 | *reg2;
    regArr[des] = regArr[reg1] | regArr[reg2];
}

void orrI(int des, int reg1, int val, u_int64_t *regArr)
{
    //*des = *reg1 | val;
    regArr[des] = regArr[reg1] | val;
}

void prnl()
{
    printf("\n");
}

void prnt(int reg, u_int64_t *regArr)
{
    printf("Register: %d \nDecimal: %d \nHex: %x \n", reg, regArr[reg], regArr[reg]);
}


void sdiv(int des, int reg1, int reg2, u_int64_t *regArr) //??
{
    //*des = *reg1 / *reg2;
    regArr[des] = regArr[reg1] / regArr[reg2];
}

//DO SMULH
void smulh(int des, int reg1, int reg2, u_int64_t *regArr) //???
{
    //*des = (*reg1) * (*reg2);
    regArr[des] = regArr[reg1] * regArr[reg2];
}


void stur(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr)
{
    int val = offset / 8;
    //memory[*reg + val] = *des;
    //memory[regArr[reg] + val] = regArr[des];
    memory[regArr[reg]] = regArr[des];
}


void sturB(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr)
{
    int val = offset / 8;
    //memory[*reg + val] = *des & 0x1;
    memory[regArr[reg] + val] = regArr[des] & 0x1;
}


void sturH(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr)
{
    int val = offset / 8;
    //memory[*reg + val] = *des & 0xFFFF;
    memory[regArr[reg] + val] = regArr[des] & 0xFFFF;
}


void sturW(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr)
{
    int val = offset / 8;
    //memory[*reg + val] = *des & 0xFFFFFFFF;
    memory[regArr[reg] + val] = regArr[des] & 0xFFFFFFFF;
}


void sub(int des, int reg1, int reg2, u_int64_t *regArr)
{
    //*des = *reg1 - *reg2;
    regArr[des] = regArr[reg1] - regArr[reg2];
}



void subI(int des, int reg1, int val, u_int64_t *regArr)
{
    //*des = *reg1 - val;
    regArr[des] = regArr[reg1] - val;
}



//TODO? SUBIS - Flag
void subis(int des, int reg1, int val, u_int64_t *regArr, unsigned int *condFlag)
{
  regArr[des] = regArr[reg1] - val;
  setConditionals(des, regArr, condFlag);
}

//TODO? SUBS - Flag
void subs(int des, int reg1, int reg2, u_int64_t *regArr, unsigned int *condFlag)
{
  regArr[des] = regArr[reg1] - regArr[reg2];
  setConditionals(des, regArr, condFlag);
}


void udiv(int des, int reg1, int reg2, u_int64_t *regArr) //??
{
    //*des = *reg1 / *reg2;
    regArr[des] = regArr[reg1]/regArr[reg2];
}

//DO UMULH
void umulh(int des, int reg1, int reg2, u_int64_t *regArr) //???
{
    //*des = (*reg1) * (*reg2);
    regArr[des] = regArr[reg1] * regArr[reg2];
}

void setConditionals(int des, u_int64_t *regArr, unsigned int *condFlag) {
  for(int i = 0; i < 16; i++) {
      switch(i) {
        //EQ
        case 0:
          if(regArr[des] == 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //NE
        case 1:
          if(regArr[des] != 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //HS
        case 2:
          if((int64_t)regArr[des] >= 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //LO
        case 3:
          if((int64_t)regArr[des] < 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //MI
        case 4:
          if((int64_t)regArr[des] < 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //PL
        case 5:
          if((int64_t)regArr[des] >= 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //VS
        case 6:
          condFlag[i] = 0;
        break;

        //VC
        case 7:
          condFlag[i] = 0;
        break;

        //HI
        case 8:
          if((int64_t)regArr[des] > 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //LS
        case 9:
          if((int64_t)regArr[des] <= 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //GE
        case 10:
          if((int64_t)regArr[des] >= 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //LT
        case 11:
          if((int64_t)regArr[des] < 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //GT
        case 12:
          if((int64_t)regArr[des] > 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        //LE
        case 13:
          if((int64_t)regArr[des] <= 0) {
            condFlag[i] = 1;
          } else {
            condFlag[i] = 0;
          }
        break;

        case 14:
        break;

        case 15:
        break;

        default:
        break;
      }
    }
}
