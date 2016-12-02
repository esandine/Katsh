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
#include "exec.h"

/**
 *Args: Signal
 *Return: exits if the signal is SIGINT
 *What it Does: Catches the exit signal that is sent from the child process to
 *the parent if the command is exit
 */
static void sighandler(int signo){
  if(signo == SIGINT){
    printf("exiting\n\n");
    exit(0);
  }
}
/**
 *Args: Input string
 *Return: the number of spaces and tabs in the Input
 *What it Does: counts the number of blanks in the string. This is used to
 *parse in parse_cmd
 */
int num_blanks(char* str){
  int spaces = 0;
  while (*str) {
    if (*str==' ' || *str=='\t') {
      spaces++;
    }
    str++;
  }
  return spaces;
}

/**
 *Args: Inputs string
 *Returns: An array of strings representing the command
 *What it does: Counts the number of spaces then callocs the appropriate
 *ammount of space. It then uses strsep to split the command into a list 
 *of arguements
 */
char** parse_cmd(char* input){
  char ** parse = (char **)calloc(sizeof(char *), num_blanks(input) + 2);
  int ctr = 0;
  char* next = input;
  while(next){
    // Skip leading whitespace
    while (next[0] == ' ' || next[0] == '\t') {
      next++;
    }
    if (next[0] == '\0') {
      break;
    }
    // Otherwise, there is an argument to parse
    parse[ctr]=strsep(&next, " ");
    ctr++;
  }
  ctr = 0;
  while(parse[ctr]){
    ctr++;
  }
  return parse;
}

/**
 *Args: Input string
 *Returns: status of waitpid
 *What it does: It parses the input, then checks if the first command is cd or
 *exit. If so it runs the command. If this is not the case it forks a child
 *process to run the command, and waits for the child process to run
 */
int run_cmd_fork(char* input){
  char ** parse = parse_cmd(input);
  if (strcmp(parse[0], "exit")==0) {
    exit(0);
  } else if (strcmp(parse[0], "cd")==0) {
    chdir(parse[1]);
    return 0;
  }
  // Otherwise, fork and execute the command
  pid_t pid = fork();
  if (pid==0) {
    execvp(parse[0], parse);
    // If we get here, it failed
    int e_errno = errno;
    if (e_errno == ENOENT) {
        printf("katsh: %s: no such file or directory\n", parse[0]);
    } else if (e_errno == EACCES) {
        printf("katsh: %s: permission denied\n", parse[0]);
    } else {
        printf("katsh: an error has occured. errno: %d\n", e_errno);
    }
    exit(1);
  } else {
    int status = 0;
    waitpid(pid, &status, 0);
    return status;
  }
}

/**
 *Args: Command that outputs to a file
 *Return: Void
 *What it Does: Executes a command and instead of printing to stdout, it prints
 *to a file.
 */
void run_cmd_stdout(char* input, int mode){
  printf("%d\n", mode);
  if (input == NULL) {
    return;
  }// Proceed if input is non-empty
  if (input[0]) {
    char* next = input;
    char* first;
    if(mode == 0){
      first = strsep(&next, ">");
    }
    else if(mode == 1){
      first = strsep(&next, ">");
      printf("%s\n", next);
      strsep(&next, ">");
      printf("%s\n", next);
    }
    else if(mode == 2){
      *strstr(next, "2>") = '>';
      first = strsep(&next, ">");
      strsep(&next, ">");
    }
    else if(mode == 3){
      *strstr(next, "2>") = '>';
      first = strsep(&next, ">");
      strsep(&next, ">");
      strsep(&next, ">");
    }
    else if(mode == 4){
      *strstr(next, "&>") = '>';
      first = strsep(&next, ">");
      strsep(&next, ">");
    }
    else{
      printf("Invalid mode");
      return;
    }
    while (next[0] == ' ' || next[0] == '\t') {
      next++;
    }
    while(next[strlen(next) - 1] == ' ' || next[strlen(next) - 1] == '\t'){
      next[strlen(next) - 1] = 0;;
    }
    while(first[strlen(first) - 1] == ' ' || first[strlen(first) - 1] == '\t')
      {
	first[strlen(first) - 1] = 0;;
      }//these whiles take care of blank space
    
    int oldout = dup(1); //copies stdout elsewhere. oldout stores it
    int olderr = dup(2); //copies stderr elsewhere. olderr stores it
    int fil;
    if( access( next, F_OK ) != -1 ) {
      if(mode % 2){//gets odd numbered modes. They are the append ones
	fil = open(next, O_RDWR | O_APPEND);
      }
      else{
	fil = open(next, O_RDWR | O_TRUNC);
      }
    }
    else {
      if(mode % 2){//gets odd numbered modes. The are the append ones
	fil = open(next, O_RDWR | O_CREAT | O_APPEND, 0644);
      }
      else{
	fil = open(next, O_RDWR | O_CREAT | O_TRUNC, 0644);
      }
    } //opens the file specified after the >
    if(mode != 2 && mode != 3){//modes 2 and 3 only redirect stderr
      dup2(fil, 1);//redirects stdout into the file
    } 
    else if(mode > 1){//modes greater than 1 use stderr
      dup2(fil, 2);//redirects stderr into the file
    }
    run_cmd_fork(first);
    dup2(oldout, 1);//restores stdout to fd 1
    dup2(olderr, 2);//restores stderr to fd 2
    close(fil);
  }
}

int run_pipeline(char *cmd) {
    char* first = strsep(&cmd, "|");
    char** parsed = parse_cmd(first);
    if (cmd == NULL) {
        return execvp(parsed[0], parsed);
    }
    pid_t fds[2];
    if (pipe(fds) == -1) {
        printf("pipe failed!");
        exit(1);
    }
    pid_t p = fork();
    if (p == 0) {
        // Redirect stdout to the input of fds
        dup2(fds[1], STDOUT_FILENO);
        execvp(parsed[0], parsed);
        exit(1); // If it's gotten here, the command failed
    } else {
        close(fds[1]);
        pid_t q = fork();
        if (q == 0) {
            dup2(fds[0], STDIN_FILENO);
            run_pipeline(cmd);
            exit(1);
        } else {
            close(fds[0]);
            int pstatus = 0, qstatus = 0;
            waitpid(p, &pstatus, 0);
            waitpid(q, &qstatus, 0);
        }
    }
    return 0;
}


/**
 *Args: Command that inputs from a file
 *Return: Void
 *What it Does: Executes a command and instead taking arguments from stdin, it  *takes commands from a file.
 */

void run_cmd_stdin(char* input){

  if (input == NULL) {
    return;
  }
  // Proceed if input is non-empty
  if (input[0]) {
    char* next = input;
    char* first = strsep(&next, "<");
    while (next[0] == ' ' || next[0] == '\t') {
      next++;
    }
    while(next[strlen(next) - 1] == ' ' || next[strlen(next) - 1] == '\t'){
      next[strlen(next) - 1] = 0;;
    }
    while(first[strlen(first) - 1] == ' ' || first[strlen(first) - 1] == '\t')
      {
	first[strlen(first) - 1] = 0;;
      }
    //copies stdout elsewhere. oldout stores it
    int oldin = dup(0);
    //opens the file specified after the <
    int fil;
    fil = open(next, O_RDONLY);

    //redirects stdin into the file
    dup2(fil, 0);
    run_cmd_fork(first);
    //restores stdin to fd o
    dup2(oldin, 0);
    close(fil);
  }
}

/**
 *Args: A command containing an &&, || operator. Mode is if it is && or ||
 *Mode = 0 is ||. Mode = 1 is &&.
 *Return: Void
 *What it Does: If mode = 1: Executes the first command, and if it works 
 *executes the second command
 *If mode = 0: executes the first command and if it fails executes the second
 *command
 */

void run_cmd_andor(char* input, char mode){
  //Skip whitespace
  while (input[0] == ' ' || input[0] == '\t') {
    input++;
  }
  char* next = input;
  char* first;
  if(mode){
    first = strsep(&next, "&&");
  }else{
    first = strsep(&next, "||");
  }
  next +=1;//moves past the extra "&" or "|"
  int status = run_cmd_fork(first);
  //If it works and status is and, or if it fails and status is or.
  if((!status && mode)||(status && !mode)){
    run_cmd_semi(next);
  }
}
/**
 *Args: Input string seperated by semicolons
 *Returns: void
 *What it does: It first checks if the input is negative. If this is not the 
 *case it checks for special characters, and runs either run_cmd_stdin, 
 *run_cmd_stdout, or just the normal run_cmd_fork on the portion of the command
 *before the first semicolon. It then recursively calls itself on the rest of
 *the input string.
 */
void run_cmd_semi(char* input){
  if (input == NULL) {
    return;
  }
  // Skip leading whitespace
  while (input[0] == ' ' || input[0] == '\t') {
    input++;
  }
  // Proceed if input is non-empty
  if (input[0]) {
    char* next = input;
    char* first = strsep(&next, ";");
    if(strstr(first, ">>") && !strstr(first, "2>>")){
      run_cmd_stdout(first, 1);
    }
    else if(strstr(first, "2>") && !strstr(first, "2>>")){
      run_cmd_stdout(first, 2);
    }
    else if(strstr(first, "2>>")){
      run_cmd_stdout(first, 3);
    }
    else if(strstr(first, "&>")){
      run_cmd_stdout(first, 4);
    }
    else if(strstr(first, ">")){
      run_cmd_stdout(first, 0);
    }
    else if(strchr(first, '<')){
      run_cmd_stdin(first);
    }else if(strstr(first, "&&")||strstr(first, "||")){
      //If both are present reads left to right
      if(strstr(first, "&&")&&strstr(first, "||")){
	if(strstr(first, "&&")<strstr(first, "||")){
	  run_cmd_andor(first, 1);
	}else{
	  run_cmd_andor(first, 0);
	}
       //if just and
      }else if(strstr(first, "&&")){
	run_cmd_andor(first, 1);
      }else{//if just or
	run_cmd_andor(first, 0);
      }
    }else if (strchr(first, '|')) {
      run_pipeline(first);
    }
    else{
      run_cmd_fork(first);
    }
    return run_cmd_semi(next);
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
