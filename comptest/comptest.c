#define READLINE_LIBRARY
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(){
  char prompt[256];
  strcpy(prompt,"This is the prompt");
  char input[256];
  readline("hello");
  printf("Input: %s", input);
  return 0;
}
