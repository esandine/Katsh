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
 * Runs a command from stdin
 */
int run_cmd(){
  char input[42];
  fgets(input, sizeof(input), stdin);
  *strstr(input,"\n")=0;
  //printf("Input: %s\n",input);
  //printf("Spaces: %d\n", num_spaces(input));
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
  execvp(parse[0],parse);
  free(parse);
  return 0;
}
int main(){
  run_cmd();
}
