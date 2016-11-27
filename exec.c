#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * Counts the number of blanks (tabs or spaces) in a string and returns it
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
 * Parse a command from the NULL-terminated string `char *input`
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
 * Run a command from input
 */
int run_cmd(char* input){
  char ** parse = parse_cmd(input);
  
  execvp(parse[0],parse);
  free(parse);
  return 0;
}

/**
 * Forks a process to run a command
 */
int run_cmd_fork(char* input){
  pid_t pid = fork();
  if(pid==0){
    run_cmd(input);
  }else{
    int status = 0;
    waitpid(pid, &status, 0);
  }
  return 0;
}

/**
 *Args: Command that outputs to a file
 *Return: Void
 *What it Does: Executes a command and instead of printing to stdout, it prints
 *to a file.
 */
void run_cmd_stdout(char* input){
  if (input == NULL) {
    return;
  }
  // Proceed if input is non-empty
  if (input[0]) {
    char* next = input;
    char* first = strsep(&next, ">");
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
    int oldout = dup(1);
    //opens the file specified after the >
    int fil;
    if( access( next, F_OK ) != -1 ) {
      fil = open(next, O_RDWR | O_TRUNC);
    } else {
      printf("got here\n");
      fil = open(next, O_RDWR | O_CREAT | O_TRUNC, 0644);
    }

    //redirects stdout into the file
    dup2(fil, 1);
    run_cmd_fork(first);
    //restores stdout to fd 1
    dup2(oldout, 1);
    close(fil);
  }
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
 * Run a semicolon-separated list of commands at *input, or does nothing
 * if input is NULL
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
    if(strchr(first, '>')){
      run_cmd_stdout(first);
    }
    else if(strchr(first, '<')){
      run_cmd_stdin(first);
    }
    else{
      run_cmd_fork(first);
    }
      run_cmd_semi(next);
  }
}



int main(){
  char input[256];
  while(1){
    printf("$ ");
    fgets(input, sizeof(input), stdin);
    *strstr(input,"\n")=0;//Replaces newline with null
    run_cmd_semi(input);
  }
  return 0;
}

