/*
 ============================================================================
 Name        : Ex5.c
 Author      : Wooseok
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <signal.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

void sig_int(int signo);

int main()
{
    struct sigaction intsig;
    
    // SIGINT (CTRL+C) ---
    intsig.sa_handler = sig_int;
    sigemptyset(&intsig.sa_mask);
    intsig.sa_flags = 0;
    if (sigaction(SIGINT, &intsig, 0) == -1)
    {
        printf ("signal(SIGINT) error");
        return -1;
    }    
    // ---------------------------------

    while(1)
    {
        printf("HELLO!\n");
        sleep(1);
    }
}

void sig_int(int signo)
{
    char in;
    sigset_t sigset, oldset;
    sigemptyset(&oldset);

    // SIGUSRs are blocked
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR2);
    sigaddset(&sigset, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &sigset, &oldset) < 0)
    {
        printf("sigprocmask %d error \n", signo);
    }

    // UNBLOCK SIG INT
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    if (sigprocmask(SIG_UNBLOCK, &sigset, &oldset) < 0)
    {
        printf("sigprocmask %d error \n", signo);
    }
	
    printf("are you sure to exit? (y/n)");
    in = getchar();

    switch(in) {
	case 'y':
	    printf("EXIT!\n");
	    exit(0);
	break;
	case 'n':
	break;
	default:
	    printf("WRONG INPUT! CONTINUE\n");
	break;
    }
}
