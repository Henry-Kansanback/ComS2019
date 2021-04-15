#include <stdio.h>
#include <stdlib.h>
//#include "hashtable.h"
#include "opcodetable.h"
//#include "opcodetable.c"
#include "readFile.h"
//#include "decoderStructure.h"
#include "opcodeFunc.h"

/*
1 = B
2 = R
3 = I
4 = D
5 = CB
6 = IW
7 = JS
*/

OpPair opcodeTable[] = {
   {5, BRANCH, B},
   {37, BL, B}, //End 6 bit opcodes
   {84, BCOND, CB},
   {180, CBZ, CB},
   {181, CBNZ, CB}, //End 8 bit opcodes
   {580, ADDI, I},
   {584, ANDI, I},
   {712, ORRI, I},
   {836, SUBI, I},
   {840, EORI, I},
   {964, SUBIS, I}, //End of 10 bit opcodes
   {448, STURB, D},
   {450, LDURB, D},
   {960, STURH, D},
   {962, LDURH, D},
   {1104, AND, R},
   {1112, ADD, R},
   {1238, SDIV, R},
   {1238, UDIV, R}, //For now, all division is unsigned, be careful
   {1240, MUL, R},
   {1242, SMULH, R},
   {1246, UMULH, R},
   {1360, ORR, R},
   {1472, STURW, D},
   {1476, LDURSW, D},
   {1690, LSR, R},
   {1691, LSL, R},
   {1712, BR, R},
   {1616, EOR, R},
   {1624, SUB, R},
   {1880, SUBS, R},
   {1984, STUR, D},
   {1986, LDUR, D},
   {2044, PRNL, JS},
   {1021, PRNT, R},
   {2046, DUMP, JS},
   {2047, HALT, JS} //End 11 bit opcodes
};

struct Data {
  int rm, rn, rd, imm, dtaddr, braddr, condbraddr, instructionShift, shamt;
};

#define MAX_INSTRUCTION_SIZE 10000000

struct Data instructionData[MAX_INSTRUCTION_SIZE];

int instructionArray[MAX_INSTRUCTION_SIZE];
u_int64_t regArr[32];
u_int64_t memory[512];
u_int64_t stack[64];
int counter = 0;
int extraCycles = 0;
int dataHazardCounter = 0;
int controlHazardCounter = 0;

FILE *file;

void decode(int a, int i) {

  int shiftAmount = 6;
  int shift = 32 - shiftAmount;
  int breakout = 0;
  int foundOpcode = 0;
  int opcodeIndex = -1;
  int mask = 0x7FF;

  //TODO compare and find the opcode that is given in a
  while(shiftAmount > 0) {

    //printf("%d\n", a>>shift);

    //search for opcode
    //opcodeIndex = searchTable(a>>shift, opcodeTable);

    switch(shiftAmount) {
      case 6:
        mask = 0x3F;
      break;

      case 8:
        mask = 0xFF;
      break;

      case 9:
        mask = 0x1FF;
      break;

      case 10:
        mask = 0x3FF;
      break;

      case 11:
        mask = 0x7FF;
      break;
    }

    opcodeIndex = searchTable(a>>shift & mask, opcodeTable);

    if(opcodeIndex >= 0){
      //printf("shiftAmount: %d -- %d -- %d", shiftAmount, a>>shift & mask, opcodeTable[opcodeIndex].opname);
      instructionData[i].instructionShift = shift;
      switch(opcodeTable[opcodeIndex].opformat) {
	case R:
    instructionData[i].rd = a & 0x1F;
    instructionData[i].rn = a>>5 & 0x1F;
    instructionData[i].shamt = a>>10 & 0x3f;
    instructionData[i].rm = a>>16 & 0x1f;
	break;

	case I:
    instructionData[i].rd = a & 0x1F;
    instructionData[i].rn = a>>5 & 0x1F;
    instructionData[i].imm = a>>10 & 0xFFF;
	break;

	case D:
    instructionData[i].rd = a & 0x1F;
    instructionData[i].rn = a>>5 & 0x1F;
    instructionData[i].dtaddr = a>>12 & 0x7FF;
	break;

	case B:
    instructionData[i].braddr = a & 0x3FFFFFF;
	break;

	case CB:
    instructionData[i].rd = a & 0x1F;
    instructionData[i].condbraddr = a>>5 & 0x7FFFF;
	break;

	case IW:
	break;

	case JS:
	break;

	case DEFAULT:
	break;
      }

      foundOpcode = 1;
      breakout = 1;
    }

    //update shiftAmount
    switch(shiftAmount) {
      case 11:
        shiftAmount = -1;
      break;

      case 10:
        shiftAmount = 11;
      break;

      case 9:
        shiftAmount = 10;
      break;

      case 8:
        shiftAmount = 9;
      break;

      case 6:
        shiftAmount = 8;
      break;
    }

    //exit while loop
    if(breakout == 1) {
      break;
    }

    //update shift
    shift = 32 - shiftAmount;
  }

  if(foundOpcode == 0) {
  }

}

void functionCaller() {

  unsigned int condFlag[16];
  
  enum condFlagEnum {
    EQ,
    NE,
    HS,
    LO,
    MI,
    PL,
    VS,
    VC,
    HI,
    LS,
    GE,
    LT,
    GT,
    LE
  };

  int i;
  unsigned int a;
  for(i = 0; i < counter; i++) {
    a = instructionArray[i];
    switch(a>>instructionData[i].instructionShift) {

      //BRANCH
      case 5:
        i += instructionData[i].braddr - 1;
      break;

      //BL
      case 37:
        //set BR register to have our current address
        regArr[30] = (u_int64_t)i;

        //branch
        i += instructionData[i].braddr - 1;
      break;

      //BCOND
      case 84:
        if(Bcond(instructionData[i].rd, regArr, condFlag)) {
          i += instructionData[i].condbraddr - 1;
        }
      break;

      //CBZ
      case 180:
        if(instructionData[i].rd == 0) {
          i += instructionData[i].condbraddr - 1;
        }
      break;

      //CBNZ
      case 181:
        if(instructionData[i].rd != 0) {
          i += instructionData[i].condbraddr - 1;
        }
      break;

      //ADDI
      case 580:
        addI(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr);
      break;

      //ANDI
      case 584:
        andI(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr);
      break;

      //ORRI
      case 712:
        orrI(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr);
      break;

      //SUBI
      case 836:
        subI(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr);
      break;

      //EORI
      case 840:
        eorI(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr);
      break;

      //SUBIS
      case 964:
        subis(instructionData[i].rd, instructionData[i].rn, instructionData[i].imm, regArr, condFlag);
      break;

      //STURB
      case 448:
        //this is very questionable
        sturB(instructionData[i].rd, memory, instructionData[i].rn, instructionData[i].dtaddr, regArr);
      break;

      //LDURB
      case 450:
        ldurB(instructionData[i].rd, instructionData[i].rn, memory, instructionData[i].dtaddr, regArr);
      break;

      //STURH
      case 960:
        sturH(instructionData[i].rd, memory, instructionData[i].rn, instructionData[i].dtaddr, regArr);
      break;

      //LDURH
      case 962:
        ldurH(instructionData[i].rd, instructionData[i].rn, memory, instructionData[i].dtaddr, regArr);
      break;

      //AND
      case 1104:
        and(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //ADD
      case 1112:
        add(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //UDIV (also SDIV) note: all div. is unsigned
      case 1238:
        udiv(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //MUL
      case 1240:
        mul(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //SMULH
      case 1242:
        smulh(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //UMULH
      case 1246:
        umulh(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //ORR
      case 1360:
        orr(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //STURW
      case 1472:
        sturW(instructionData[i].rd, memory, instructionData[i].rn, instructionData[i].dtaddr, regArr);
      break;

      //LDURSW
      case 1476:
        ldurSW(instructionData[i].rd, instructionData[i].rn, memory, instructionData[i].dtaddr, regArr);
      break;

      //LSR
      case 1690:
        lsr(instructionData[i].rd, instructionData[i].rn, instructionData[i].shamt, regArr);
      break;

      //LSL
      case 1691:
        lsl(instructionData[i].rd, instructionData[i].rn, instructionData[i].shamt, regArr);
      break;

      //BR
      case 1712:
        i += instructionData[i].rd - 1;
      break;

      //EOR
      case 1616:
        eor(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
      break;

      //SUB
      case 1624:
        sub(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);

      break;

      //SUBS
      case 1880:
        subs(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr, condFlag);
      break;

      //STUR
      case 1984:
        stur(instructionData[i].rd, memory, instructionData[i].rn, instructionData[i].dtaddr, regArr);
      break;

      //LDUR
      case 1986:
        ldur(instructionData[i].rd, instructionData[i].rn, memory, instructionData[i].dtaddr, regArr);
      break;

      //PRNL
      case 2044:
        prnl();
      break;

      //PRNT
      case 1021:
        prnt(instructionData[i].rd, regArr);
      break;

      //DUMP
      case 2046:
        //TODO
        //how would it access memory & stack to dump??
        dump(regArr, memory, stack, file);
      break;

      //HALT
      case 2047:
        halt(regArr, memory, stack, file);
      break;

    }
  }
}


int main(int argc, char const *argv[])
{

  //set local vars.
  int mainMemorySize = 4096;
  int stackSize = 512;

  //TODO???????
  //u_int8_t stack[512];


  //parse command line arguments if there are two or more arguments
  if(argc >= 2) {
    //printf("The arguments supplied are:\n");

    //print out all arguments
    /*
    for(int i = 1; i < argc; i++) {
        printf("%s\t ", argv[i]);
    }
    */

    //printf("\n");

    //if atleast three arguments set mainMemorySize
    if(argc >= 3) {
      mainMemorySize = atoi(argv[2]);
      //printf("mainMemorySize = %d\n", mainMemorySize);
    }

    //if atleas four arguments set stackSize
    if(argc >= 4) {
      stackSize = atoi(argv[3]);
      //printf("stackSize = %d\n", stackSize);
    }

    //if there is not two or more arguments then print the input format
  } else {
      printf("<.machine file> <main memory size> <stack size>\n");
      return 0;
  }

  //read from file and put all instructions into instructionArray
  counter = readFile(argc, argv, instructionArray, file);

  //for visual purposes print out the instructions in hex
  /*
  for(int i = 0; i < counter; i++) {
    printf("%x\n", instructionArray[i]);
  }


  printf("-----\n");

  */
  //TODO compare opcode by taking the instruction and shifting right till you have just the beggining and compare it to the decimal version of the op code
  
  for (int x = 0; x < counter; x++) {
    instructionData[x].rm = -1;
    instructionData[x].rn = -1;
    instructionData[x].rd = -1;
    instructionData[x].imm = -1;
    instructionData[x].dtaddr = -1;
    instructionData[x].braddr = -1;
    instructionData[x].condbraddr= -1;
    instructionData[x].instructionShift = -1;
    instructionData[x].shamt = -1;
  }
  
  for(int i = 0; i < counter; i++) {
    //unsigned int a = instructionArray[i];
    int a = instructionArray[i];
    decode(a, i);
  }
  
  functionCaller();

  int c = 0;

  for(int i = 0; i < counter; i++) {
    c = i;
    while(c < i+4 && c < counter - 1) {
      c++;
      if(instructionData[i].rd != -1) {
        if(instructionData[i].rd == instructionData[c].rm || 
        instructionData[i].rd == instructionData[c].rn || 
        instructionData[i].rd == instructionData[c].rd) {
          dataHazardCounter++;
          switch(c-i){
            case 1:
              extraCycles += 4;
            break;

            case 2:
              extraCycles += 3;
            break;

            case 3:
              extraCycles += 2;
            break;

            case 4:
              extraCycles += 1;
            break;
          }
        }
      }

    }

    c = i;
    if(instructionArray[i]>>instructionData[i].instructionShift == 180 ||
       instructionArray[i]>>instructionData[i].instructionShift == 181 ||
       instructionArray[i]>>instructionData[i].instructionShift == 1712  ) {

      while(c > i-4 && c > 1) {
        c--;
        if(instructionData[i].rd != -1) {
          if(instructionData[i].rd == instructionData[c].rd || instructionData[i].rd == instructionData[c].rd) {
            controlHazardCounter++;
            switch(i-c){
              case 1:
                extraCycles += 4;
              break;

              case 2:
                extraCycles += 3;
              break;

              case 3:
                extraCycles += 2;
              break;

              case 4:
                extraCycles += 1;
              break;
            }
          }
        }
      }
    }

  }

  //Print Stats:
  printf("The number of cycles unpipelined is = %d\n", counter * 5);
  //printf("The number of cycles in a perfect 5-stage pipeline is = %d\n", counter - 1 + 5);
  printf("The number of cycles in a 5-stage pipeline is = %d\n", extraCycles + counter - 1 + 5);
  printf("The number of data hazards is = %d\n", dataHazardCounter);
  printf("The number of control hazards is = %d\n", controlHazardCounter);
  printf("The number of cycles with bypassing but no branch prediction = %d\n", (counter - 1 + 5) );


  return 0;
};
