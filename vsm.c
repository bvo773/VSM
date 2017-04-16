/*  
  * Program: vsm.c
  * Purpose: Stimulate the VSM
  * Author: Binh Vo
  * Date: 10/2/16
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WORD_SIZE 2              //WORD_SIZE = 16 bits = 2 bytes = an instruction
#define MEM_SIZE 2048            //Total size (bytes) of the VSM memory system, including CODE & DATA

#define OP_EOC  0
#define OP_LOAD 1
#define OP_STORE 2
#define OP_READ 3
#define OP_WRITE 4
#define OP_ADD 5
#define OP_SUB 6
#define OP_MUL 7
#define OP_DIV 8
#define OP_JUMP 9
#define OP_JNEG 10
#define OP_JZERO 11
#define OP_HALT 12

// --------------------------------------
// FUNCTION DECLARATIONS
// --------------------------------------
void scanMem(unsigned short *pvsmMem, unsigned short *plengthScan);
unsigned short opCodeGen(unsigned short hex);
unsigned short middleBit(unsigned short hex);
unsigned short operandGen(unsigned short hex);

int opLoad(unsigned short middleBit, unsigned short operand);
int opStore(unsigned short operand);
int opRead(unsigned short operand);
int opWrite(unsigned short operand);
int opAdd(unsigned short middleBit, unsigned short operand);
int opSub(unsigned short middleBit, unsigned short operand);
int opMul(unsigned short middleBit, unsigned short operand);
int opDiv(unsigned short middleBit, unsigned short operand);
int opJump(unsigned short operand);
int opJneg(unsigned short operand);
int opJzero(unsigned short operand);
void computerDump(unsigned short opCode, unsigned short operand);

// --------------------------------------
// GLOBAL VARIABLES
// --------------------------------------
unsigned short vsmMem[MEM_SIZE/WORD_SIZE]; //The entire memory space of the VSM, including CODE and DATA
short accumulator;
unsigned instructionCounter, instructionRegister; 

// --------------------------------------
// MAIN
// --------------------------------------
int main(){
  unsigned short opCode, n, operand, i;
  unsigned short numOfInstructions = 0; //Number of instructions (bit strings)
  unsigned short isDone = 0, isError = 0;

  //Load all the instructions into memory
  scanMem(vsmMem, &numOfInstructions);
  //printf("There are %d instructions.\n", numOfInstructions);
  
  
  //Run the instructions
  instructionCounter = 0; //Start at the first instruction
  while (!isDone && !isError) {
    //Check if the instructionCounter is pointing to a valid CODE location
    if (instructionCounter % WORD_SIZE != 0) {
      //PRINT FATAL ERROR MESSAGE
      printf("Address is at the wrong location\n");
      break;
    }
    if (instructionCounter / WORD_SIZE > numOfInstructions) {
      //PRINT FATAL ERROR MESSAGE
      printf("Location of address is out of bounds\n");
      break;
    }
    
    //Load the next instruction into the instructionRegister
    instructionRegister = vsmMem[instructionCounter / WORD_SIZE];
    
    //Update the instructionCounter with the location of the next instruction (get it ready for the next loop)
    instructionCounter += WORD_SIZE;
    
    //Run the instruction
    opCode = opCodeGen(instructionRegister);
    n = middleBit(instructionRegister);
    operand = operandGen(instructionRegister);

    printf("%x\n", instructionRegister);
    switch (opCode) {
      case OP_EOC:
        isDone = 1;
        break;
      case OP_LOAD:
        isError = opLoad(n, operand);
        break;
      case OP_STORE:
        isError = opStore(operand);
        break;
      case OP_READ:
        isError = opRead(operand);
        break;
      case OP_WRITE:
        isError = opWrite(operand);
        break;
      case OP_ADD:
        isError = opAdd(n, operand);
        break;
      case OP_SUB:
        isError = opSub(n, operand);
        break;
      case OP_MUL:
        isError = opMul(n, operand);
        break;
      case OP_DIV:
        isError = opDiv(n, operand);
        break;
      case OP_JUMP:
        isError = opJump(operand);
        break;
      case OP_JNEG:
        isError = opJneg(operand);
        break;
      case OP_JZERO:
        isError = opJzero(operand);
        break;
      case OP_HALT:
        isDone = 1;
        break;
      default:
        //PRINT ERROR MESSAGE - UKNOWN OPCODE
        isError = 1;
        break;
    }
  }
  if (isError == 1) {
    printf("*** VSM execution abnormally terminated ***\n");
  }

  computerDump(opCode,operand);
}

// --------------------------------------
//  HELPER FUNCTIONS
// --------------------------------------

void scanMem(unsigned short *pvsmMem, unsigned short *pNumOfInstructions){
  unsigned short hex;
  while(1){
    scanf("%hx", &hex);
    *pvsmMem = hex;
    pvsmMem++;
    *pNumOfInstructions += 1;
    if(hex == 0)
      break;
  }
}

unsigned short opCodeGen(unsigned short hex){
  hex = hex >> 12;
  return hex;
}

unsigned short middleBit(unsigned short hex){
  hex = hex >> 11;
  hex = hex & 1;
  return hex;
}

unsigned short operandGen(unsigned short hex){
  hex = hex & 0x7FF;
  return hex;
}

// --------------------------------------
//  OPERATIONS
// --------------------------------------

//Load a word at a specific location in memory(or a number) into the accumulator
int opLoad(unsigned short middleBit, unsigned short operand) {
  if (middleBit == 0) {
    //Check if its within range of the data section
    if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024)
    //The operand contains the memory location to get the value from
      accumulator = vsmMem[operand/WORD_SIZE]; //Store address into accu
    else 
      return 1;
  }
  else {
    //The operand contains the actual value to load
    accumulator = operand;

  }
  return 0; // Return 0 If there is no error
}

//Store a word in the accumulator into a specific location in memory
int opStore(unsigned short operand) {
  if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024){
    vsmMem[operand/WORD_SIZE] = accumulator;
    return 0;
  }
  else{
    return 1;
  }
}

//Read a word from the standard input into a specific location in memory
int opRead(unsigned short operand){
  scanf("%hx", &vsmMem[operand/WORD_SIZE]);
  return 0;
}

//Write a word at a specific memory location in memory to the standard output
int opWrite(unsigned short operand) {
  if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024){
    printf("%hd\n", vsmMem[operand/WORD_SIZE]);
    return 0;
  }
  else{
    return 1;
  }
}

/*Add a word at a specific location in memory(or a number) to the word in the
accumalator, leaving the sum in the accumalator*/
int opAdd(unsigned short middleBit, unsigned short operand) {
  if (middleBit == 0) {
    //The operand contains the memory location to get the value from
    accumulator = accumulator + vsmMem[operand/WORD_SIZE];
  }
  else {
    //The operand contains the actual value to add
    accumulator = accumulator + operand;
  }
  return 0;
}

/*Subtract a word at a specific location in memory(or a number) to the word in the
accumalator, leaving the difference in the accumalator*/
int opSub(unsigned short middleBit, unsigned short operand) {
  if (middleBit == 0) {
    if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024)
    //The operand contains the memory location to get the value from
      accumulator = accumulator - vsmMem[operand/WORD_SIZE];
    else 
      return 1;
  }
  else {
    //The operand contains the actual value to subtract
    accumulator = accumulator - operand;
  }
  return 0;
}

/*Mult a word at a specific location in memory(or a number) to the word in the
accumalator, leaving the product in the accumalator*/
int opMul(unsigned short middleBit, unsigned short operand) {
  if (middleBit == 0) {
    if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024)
    //The operand contains the memory location to get the value from
      accumulator = accumulator * vsmMem[operand/WORD_SIZE];
    else 
      return 1;
  }
  else {
    //The operand contains the actual value to multiply
    accumulator = accumulator * operand;
  }
  return 0;
}

/*Divide a word at a specific location in memory(or a number) to the word in the
accumalator, leaving the quoitent in the accumalator*/
int opDiv(unsigned short middleBit, unsigned short operand) {
  if (operand == 0){
    printf("*** Attempt to divide by zero ***\n");
    return 1;
  }
  else if (middleBit == 0){
    if((operand/WORD_SIZE) >= 512 && (operand/WORD_SIZE) < 1024)
    //The operand contains the memory location to get the value from
    accumulator = accumulator / vsmMem[operand/WORD_SIZE];
  }
  else{
    //The operand contains the actual value to divide
    accumulator = accumulator / operand;
  }
  return 0;
}

/*Branch to a specific location in the memory*/
int opJump(unsigned short operand) {
  if((operand/WORD_SIZE) >= 0  && (operand/WORD_SIZE) <= 511)
    instructionCounter = operand;
  else
    return 1;
  return 0;
}

/*Branch to a specific location in the memory if accumulator is negative*/
int opJneg(unsigned short operand) {
  if ((operand/WORD_SIZE) < 0  && (operand/WORD_SIZE) >= 512){
    printf("*** Address out of bounds ***\n");
    return 1;
  }

  if (accumulator < 0)
    instructionCounter = operand;
  
  return 0;
}

/*Branch to a specific location in the memory if accumulator is zero*/
int opJzero(unsigned short operand) {
  if((operand/WORD_SIZE) >= 0  && (operand/WORD_SIZE) <= 511){
    if (accumulator == 0)
      instructionCounter = operand;
  }
  else{
    printf("*** Address out of bounds ***\n"); 
    return 1;
  }
  return 0;
}

/*Printing computer dump*/
void computerDump(unsigned short opCode, unsigned short operand){
  printf("REGISTERS:\n");
  printf("accumulator\t\t0x%04X\n", accumulator);
  printf("instructionCounter\t0x%04X\n", instructionCounter);
  printf("instructionRegister\t0x%04X\n\n", instructionRegister);

  printf("opCode\t\t\t0x%X\n", opCode);
  printf("operand\t\t\t0x%X\n\nCode:\n", operand);

  int i, j, top;
  int y = 0;

  unsigned char* byteArray = vsmMem;
  printf("%5.0s", " ");
  for(top = 0; top < 10; top++){
    printf("%2d ", top);
  }
  printf("\n");
  
  for (i = 0; i < 100; i += 10){
    printf("%1.0s", " ");
    printf("%04d ",i);
    for( j = 0; j < 10; j++){
      printf("%02X ", (unsigned)byteArray[y]);
      y++;
    }
    printf("\n");
  }
  printf("%1.0s...\n\n", " ");
  printf("Data:\n");


  printf("%5.0s", " ");
  for(top = 0; top < 10; top++){
    printf("%2d ", top);
  }
  printf("\n");
  y = 1024; 
  for (i = 1024; i < 1124; i += 10){
    printf("%1.0s", " ");
    printf("%04d ",i);
    for( j = 0; j < 10; j++){
      printf("%02X ", (unsigned)byteArray[y]);
      y++;
    }
    printf("\n");
  }   
}


