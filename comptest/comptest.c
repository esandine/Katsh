#define READLINE_LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline-6.3/readline.h"
#include "readline-6.3/history.h"

int main(){
  char prompt[256];
  strcpy(prompt,"This is the prompt");
  char input[256];
  strcpy(input, readline("HELLO"));
  printf("Input: %s", input);
  return 0;
}
