#ifndef EXEC_H
#define EXEC_H

int num_blanks(char* str);
char** parse_cmd(char* input);
int wrath_of_kats();
int run_pipeline(char * cmd);
int run_cmd_fork(char* input);
void run_cmd_stdout(char* input, int mode);
void run_cmd_stdin(char* input);
void run_cmd_semi(char* input);
void run_cmd_andor(char* input, char mode);

#endif
