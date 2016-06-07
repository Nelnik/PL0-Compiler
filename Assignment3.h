// Created by Alexander Kinlen, Khris Bandong, Jesse Lopez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, 
	oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, 
	commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, 
	whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym, elsesym
} token_type;


typedef enum{
	LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO
} opcode;


typedef enum{
	NEG = 1, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} ALUop;

typedef struct symbol{
	int type;
	char name[12];
	int val;
	int level;
	int addr;
}symbol;


typedef struct code{
	int op;
	int l;
	int m;
}code;



#define CODE_SIZE 500
#define SYMBOL_TABLE_SIZE 100

char curID[12];
int curNum;
FILE* inputFile;
token_type Token = 1;
int codeIndex = 0;
int symbolIndex = 0;
int curLevel = -1;
int procedureCount = 0;

symbol symbolTable[SYMBOL_TABLE_SIZE];
code codeStore[CODE_SIZE];

void openFile();
// create mcode
void Gen(int op, int l, int m);
// scan the next token
void Get_Token();
// print errors
void error(int code);

// these 3 replace Enter();
void createConstSym();
void createVarSym();
void createProcSym();
// parsing
void block();
void constDec();
int varDec();
void procDec();
void statement();
void condition();
void expression();
void term();
void factor();
int find(char* ident);
int symbolType(int num);
int symbolLevel(int num);
int symbolAddress(int num);
int symbolValue(int num);

void parser(){
	openFile();
	Get_Token();
	block();
	if(Token != periodsym){
		
		error(9);
	}
		
	Gen(SIO, 0, 3);
	fclose(inputFile);
	int i;
	FILE* output;
	output=fopen("mcode.txt","w");
	for(i = 0; i < codeIndex; i++){
		fprintf(output,"%d %d %d\n", codeStore[i].op, codeStore[i].l, codeStore[i].m);
	}
	fclose(output);
	output=fopen("symlist.txt","w");
	for(i = 0; i < symbolIndex; i++){
		fprintf(output,"%s %d %d %d\n", symbolTable[i].name, symbolTable[i].type, symbolTable[i].level, (symbolTable[i].type == 1 ? symbolTable[i].val : symbolTable[i].addr));
	}
	fclose(output);
}

void openFile(){
	if((inputFile = fopen("tokenlist.txt", "r")) == NULL) {
        printf("Unable to find input file!" "\n\nExiting program");
        exit(EXIT_FAILURE);
    }
}

void Gen(int op, int l, int m){

	if(codeIndex > CODE_SIZE)
		error(25);
	if(op == SIO){
		if(m == 1)
			op = 9;
		else if(m == 2)
			op = 10;
		else if(m == 3)
			op = 11;
		else
			error(28);
	}
	
	codeStore[codeIndex].op = op; 
	codeStore[codeIndex].l = l; 
	codeStore[codeIndex].m = m; 
	codeIndex++;
}

void Get_Token(){
	fscanf(inputFile, "%d", &Token);
	//printf("%d\n",Token);
	if(Token == identsym){
		fscanf(inputFile, " %s", curID);
	}
	else if(Token == numbersym)
		fscanf(inputFile, "%d", &curNum);
	
}

void error(int code){
	printf("ERROR: ");
	switch(code){
		case 1:
			printf("Use '='' instead of ':='\n");
			break;
		case 2:
			printf("'=' must be followed by a number\n");
			break;
		case 3:
			printf("Indentifier must be followed by ':='\n");
			break;
		case 4:
			printf("Const/Var/Procedure must be followed by an identifer\n");
			break;
		case 5:
			printf("Semicolon or comma missing\n");
			break;
		case 6:
			printf("Incorrect symbol after procedure declaration\n");
			break;
		case 7:
			printf("Statement expected\n");
			break;
		case 8:
			printf("Incorrect symbol after statement part in block\n");
			break;
		case 9:
			printf("Period expected\n");
			break;
		case 10:
			printf("Semicolon between statements missing\n");
			break;
		case 11:
			printf("Undeclared identifier: %s\n", curID);
			break;
		case 12:
			printf("Assignment to constant or procedure is not allowed\n");
			break;
		case 13:
			printf("Assignment operator expected\n");
			break;
		case 14:
			printf("'call' must be followed by an identifier\n");
			break;
		case 15:
			printf("Call of a constant or variable is meaningless\n");
			break;
		case 16:
			printf("'then' expected\n");
			break;
		case 17:
			printf("Semicolon or '}' expected\n");
			break;
		case 18:
			printf("'do' expected\n");
			break;
		case 19:
			printf("Incorrect symbol following statement\n");
			break;
		case 20:
			printf("Relational operator expected\n");
			break;
		case 21:
			printf("Expression must not contain a procedure identifier\n");
			break;
		case 22:
			printf("Right parenthesis missing\n");
			break;
		case 23:
			printf("The preceding factor cannot begin with this symbol\n");
			break;
		case 24:
			printf("An expression cannot begin with this symbol\n");
			break;
		case 25:
			printf("This number is too large\n");
			break;
		case 26:
			printf("Begin must be followed by 'end'\n");
			break;
		case 27:
			printf("Factor, left paren, or factor expected\n");
			break;
		case 28:
			printf("CODE GENERATION ERROR: SIO can have arguments of 1, 2, or 3 ONLY\n");
			break;
		case 29:
			printf("Read and write must be followed by an identifier\n");
			break;
		default:
			printf("Indeterminate error!\n");
			break;
	}
	printf("Exiting.\n");
	exit(EXIT_FAILURE);
}

void createConstSym(){ 
	symbol sym;
	sym.type = 1;
	strcpy(sym.name, curID);
	sym.val = curNum;
	sym.level = curLevel;
	symbolTable[symbolIndex++] = sym;
}

void createVarSym(){
	symbol sym;
	sym.type = 2;
	strcpy(sym.name, curID);
	sym.level = curLevel;
	symbolTable[symbolIndex++] = sym;
}

void createProcSym(){
	symbol sym;
	sym.type = 3;
	strcpy(sym.name, curID);
	sym.level = curLevel;
	sym.addr = codeIndex;
	symbolTable[symbolIndex++] = sym;
}




void block(){
	curLevel++;
	int space = 4;
	int jmpAddr = codeIndex;
	Gen(JMP, 0, 0);
	if(Token == constsym)
		constDec();
	if(Token == varsym)
		space += varDec();
	if(Token == procsym)
		procDec();
	codeStore[jmpAddr].m = codeIndex;
	Gen(INC, 0, space); //allocate enough space for locals and AR info
	statement();
	if(curLevel!=0)
		Gen(OPR, 0, 0); //return
	curLevel--;
	//Get_Token();
}



void constDec(){
	do{ 
		Get_Token();
		if(Token != identsym) 
			error(4);
		Get_Token();
		if(Token != eqsym) 
			error(13);
		Get_Token();
		if(Token != numbersym) 
			error(2);
		createConstSym();
		Get_Token();
	} while(Token == commasym);

	if(Token != semicolonsym) 
		error(5);
	Get_Token();
}

int varDec(){
	int varCount = 0;
	do{ 
		Get_Token();
		if(Token != identsym) error(4);
		createVarSym();
		symbolTable[symbolIndex - 1].addr = 4 + varCount;
		varCount++;
		Get_Token();
	} while(Token == commasym);

	if(Token != semicolonsym) error(5);
	Get_Token();
	return varCount;
}

void procDec(){
	while(Token == procsym){
		Get_Token();
		if(Token != identsym) error(4);
		createProcSym();
		Get_Token();
		if(Token != semicolonsym) error(5);
		Get_Token();
		block();
		if(Token != semicolonsym) error(5);
		Get_Token();
	}
}

void statement(){
	if(Token == identsym){
		int num = find(curID);
		if(num == -1) error(11);
		if(symbolTable[num].type != 2) 
			error(12);
		Get_Token();
		if(Token != becomessym) 
			error(3);
		Get_Token();
		expression();
		Gen(STO, curLevel - symbolLevel(num), symbolAddress(num));
	}
	else if(Token == callsym){
		Get_Token();
		if(Token != identsym) error(14);
		int num = find(curID);
		if(num == -1) 
			error(11);
		if(symbolType(num) == 3)
			Gen(CAL, curLevel - symbolLevel(num), symbolAddress(num));
		else error(14);
		Get_Token();		
	}
	else if(Token == beginsym){
		Get_Token();
		statement();
		while(Token == semicolonsym){
			Get_Token();
			statement();
		}
		if(Token != endsym) 
			error(26);
		Get_Token();
	}
	else if(Token == ifsym){ 
		Get_Token();
		condition();
		if(Token != thensym) 
			error(16);
		Get_Token();
		int temp = codeIndex;
		Gen(JPC, 0, 0);
		statement();
		if(Token == elsesym){
			Get_Token();
			int temp2 = codeIndex;
			Gen(JMP, 0, 0);
			codeStore[temp].m = codeIndex;
			statement();
			codeStore[temp2].m = codeIndex;
		}
		else{
			Gen(JMP, 0, codeIndex+1);
			codeStore[temp].m = codeIndex;
		}
	}
	else if(Token == whilesym){ 
		int start = codeIndex;
		Get_Token();
		condition();
		int temp = codeIndex;
		Gen(JPC, 0, 0);
		if(Token != dosym) 
			error(18);
		Get_Token();
		statement();
		Gen(JMP, 0, start);
		codeStore[temp].m = codeIndex;
	}
	else if ( Token == readsym )
    {
        Get_Token();
        if ( Token != identsym )
        {
            error(29); 
        }
        Get_Token();
        char* i = curID;
        int index = find(i);
        if ( symbolTable[index].type != 2 )
        {
            error(17);
        }
        Gen( SIO,  0, 2 );   
        Gen( STO,  curLevel - symbolTable[index].level, symbolTable[index].addr );
        Get_Token();
    }
     else if ( Token == writesym )
    {
        Get_Token();
        if ( Token != identsym )
        {
            error(29);
        }
        Get_Token();
        char* i = curID;
        int index = find(i);
        if ( symbolTable[index].type != 2 )
        {
            error(17);
        }
        Gen( LOD,  curLevel - symbolTable[index].level, symbolTable[index].addr );   
        Gen( SIO,  0, 1 );
        Get_Token();
        
    }
}

void condition(){
    if(Token == oddsym){
        Get_Token();
        expression();
        Gen(OPR,0,ODD);
    }
    else{
        expression();
        int relOp = Token - 1; 
        if(Token != eqsym && Token != neqsym && Token != gtrsym && Token != geqsym && Token != lessym && Token != leqsym){
            error(20);
        }
        Get_Token();
        expression();
        Gen(OPR, 0, relOp);
    }
}

void expression(){
	int addingOp;
  if (Token == plussym || Token == minussym){ 
      addingOp = Token;
      Get_Token();
      term();
      Gen(OPR, 0, NEG); 
  }
  else
  	term();
  while (Token == plussym || Token == minussym){
      addingOp = Token;
      Get_Token();
      term();
      if(addingOp == plussym)
      	Gen(OPR, 0, ADD);
      else if(addingOp == minussym)
      	Gen(OPR, 0, SUB);
  }
}

void term(){
	int multiplyOp;
  	factor();
	while (Token == multsym || Token == slashsym){
	      multiplyOp = Token;
	      Get_Token();
	      factor();
	
	      if(multiplyOp == multsym)
	      	Gen(OPR, 0, MUL);
	      else if(multiplyOp == slashsym)
	      	Gen(OPR, 0, DIV);
	}
}

void factor(){
    if (Token == identsym){
        int num = find(curID);
        if(num == -1) error(11);
        if(symbolType(num) == 2)
        	Gen(LOD, curLevel - symbolLevel(num), symbolAddress(num)); 
        else if(symbolType(num) == 1)
        	Gen(LIT, 0, symbolValue(num)); 
        else
        	error(21);
        Get_Token();
    }
    else if (Token == numbersym){
        Gen(LIT, 0, curNum);
        Get_Token();
    } 
    else if (Token == lparentsym){
        Get_Token();
        expression();
        if (Token != rparentsym){
            error(22);
        }
        Get_Token();
    }
    else
    	error(27);
}

int find(char* ident){
	int i;
	for(i = symbolIndex - 1; i >= 0; i--){
		if(strcmp(symbolTable[i].name, ident) == 0 && symbolTable[i].level <= curLevel)
			return i;
		else if(strcmp(symbolTable[i].name, ident) == 0 && symbolType(i) == 1)
			return i;
	}
	return -1;
}

int symbolType(int num){
	return symbolTable[num].type;
}

int symbolLevel(int num){
	return symbolTable[num].level;
}

int symbolAddress(int num){
	return symbolTable[num].addr;
}

int symbolValue(int num){
	return symbolTable[num].val;
}



