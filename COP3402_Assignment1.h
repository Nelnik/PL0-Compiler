// Created by Alexander Kinlen
// COP3402 
// Assainment 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
// Instructions
typedef struct instructions{
int op;
int l;
int m;
} instruction;
	
#define MAX_STACK_HEIGHT  2000 	// Max stack height	
#define MAX_instructionStore_LENGTH  500 	// Max number of instructions	
#define MAX_LEXI_LEVELS  3      // Max Number of stack frames    

// Registers
int SP = 0;
int BP = 1;
int PC = 0; 
instruction IR;

// Temporary variable to test if the instruction is valid
instruction ErrorCheck;

// To stop the fetch-execute cycle
int halt =0;

// Keeping track of instructionStore line
int line;

int stack[MAX_STACK_HEIGHT];		//Stack to store
instruction instructionStore[MAX_instructionStore_LENGTH];	//instructionStore Store

int lexi[MAX_LEXI_LEVELS];              //Keeping track of stack frames
int lv =0;

const char* OP_String[] = {"","LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO"}; 	// Names of the op instructionStores 
												
																									
// List of functions
void readFile();
void fetch();
void execute();
void ALUOperation();
int base(int l);
void print();
void initialize();

// Output File
FILE* outFile;
FILE* A;

int VM()
{
     lexi[0]=0;
     lexi[1]=0;
     lexi[2]=0;
     
	// File to write to
	outFile =fopen("stacktrace.txt","w");
	A=fopen("acode.txt","w");
	// Header for first section
	fprintf(A,"  Line     OP    L       M\n");
	// Function to read file and store instruction
	readFile();
	// Header for second section
	fprintf(outFile,"                              PC   BP    SP    Stack\nInitial values                 %d    %d     %d\n",PC,BP,SP);
	
	// Fetch then Execute then print
	while(halt==0)
	{
		fetch();
		execute();
		print();
	}
	
	fclose(A);
	fclose(outFile);
	return;
}

// Set all cells in stack to 0
void initialize()
{
	int i =0;
	while(i<MAX_STACK_HEIGHT)
	{
		stack[i]=0;
		i++;
	}
}


// Store the instructions to instruction array
// after checking instructions
void readFile()
{
	FILE* inputFile;
	inputFile =fopen("mcode.txt","r");
	int i =0;
	int valid =0;
	
	// Store instructions to ErrorCheck first to see if the instruction is valid
	while (fscanf(inputFile, "%d  %d %d",&ErrorCheck.op,&ErrorCheck.l,&ErrorCheck.m ) != EOF) 
	{
		if(ErrorCheck.op==1 && ErrorCheck.l==0)
			valid=1;
		
		if(ErrorCheck.op==2 && ErrorCheck.l==0 && ErrorCheck.m>=0 && ErrorCheck.m<=13)
			valid = 1;
			
		if(ErrorCheck.op==3 && ErrorCheck.l>=0 && ErrorCheck.l<=MAX_LEXI_LEVELS)
			valid=1;
		
		if(ErrorCheck.op==4 && ErrorCheck.l>=0 && ErrorCheck.l<=MAX_LEXI_LEVELS)
			valid=1;
			
		if(ErrorCheck.op==5 && ErrorCheck.l<=MAX_LEXI_LEVELS && ErrorCheck.l >=0)
			valid=1;
			
		if(ErrorCheck.op==6 && ErrorCheck.l==0)
			valid=1;
			
		if(ErrorCheck.op==7 && ErrorCheck.l==0)
			valid=1;
		
		if(ErrorCheck.op==8 && ErrorCheck.l ==0)
			valid=1;		
		
		if(ErrorCheck.op>=9 && ErrorCheck.op<=11 && ErrorCheck.l==0 && ErrorCheck.m>=0 &ErrorCheck.m<=3) 
			valid=1;
		// Invalid instruction case	
		if(valid==0)
		{
			fprintf(outFile,"Sorry the following instruction is not supported : %d %d %d\n",ErrorCheck.op,ErrorCheck.l,ErrorCheck.m);
			halt=1;
			return;
		}
			
		// For valid instructions	
		if(valid == 1)
		{
			instructionStore[i].op=ErrorCheck.op;
			instructionStore[i].l=ErrorCheck.l;
			instructionStore[i].m=ErrorCheck.m;
			fprintf(A,"  %3d%8d%5d%8d\n",i,instructionStore[i].op,instructionStore[i].l,instructionStore[i].m);
		}
			
		valid=0;
		i++;
	}
	fclose(inputFile);	
}


// print current instruction and stack
void print()
{
	int i=1;
	int operation;
	if(IR.op>=9)
	{
		operation=9;
	}
	else
	{
		operation=IR.op;
	}
	
	if(halt==1)
	{
		SP=0;
		PC=0;
		BP=0;
	}
	
	fprintf(outFile,"  %3d%8s%5d%7d%7d%5d%6d    ",line,OP_String[operation],IR.l,IR.m,PC,BP,SP);
	
	// So stack doesn't display when halt
	if(halt==1)
		return;
	// Prints the current stack
	while(i<=SP)
	{
        fprintf(outFile,"%d ",stack[i]);
        
		if(i==lexi[0] || i==lexi[1] ||i==lexi[2]) 
        {
           
                              
           if(i!=1 && i<SP)
			  fprintf(outFile,"| ");
        }
		
		i++;
		
	}
	fprintf(outFile,"\n");
}

// Find base L levels
int base(l) {
        int b1 = BP; // find base L levels down
        while (l > 0) {
            b1 = stack[b1 + 1];
            l--;
        }
        return b1;
    }

// Fecth Cycle
void fetch()
{
    IR = instructionStore[PC];
    line=PC;
    PC++;
	return;	
}

// Executtion Cycle
void execute()
{
	//LIT
	if(IR.op==1)
	{
		SP++;
		stack[SP]=IR.m;
	}
	//OPR
	if(IR.op==2)
	{
		if(IR.m==0)
		{
			
			SP=BP-1;
			PC=stack[SP+4];
			BP=stack[SP+3];
			lexi[lv]=0;
			lv++;
		}
		else
		{
			ALUOperation();
		}
	}
	//LOD
	if(IR.op==3)
	{
		SP++; 
		stack[SP]=stack[base(IR.l)+IR.m];
	}
	//STO
	if(IR.op==4)
	{
		stack[base(IR.l)+IR.m]=stack[SP];
		SP--;
	}
	//CAL
	if(IR.op==5)
	{
		stack[SP+1]=0;
		stack[SP+2]=base(IR.l);
		stack[SP+3]=BP;
		stack[SP+4]=PC;
		BP=SP+1;
		PC=IR.m;
		lexi[lv] = SP;
        lv++;
	}
	//INC
	if(IR.op==6)
	{
		SP=SP+IR.m;
	}
	//JMP
	if(IR.op==7)
	{
		PC=IR.m;
	}
	//JPC
	if(IR.op==8)
	{
		if(stack[SP]==0)
			PC=IR.m;
		SP--;
	}
	//SIO 1
	if(IR.op==9)
	{
		printf("%d\n",stack[SP]);
		SP--;
	}
	//SIO 2
	if(IR.op==10)
	{
		SP++;
		int i;
        while (!scanf("%d", &i));
        	stack[SP] = i;
	}
	//SIO 3
	if(IR.op==11)
	{
		halt=1;
	}
	
	return;
}

void ALUOperation()
{
	switch(IR.m)
	{
		// NEG
		case 1:
			stack[SP]=stack[SP]*(-1);
			break;
		//ADD
		case 2:
			SP--;
			stack[SP]=stack[SP]+stack[SP+1];
			break;	
		//SUB
		case 3:
			SP--;
			stack[SP]=stack[SP]-stack[SP+1];
			break;
		//MUL
		case 4:
			SP--;
			stack[SP]=stack[SP]*stack[SP+1];
			break;
		//DIV
		case 5:
			SP--;
			stack[SP]=stack[SP]/stack[SP+1];
			break;
		//ODD
		case 6:
			stack[SP]=stack[SP]%2==1;
			break;
		//MOD
		case 7:
			SP--;
			stack[SP]=stack[SP]%stack[SP+1];
			break;
		//EQL
		case 8:
			SP--;
			stack[SP]=stack[SP]==stack[SP+1];
			break;
		//NEQ
		case 9:
			SP--;
			stack[SP]=stack[SP]!=stack[SP+1];
			break;
		//LSS
		case 10:
			SP--;
			stack[SP]=stack[SP]<stack[SP+1];
			break;
		//LEQ
		case 11:
			SP--;
			stack[SP]=stack[SP]<=stack[SP+1];
			break;
		//GTR
		case 12:
			SP--;
			stack[SP]=stack[SP]>stack[SP+1];
			break;
		//GEQ
		case 13:
			SP--;
			stack[SP]=stack[SP]>=stack[SP+1];
			break;
	}
	
}



