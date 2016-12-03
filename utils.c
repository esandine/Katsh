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
 *Args: None
 *Returns: It fork bombs the user recursively
 *What it does: Forks and then calls itself. It acts as a remote force quit for
 *all processes running. Because why not?
 */
int wrath_of_kats(){
  int f = fork();
  return wrath_of_kats();
}
