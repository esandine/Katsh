#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pwd(char* path){
  chdir(path);
}
int checkCommand(**parsed){
  if(strcmp(parsed[0], "cd")==0){
    chdir(path);
  }
}
int exit(){
  return 0;
}
int main(){
  return 0;
}
