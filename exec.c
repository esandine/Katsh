#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
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
    run_cmd_fork(first);
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

