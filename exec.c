#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Counts the number of spaces in a string and returns it
 */
int num_spaces(char* str){
  int spaces = 0;
  while(*str){
    if(*str==' ')
      spaces++;
    str++;
  }
  return spaces;
}

/**
 * Parse a command from input
 */

char** parse_cmd(char* input){
  //*strstr(input,"\n")=0;//Replaces newline with null
  char ** parse = (char **)calloc(sizeof(char *), num_spaces(input) + 2);
  char ctr = 0;
  char* next = input;
  while(next){
    parse[ctr]=strsep(&next, " ");
    ctr++;
  }
  ctr = 0;
  while(parse[ctr]){
    //printf("%s\n", parse[ctr]);
    ctr++;
  }
  return parse;
}

/**
 * Run a command from input
 */
int run_cmd(char* input){
  //printf("Input: %s\n",input);
  //printf("Spaces: %d\n", num_spaces(input));
  char ** parse = parse_cmd(input);
  /*ctr = 0;
  while(parse[ctr]){
    printf("%s\n", parse[ctr]);
    ctr++;
    }*/
  execvp(parse[0],parse);
  free(parse);
  return 0;
}

int run_cmd_fork(char* input){
  int pid = fork();
  if(pid==0){
    run_cmd(input);
  }else{
    int status = 0;
    waitpid(pid,&status,0);
  }
  return 0;
}

int run_cmd_semi(char* input){
  char* next = input;
  char* first = strsep(&next, ";");
  //printf("First: %s\n", first);
  //printf("Next: %s\n", next);
  run_cmd_fork(first);
  if(next){
    //printf("iter1 done\n");
    run_cmd_semi(next);
  }
  return 0;
}
int main(){
  char input[256];
  while(1){
    fgets(input, sizeof(input), stdin);
    *strstr(input,"\n")=0;//Replaces newline with null
    run_cmd_semi(input);
  }
}

