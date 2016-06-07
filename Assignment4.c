// Alexander Kinlen, Jesse Lopez, Khris Bandong

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Assignment2v4.h"
#include "Assignment3.h"
#include "COP3402_Assignment1.h"

void display();


int t=0;
int s=0;
int m=0;
int a=0;
int v=0;
FILE* file;
	
void main(int argc, char *argv[] ){
	int count;
	for(count=1;count<argc;count++){
		if(strcmp(argv[count],"-t")==0){
			
			t=1;
		}
		else if(strcmp(argv[count],"-s")==0){
			s=1;
		}else if(strcmp(argv[count],"-m")==0){
			m=1;
		}else if(strcmp(argv[count],"-a")==0){
			a=1;
		}else if(strcmp(argv[count],"-v")==0){
			v=1;
		}	
		else{
			printf("%s invalid command.\n",argv[count]);
		}		
				
		
				
			
		
		
	}
	
	scanner();
	parser();
	VM();
	
	
	
	
	display();
	
}

void display(){
	
	int c;
	
	if(t==1){
		file=fopen("tokenlist.txt","r");
		while ((c = getc(file)) != EOF)
        	putchar(c);
		printf("\n\n");
		fclose(file);
	}
	
	if(s==1){
		file=fopen("symlist.txt","r");
		while ((c = getc(file)) != EOF)
        	putchar(c);
		printf("\n\n");
		
		fclose(file);
	}
	if(m==1){
		file=fopen("mcode.txt","r");
		while ((c = getc(file)) != EOF)
        	putchar(c);
		printf("\n\n");
		fclose(file);
	}
	if(a==1){
		file=fopen("acode.txt","r");
		while ((c = getc(file)) != EOF)
        	putchar(c);
		printf("\n\n");
		fclose(file);
	}
	
	if(v==1){
		file=fopen("stacktrace.txt","r");
		while ((c = getc(file)) != EOF)
        	putchar(c);
		printf("\n\n");
		fclose(file);
	}
	return;
}

