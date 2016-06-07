// Created by Alexander Kinlen, Jesse Lopez, and Khris Bandong
// COP3402 System Software
// Assignment 2

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

FILE* input;
FILE* cleanInput;
FILE* lexemetable;
FILE* tokenlist;
char inputString[1000];
char tempString[1000];
char cleanString[1000];
char temp[100];
int line =1;

int tokenList[100000];
char* tokenString[100000];


					



    
char digits[10] = {'0', '1', '2','3', '4','5','6','7','8','9'};

char alpha[52] = {'a','b','c','d','e','f','g','h','i','j',
				'k','l','m','n','o','p','q','r','s','t',
				'u','v','w','x','y','z','A','B','C','D',
				'E','F','G','H','I','J','K','L','M','N',
				'O','P','Q','R','S','T','U','V','W','X',
				'Y','Z'};
                     


void readInput();
void removeComments();
void convertToToken();


int scanner(){
	input=fopen("input.txt","r");
	cleanInput=fopen("cleanInput.txt","w");
	lexemetable=fopen("lexemetable.txt","w");
	tokenlist=fopen("tokenlist.txt","w");
	readInput();
	fclose(input);
	removeComments();
	fprintf(cleanInput,"%s",cleanString);
	fclose(cleanInput);
	cleanInput=fopen("cleanInput.txt","r");
	fprintf(lexemetable,"lexeme       token type\n");
	convertToToken();
	fclose(cleanInput);
	fclose(tokenlist);
//	fprintf(lexemetable,"\n\n");
//	int i=0;
//	while(tokenString[i]!='\0'){
//		fprintf(lexemetable,"%-11s%5d\n",tokenString[i],tokenList[i]);
//		i++;
//	}
//	
	
	return 0;
}



void convertToToken(){
	char current[100];
	
	
	int j = 0;
	char tmpch[1];
	
		
		
		int i=0;
		
		int k=0;
		while(cleanString[i]!='\0'){
			k=0;
			if(cleanString[i]==' '){
				i++;
			}
			if(cleanString[i]=='\t'){
				i++;
			}
			else if(cleanString[i]=='\n'){
				line++;
				i++;
			}
			
			else if(cleanString[i]=='n'&&cleanString[i+1]=='u'&& cleanString[i+2]=='l' && cleanString[i+3]=='l'&&!isalnum(cleanString[i+4])&&!isalnum(cleanString[i-1])){
				
							tokenString[j]="null";
							tokenList[j]=1;
							k=4;
							j++;
							fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
							fprintf(tokenlist,"%d ",tokenList[j-1]);
			}
			else if(cleanString[i]=='+'){
				tokenString[j]="+";
				tokenList[j]=4;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='-'){
				tokenString[j]="-";
				tokenList[j]=5;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='*'){
				tokenString[j]="-";
				tokenList[j]=6;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='/'){
				tokenString[j]="/";
				tokenList[j]=7;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='o' && cleanString[i+1]=='d'&& cleanString[i+2]=='d'&&!isalnum(cleanString[i+3])&&!isalnum(cleanString[i-1])){
				
						tokenString[j]="odd";
						tokenList[j]=8;
						j++;
						k=3;
						fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
						fprintf(tokenlist,"%d ",tokenList[j-1]);
					
				
				
			}else if(cleanString[i]=='='){
				if(cleanString[i-1]!=':'&&cleanString[i-1]!='>'&&cleanString[i-1]!='<'){
					tokenString[j]="=";
					tokenList[j]=9;
					j++;
					k=1;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				}
			}else if(cleanString[i]=='<' && cleanString[i+1]=='>'){
				tokenString[j]="<>";
					tokenList[j]=10;
					j++;
					k=2;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='<'){
				if(cleanString[i+1]!='='&& cleanString[i+1]!='>'){
					tokenString[j]="<";
					tokenList[j]=11;
					j++;
					k=1;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				}
				
			}else if(cleanString[i]=='<'&&cleanString[i]=='='){
				tokenString[j]="<=";
				tokenList[j]=12;
				j++;
				k=2;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='>'&&cleanString[i+1]!='='){
				tokenString[j]=">";
				tokenList[j]=13;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='>'&&cleanString[i+1]=='='){
				tokenString[j]=">=";
				tokenList[j]=14;
				j++;
				k=2;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='('){
				tokenString[j]="(";
				tokenList[j]=15;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]==')'){
				tokenString[j]=")";
				tokenList[j]=16;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]==','){
				tokenString[j]=",";
				tokenList[j]=17;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]==';'){
				tokenString[j]=";";
				tokenList[j]=18;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='.'){
				tokenString[j]=".";
				tokenList[j]=19;
				j++;
				k=1;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]==':'&& cleanString[i+1]=='='){
				tokenString[j]=":=";
				tokenList[j]=20;
				j++;
				k=2;
				fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
			}else if(cleanString[i]=='b'&&cleanString[i+1]=='e'&& cleanString[i+2]=='g' && cleanString[i+3]=='i'&& cleanString[i+4]=='n'&&!isalnum(cleanString[i+5])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="begin";
					tokenList[j]=21;
					j++;
					k=5;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
			
				
			}else if(cleanString[i]=='e'&&cleanString[i+1]=='n'&& cleanString[i+2]=='d' &&!isalnum(cleanString[i+3])&&!isalnum(cleanString[i-1])){
			
					tokenString[j]="end";
					tokenList[j]=22;
					j++;
					k=3;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='i'&&cleanString[i+1]=='f' &&!isalnum(cleanString[i+2])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="if";
					tokenList[j]=23;
					j++;
					k=2;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='t'&&cleanString[i+1]=='h'&& cleanString[i+2]=='e'&& cleanString[i+3]=='n' &&!isalnum(cleanString[i+4])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="then";
					tokenList[j]=24;
					j++;
					k=4;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='w'&&cleanString[i+1]=='h'&& cleanString[i+2]=='i'&& cleanString[i+3]=='l'&& cleanString[i+4]=='e'&&!isalnum(cleanString[i+5])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="while";
					tokenList[j]=25;
					j++;
					k=5;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='d'&&cleanString[i+1]=='o' &&!isalnum(cleanString[i+2])&&!isalnum(cleanString[i-1])){
			
					tokenString[j]="do";
					tokenList[j]=26;
					j++;
					k=2;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='c'&&cleanString[i+1]=='a'&& cleanString[i+2]=='l'&& cleanString[i+3]=='l' &&!isalnum(cleanString[i+4])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="call";
					tokenList[j]=27;
					j++;
					k=4;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='c'&&cleanString[i+1]=='o'&& cleanString[i+2]=='n'&& cleanString[i+3]=='s'&& cleanString[i+4]=='t'&&!isalnum(cleanString[i+5] )&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="const";
					tokenList[j]=28;
					j++;
					k=5;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='v'&&cleanString[i+1]=='a'&& cleanString[i+2]=='r'&& !isalnum(cleanString[i+3])&&!isalnum(cleanString[i-1])){
				
					
					tokenString[j]="var";
					tokenList[j]=29;
					j++;
					k=3;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='p'&&cleanString[i+1]=='r'&& cleanString[i+2]=='o'&& cleanString[i+3]=='c'&& cleanString[i+4]=='e'&& cleanString[i+5]=='d'&& cleanString[i+6]=='u'&& cleanString[i+7]=='r'&& cleanString[i+8]=='e'&&!isalnum(cleanString[i+9])&&!isalnum(cleanString[i-1])){
			
					tokenString[j]="procedure";
					tokenList[j]=30;
					j++;
					k=9;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='w'&&cleanString[i+1]=='r'&& cleanString[i+2]=='i'&& cleanString[i+3]=='t'&& cleanString[i+4]=='e'&&!isalnum(cleanString[i+5])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="write";
					tokenList[j]=31;
					j++;
					k=5;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j]-1);
				
			}else if(cleanString[i]=='r'&&cleanString[i+1]=='e'&& cleanString[i+2]=='a'&& cleanString[i+3]=='d'&&!isalnum(cleanString[i+4])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="read";
					tokenList[j]=32;
					j++;
					k=4;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
					fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else if(cleanString[i]=='e'&&cleanString[i+1]=='l'&& cleanString[i+2]=='s'&& cleanString[i+3]=='e'&&!isalnum(cleanString[i+4])&&!isalnum(cleanString[i-1])){
				
					tokenString[j]="else";
					tokenList[j]=33;
					j++;
					k=4;
					fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
				fprintf(tokenlist,"%d ",tokenList[j-1]);
				
			}else{
				k=0;
				if( (cleanString[i]>='a'&& cleanString[i]<='z') || (cleanString[i]>='A' && cleanString[i]<='Z')){	
					int x=0;
					while(x<100){
						current[x]='\0';
						x++;
					}
					int y=0;
					while(isalnum(cleanString[i])){
						
						current[y]=cleanString[i];
						y++;
						i++;	
					}
					k=0;
					if(strlen(current)>11){
							printf("Line %d: Identifier %s is too long!\n",line,current);
							return;
					}else{
						tokenList[j]=2;
						tokenString[j]=current;
						j++;
						fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
						fprintf(tokenlist,"%d %s ",tokenList[j-1],tokenString[j-1]);
					}
					
				}else if(cleanString[i]>='0'&& cleanString[i]<='9'){
					int x=0;
					while(x<100){
						current[x]='\0';
						x++;
					}
					current[0]=cleanString[i];
					
					i++;
						int y=1;
					while(isalnum(cleanString[i])){
						if((cleanString[i]>='a'&& cleanString[i]<='z') || (cleanString[i]>='A' && cleanString[i]<='Z')){
							printf("Line %d: Identifier cannot start with a number!",line);
							return;
						}
						tmpch[0]=cleanString[i];
						current[y]=cleanString[i];
						y++;
						i++;
						k=0;
					}
					
					int len=strlen(current);
					if(len>5){
						printf("Line %d: Number exceed 5 digits!\n",line);
							return;
					}
					tokenList[j]=3;
						tokenString[j]=current;
						j++;
						fprintf(lexemetable,"%-11s%5d\n",tokenString[j-1],tokenList[j-1]);
						fprintf(tokenlist,"%d %s ",tokenList[j-1],tokenString[j-1]);	
				}else{
					printf("Line %d: %c is an invalid symbol\n",line,cleanString[i]);
					return;
					k=0;
					i++;
				}
				
				
			}
			
			i=i+k;
		
	}
	
}



void removeComments(){
	
	int isComment=0;
	int i=0;
	int j=0;
	while(inputString[i]!='\0'){
		if(inputString[i]=='/' && inputString[i+1]=='*'){
			isComment=1;
			i++;
		}
		if(isComment==0){
			cleanString[j]=inputString[i];
			j++;
			
		}
		if(inputString[i]=='*' && inputString[i+1]=='/'){
			isComment=0;
			i++;
		}
		i++;
	}
	

	
	return;
}

void readInput(){
	while(fgets(tempString,100000,input)!=NULL){
		strcat(inputString,tempString);
	}
	
	return;
}
