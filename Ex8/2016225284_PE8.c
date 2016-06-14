/*
 ============================================================================
 Name        : Ex8.c
 Author      : Wooseok
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <signal.h>



#define READ 0
#define WRITE 1

FILE *popen2(const char *command, const char *mode);

int main() {
	FILE *fp;
	char buf[BUFSIZ];

	fp = popen2("ls" ,"r");
	while(fgets(buf, BUFSIZ, fp) != NULL)
		fputs(buf, stdout);
}

FILE *popen2(const char *command, const char *mode) {
	int thepipe[2], pid;
	FILE *ret;

	if(*mode != 'r' && *mode != 'w' || mode[1])
		return(NULL);

	if(pipe(thepipe) < 0)
		return(NULL);

	if((pid = fork()) == -1) {
		close(thepipe[0]);
		close(thepipe[1]);
		return(NULL);
	}

	//CHILD
	if(pid == 0) {
		if(*mode == 'r'){
			dup2(thepipe[1], 1);
			close(thepipe[1]);
			close(thepipe[0]);
		}else {
			dup2(thepipe[0], 0);
			close(thepipe[0]);
			close(thepipe[1]);
		}
		execl("/bin/sh", "sh", "-c", command, NULL);
		exit(127);
	}

	//PARENT
	if(*mode == 'r') {
		ret = fdopen(thepipe[0], mode);
		close(thepipe[1]);
	}else {
		ret = fdopen(thepipe[1], mode);
		close(thepipe[0]);
	}

	return (ret);
}
