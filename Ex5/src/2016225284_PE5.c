/*
 ============================================================================
 Name        : Ex5.c
 Author      : Wooseok
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void are_you_sure(int signo);


int main(void) {
	signal(SIGINT, (void*)are_you_sure);
	while(1) {
		printf("Hello\n");
		sleep(4);
	}
	return 0;
}

void are_you_sure(int signo){
	char c = 0;
	printf("Interrupted! OK to quit (y/n)? ");
	c = getchar();
	//	putchar('\n');
	if(c == 'y') exit(0);
}
