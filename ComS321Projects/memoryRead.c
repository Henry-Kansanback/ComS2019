#include <stdio.h>
#include <stdint.h>
#include <ctype.h>


int memory[4096]; 
int stack[512];
int reg[32];
boolean cond[14]; 

char printable_char(uint8_t c)
{
  return isprint(c) ? c : '.';
}

void hexdump(FILE *f, int8_t *start, size_t size)
{
  size_t i;

  for (i = 0; i < size - (size % 16); i += 16) {
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
  }
  fprintf(f, "%08x\n", (int32_t) size);
}

void dump(FILE *f)
{
    printf("%s\n", "Registers: ");
    int i;
    for(i= 0; i < 32; i ++)
    {


        if(i == 16)
        {
            printf("(IPO)  X%d: %d\n", i, reg[i]);

        } else if(i == 17)
        {
            printf("(IP1)  X%d: %d\n", i, reg[i]);

        } else if(i == 28)
        {
            printf(" (SP)  X%d: %d\n", i, reg[i]);
        } else if(i == 29)
        {
            printf(" (FP)  X%d: %d\n", i, reg[i]);
        } else if(i == 30)
        {
            printf(" (LR)  X%d: %d\n", i, reg[i]);
        } else if(i == 31)
        {
            printf("(XZR)  X%d: %d\n", i, reg[i]);
        }
        else if(i < 10)
        {
            printf("       X%d:  %d\n", i, reg[i]);
        } else
        {
            printf("       X%d: %d\n", i, reg[i]);
        }
    }
    PRNL();
    PRNL();
    printf("%s\n", "Stack: ");
    hexdump(f, &stack[0], 512);
    PRNL();
    PRNL();
    PRNL();
    PRNL();
    printf("%s\n", "Main Memory: ");
    hexdump(f, &memory[0], 4096)
}
//Call the dump function and terminate the emulator
void Halt()
{
    dump();
    exit(0);
}
void ADD(int *reg3, int *reg1, int *reg2)
{
    *reg3 = *reg1 + *reg2;
}

void ADDI(int *reg3, int *reg1, int i)
{
    *reg3 = *reg1 + i;
}

void AND(int *reg3, int *reg1, int *reg2)
{
    *reg3 = *reg1 & *reg2;
}

void ANDI(int *reg2, int *reg1, int i)
{
    *reg2 = *reg1 & i;
}

void LDUR(int *reg3, int *reg1, int memoryArray[], int i)
{
    int offsetVal = i / 8;
    *reg3 = memoryArray[offsetVal + *reg1]; 
}


void LDURB(int *reg3, int *reg1, int memoryArray[], int i)
{
    int offsetVal= i / 8;
    *reg3 = memoryArray[val + *reg1] & 0x1; //????
}


void LDURH(int *reg2, int *reg1, int memoryArray[], int i)
{
    int offsetVal = i / 8; 
    *reg2 = memoryArray[offsetVal + *reg1] & 0xFFFF;
}


void LDURSW(int *reg2, int *reg1, int memoryArray[], int i)
{
    int val = i / 8; 
    *reg2 = memoryArray[val + *reg1] & 0xFFFFFFFF; 
}

void LSL(int *reg2, int *reg1, int i)
{
  *reg2 = *reg1 << i;
}

void LSR(int *reg2, int *reg1, int i)
{
    *reg2 = *reg1 >> i;
}

void MUL(int *reg3, int *reg2, int *reg1)
{
    *reg3 = (*reg2) * (*reg1);
}

void ORR(int *reg3, int *reg2, int *reg1)
{
    *reg3 = *reg2 | *reg1;
}

void ORRI(int *reg2, int *reg1, int i)
{
    *reg2 = *reg1 | i;
}
void EOR(int *reg3, int *reg1, int *reg2)
{
    *reg3 = *reg1 ^ *reg2;
}

void EORI(int *reg2, int *reg1, int i)
{
    *reg2 = *reg1 ^ i;
}

void PRNL()
{
    printf("\n");   
}

void PRINT(int *reg)
{
    printf("DecimalValue: %d\nHex: %x\n", *reg, *reg);
}


void SDIV(int *reg3, int *reg1, int *reg2) //??
{
    *reg3 = *reg1 / *reg2;
}

void SMULH(int *reg3, int *reg2, int *reg1) //???
{
    *reg3 = (*reg2) * (*reg1);
}
void UMULH(int *reg3, int *reg1, int *reg2) 
{
    *reg3 = (*reg1) * (*reg2);
}

void STUR(int *reg1, int memoryArray[], int *reg, int i)
{
    int offesetVal = i / 8;
    memoryArray[*reg + offesetVal] = *reg1;
}


void STRUB(int *reg3, int memoryArray[], int *reg, int i)
{
    int offsetVal = i / 8;
    memoryArray[*reg + offsetVal] = *reg3 & 0x1;
}


void STRUH(int *des, int memoryArray[], int *reg, int i) 
{
    int val = i / 8;
    memoryArray[*reg + val] = *des & 0xFFFF;
}


void STURW(int *reg3, int memory[], int *reg, int i)
{
    int val = offset / 8;
    memory[*reg + val] = *reg3 & 0xFFFFFFFF;
}


void SUB(int *reg3, int *reg1, int *reg2)
{
    *reg3 = *reg1 - *reg2;
}
void SUBI(int *reg2, int *reg1, int i)
{

    *reg2 = *reg1 - i;
}
void SUBIS(int *reg3, int *reg2, int *reg1){
    *reg3 = *reg1 - *reg2;
    if(*reg3==0){
        cond[0]==true;
    }
    else if(*reg3 >=0){ 
        cond[1] = true; //do the brunch
        

    }
    else if(*reg3 >0){ 
//TODO
        
    }
      else if(*reg3 <=0){ 
//TODO
        
    }
      else if(*reg3< 0){ 
//TODO
        
    }
        else if(*reg3 != 0){ 
//TODO
    
    }


}

void UDIV(int *reg3, int *reg1, int *reg2) 
{
    *reg3 = *reg1 / *reg2;
}






