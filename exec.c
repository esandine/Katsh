#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include "cmds.h"
#include "utils.h"

/**
 *Args: Signal
 *Return: exits if the signal is SIGINT
 *What it Does: Catches the exit signal that is sent from the child process to
 *the parent if the command is exit
 */
static void sighandler(int signo){
  if(signo == SIGINT){
    printf("\nexiting\n\n");
    exit(0);
  }
}

int main(){
  signal(SIGINT, sighandler);
  char input[256];
  char cwd[256];
  while(1){
    getcwd(cwd, 255);
    printf("%s $ ", cwd);
    fgets(input, sizeof(input), stdin);
    *strstr(input,"\n")=0;//Replaces newline with null
    run_cmd_semi(input);
  }
  return 0;
}
